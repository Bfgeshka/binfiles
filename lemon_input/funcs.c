#include <alsa/asoundlib.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <linux/if_link.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "funcs.h"
#include "config.h"

char cbuf[BUFSIZ];
char fcast[BUFSIZ];
snd_mixer_t *handle;
snd_mixer_selem_id_t *sid;

void
net_get ( struct network * network, const char * net_iface )
{
	network->rxbytes_old = network->rxbytes;
	network->txbytes_old = network->txbytes;

	struct ifaddrs *ifaddr, *ifa;
	getifaddrs( &ifaddr );
	int if_iter;
	int family;
	ifa = ifaddr;
	if_iter = 0;
	for (; ifa != NULL; ifa = ifa->ifa_next, ++if_iter)
	{
		if ( ifa->ifa_addr == NULL )
			continue;

		family = ifa->ifa_addr->sa_family;

		if ( family == AF_PACKET && ifa->ifa_data != NULL && strcmp(ifa->ifa_name, net_iface) == 0 )
		{
			struct rtnl_link_stats *stats = ifa->ifa_data;
			network->rxbytes = stats->rx_bytes;
			network->txbytes = stats->tx_bytes;
		}
	}

	freeifaddrs(ifaddr);
	network->rx_speed = fkilo(network->rxbytes - network->rxbytes_old);
	network->tx_speed = fkilo(network->txbytes - network->txbytes_old);
}

#define CPY(x,y,z); strncpy( x, y, z ); break;
#define COL_LENGTH 10

char *
set_col ( const char a, const int t )
{
	char pref[2];
	char colour[COL_LENGTH];
	char * output = malloc(16);

	if ( a == 1 )
		pref[0] = 'F';
	else
	if ( a == 0 )
		pref[0] = 'B';
	pref[1] = '\0';

	switch (t)
	{
		case 0:		CPY( colour, COLOURS.c0, COL_LENGTH );
		case 1:		CPY( colour, COLOURS.c1, COL_LENGTH );
		case 2:		CPY( colour, COLOURS.c2, COL_LENGTH );
		case 3:		CPY( colour, COLOURS.c3, COL_LENGTH );
		case 4:		CPY( colour, COLOURS.c4, COL_LENGTH );
		case 5:		CPY( colour, COLOURS.c5, COL_LENGTH );
		default:	CPY( colour, COLOURS.c0, COL_LENGTH );
	}

	sprintf( output, "%%{%s%s}", pref, colour );
	return output;
}

#undef CPY
#undef COL_LENGTH

void
mm_used (struct sysinfo * si, struct usage_m * usage_m)
{
	usage_m->free    = kilo(si->freeram);
	usage_m->buffers = kilo(si->bufferram);
	usage_m->swap    = kilo(si->totalswap - si->freeswap);
	usage_m->total   = kilo(si->totalram);

	FILE * fd = fopen(MEMINFO_PATH, "r");
	while (fgets( cbuf, BUFSIZ, fd ))
	{
		if ( strstr( cbuf, "Cached:" ) )
			sscanf( cbuf, "Cached: %lu kB", &usage_m->cache );
		else
		if ( strstr( cbuf, "Shmem:" ) )
			sscanf( cbuf, "Shmem: %lu kB", &usage_m->shmem );
		else
		if ( strstr( cbuf, "SReclaimable:" ) )
			sscanf( cbuf, "SReclaimable %lu kB", &usage_m->sreclaim );
	}

	usage_m->used = usage_m->total - usage_m->cache - usage_m->buffers - usage_m->sreclaim - usage_m->free + usage_m->shmem;
	fclose(fd);
}


void
alsa_init ( struct audio * audio )
{
	snd_mixer_open( &handle, 0 );
	snd_mixer_attach( handle, card );
	snd_mixer_selem_register( handle, NULL, NULL );
	snd_mixer_load( handle );

	snd_mixer_selem_id_alloca( &sid );
	snd_mixer_selem_id_set_index( sid, 0 );
	snd_mixer_selem_id_set_name( sid, selem_name );
	snd_mixer_elem_t * elem = snd_mixer_find_selem( handle, sid );
	snd_mixer_selem_get_playback_volume_range( elem, &audio->min, &audio->max );
	snd_mixer_free( handle );
	snd_mixer_detach( handle, card );
	snd_mixer_close( handle );
	snd_config_update_free_global();
}


void
alsa_get ( struct audio * audio )
{
	snd_mixer_open( &handle, 0 );
	snd_mixer_attach( handle, card );
	snd_mixer_selem_register( handle, NULL, NULL );
	snd_mixer_load( handle );

	snd_mixer_selem_id_alloca( &sid );
	snd_mixer_selem_id_set_index( sid, 0 );
	snd_mixer_selem_id_set_name( sid, selem_name );
	snd_mixer_elem_t * elem = snd_mixer_find_selem( handle, sid );
	snd_mixer_selem_get_playback_volume( elem, SND_MIXER_SCHN_MONO, &audio->volume );
	snd_mixer_selem_get_playback_switch( elem, SND_MIXER_SCHN_MONO, &audio->mute );
	snd_mixer_free( handle );
	snd_mixer_detach( handle, card );
	snd_mixer_close( handle );
	snd_config_update_free_global();

	audio->perc_vol = audio->volume * 100 / audio->max;
}

void
forecast ()
{
	char dir[BUFSIZ];
	if ( getenv("TMPFSPATH") == NULL )
		strcpy( dir, "/mnt/tmp");
	else
		strcpy( dir, getenv("TMPFSPATH") );

	strcat( dir, "/forecast_short" );

	FILE * fp = fopen( dir, "r" );
	int err = errno;
	if ( err > 0 )
	{
		sprintf( fcast, "no forecast" );
		return;
	}
	if ( fgets( fcast, BUFSIZ, fp ) == NULL )
	{
		fprintf( stderr, "no forecast" );
		sprintf( fcast, "no forecast");
	}
	fclose(fp);
}


void
current_playing ( char * track_out, int player )
{
	FILE * out;
	if ( player == 0 )
	{
		out = popen(PLAYER_0_INVOKE, "r");
		if ( fgets(track_out, BUFSIZ, out) == NULL )
			sprintf( track_out, "ddb: empty output" );
		pclose(out);
	}
	else if ( player == 1 )
	{
		out = popen(PLAYER_1_INVOKE, "r");
		if ( fgets(track_out, BUFSIZ, out) == NULL )
			sprintf( track_out, "cmus: empty output" );
		else
			track_out[strlen(track_out) - 1] = '\0';
		pclose(out);
	}
}

//  ═ ║ ╒ ╓ ╔ ╕ ╖ ╗ ╘ ╙ ╚ ╛ ╜ ╝ ╞ ╟ ╠ ╡ ╢ ╣ ╤ ╥ ╦ ╧ ╨ ╩ ╪ ╫ ╬  ⌊ ⌋
void
delimeter_close ( const int a )
{
	char * aa = set_col( 1, a );
	char * bb = set_col( 0, 0 );
	char * cc = set_col( 1, 0 );
//	printf( "%s%s▓▒░ %s%s", aa, bb, cc, bb );
//	printf( "%s%s◘⮀⮁ %s%s", aa, bb, cc, bb );
//	printf( "%s%s▛▞ %s%s", aa, bb, cc, bb );
	printf( "%s%s═╗ %s%s", aa, bb, cc, bb );
	free(aa);
	free(bb);
	free(cc);
}

void
delimeter_close_r ( const int a )
{
	char * aa = set_col( 1, a );
	char * bb = set_col( 0, 0 );
	char * cc = set_col( 1, 0 );
//	printf( "%s%s▓▒░ %s%s", aa, bb, cc, bb );
//	printf( "%s%s◘⮀⮁ %s%s", aa, bb, cc, bb );
//	printf( "%s%s▙▚ %s%s", aa, bb, cc, bb );
	printf( "%s%s═╗ %s%s", aa, bb, cc, bb );
	free(aa);
	free(bb);
	free(cc);
}

void
delimeter_open ( const int a, const int c )
{
	char * aa = set_col( 1, a );
	char * bb = set_col( 0, 0 );
	char * cc = set_col( 1, c );
	char * dd = set_col( 0, a );
//	printf( "%s%s ░▒▓%s%s", aa, bb, cc, dd );
//	printf( "%s%s ⮃⮂◘%s%s", aa, bb, cc, dd );
//	printf( "%s%s ▞▟%s%s", aa, bb, cc, dd );
	printf( "%s%s ╔═%s%s", aa, bb, cc, dd );
	free(aa);
	free(bb);
	free(cc);
	free(dd);
}


void
delimeter_open_r ( const int a, const int c )
{
	char * aa = set_col( 1, a );
	char * bb = set_col( 0, 0 );
	char * cc = set_col( 1, c );
	char * dd = set_col( 0, a );
//	printf( "%s%s ░▒▓%s%s", aa, bb, cc, dd );
//	printf( "%s%s ⮃⮂◘%s%s", aa, bb, cc, dd );
//	printf( "%s%s ▚▜%s%s", aa, bb, cc, dd );
	printf( "%s%s ╔═%s%s", aa, bb, cc, dd );
	free(aa);
	free(bb);
	free(cc);
	free(dd);
}
