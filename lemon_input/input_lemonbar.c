#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "config.h"
#include "funcs.h"

struct player
{
	char name[16];
	char sig[64];
	char bt_prev[64];
	char bt_next[64];
	char bt_ctrl[512];
};

#define BUTTON_VOLUPDOWN_OPN "%{A4:mksh $HOME/.config/dwm/bar_volup.sh:}%{A5:mksh $HOME/.config/dwm/bar_voldown.sh:}"
#define BUTTON_MUTE "%{A2:mksh $HOME/.dwm/bar_mute.sh:}"
#define BT_CTRL_CLS "%{A}%{A}%{A}%{A}%{A}"

//#define D_RSLASH "▞"
//#define D_RSLASH "╱"
#define D_RSLASH "╒╛"
//#define D_BSLASH "▚"
//#define D_BSLASH "╲"
#define D_BSLASH "╘╕"


extern char fcast[BUFSIZ];
int pl_type = 0;
struct player pp[2];
struct n_iface netw[2];

void hdl ( int sig  )
{
	if ( sig == SIGUSR1  )
		pl_type = 0;
	else
	if ( sig == SIGUSR2  )
		pl_type = 1;
}

void
init ()
{
	sprintf( COLOURS.c0, "%s", "#ff191919" );
	sprintf( COLOURS.c1, "%s", "#fffafafa" );
	sprintf( COLOURS.c2, "%s", "#ffcacaca" );
	sprintf( COLOURS.c3, "%s", "#ff3f4c72" );
	sprintf( COLOURS.c4, "%s", "#fffc9700" );
	sprintf( COLOURS.c5, "%s", "#ff00899b" );

	strcpy( netw[0].alias, "et" );
	strcpy( netw[0].iface_name, "enp3s0" );
	strcpy( netw[1].alias, "wn" );
	strcpy( netw[1].iface_name, "wlp2s0" );

	strcpy( pp[0].name, "DDB" );
	strcpy( pp[0].sig, "%{A:kill -12 $(pgrep input_lemonbar):}" );
	strcpy( pp[0].bt_prev, "%{A:deadbeef --prev:}" );
	strcpy( pp[0].bt_next, "%{A:deadbeef --next:}" );
	strcpy( pp[0].bt_ctrl, "%{A3:deadbeef --stop:}%{A1:deadbeef --toggle-pause:}%{A4:mksh $HOME/.config/dwm/bar_volup.sh:}%{A5:mksh $HOME/.config/dwm/bar_voldown.sh:}%{A2:mksh $HOME/.dwm/bar_mute.sh:}" );

	strcpy( pp[1].name, "CMUS" );
	strcpy( pp[1].sig, "%{A:kill -10 $(pgrep input_lemonbar):}" );
	strcpy( pp[1].bt_prev, "%{A:cmus-remote -r:}" );
	strcpy( pp[1].bt_next, "%{A:cmus-remote -n:}" );
	strcpy( pp[1].bt_ctrl, "%{A3:cmus-remote -s:}%{A1:cmus-remote -u:}%{A4:mksh $HOME/.config/dwm/bar_volup.sh:}%{A5:mksh $HOME/.config/dwm/bar_voldown.sh:}%{A2:mksh $HOME/.dwm/bar_mute.sh:}" );
}

int
main ( int argc, char ** argv )
{
	init();

	for ( int i = 1; i < argc; ++i )
	{
		if ( argv[i][0] == '-' )
			switch ( argv[i][1] )
			{
				case 'h':
					printf( "help" );
					return 0;
				case 'c':
				{
					switch ( argv[i][2] )
					{
						case '0':
							sprintf( COLOURS.c0, "%s", argv[i+1] );
							break;
						case '1':
							sprintf( COLOURS.c1, "%s", argv[i+1] );
							break;
						case '2':
							sprintf( COLOURS.c2, "%s", argv[i+1] );
							break;
						case '3':
							sprintf( COLOURS.c3, "%s", argv[i+1] );
							break;
						case '4':
							sprintf( COLOURS.c4, "%s", argv[i+1] );
							break;
						case '5':
							sprintf( COLOURS.c5, "%s", argv[i+1] );
							break;
						default:
							break;
					}
				}
			}
	}

	struct network network;
	struct network network2;
	struct usage_m usage_m;
	struct audio audio;

	struct sigaction act;
	memset (&act, '\0', sizeof(act));
	act.sa_handler = hdl;

	if (-1 == sigaction(SIGUSR1, &act, NULL))
	{
		perror("sigaction()");
		exit(EXIT_FAILURE);
	}

	if (-1 == sigaction(SIGUSR2, &act, NULL))
	{
		perror("sigaction()");
		exit(EXIT_FAILURE);
	}

	char track_out[BUFSIZ];
	network.rxbytes = 0;
	network2.rxbytes = 0;
	network.txbytes = 0;
	network2.txbytes = 0;
	struct sysinfo si;
	int iterator = 0;


	/* Init values */
	sysinfo(&si);
	usage_m.total = kilo(si.totalram);
	mm_used(&si, &usage_m);
	alsa_init(&audio);
	net_get(&network, netw[0].iface_name);
	net_get(&network2, netw[1].iface_name);

	for ( ;; ++iterator, usleep(DEF_TIMER), sysinfo(&si), alsa_get(&audio) )
	{
		/* Plain stupid state machine */
		switch (iterator)
		{
		case 1:
			forecast();
			current_playing( track_out, pl_type );
			break;
		case 2:
			net_get(&network, netw[0].iface_name);
			net_get(&network2, netw[1].iface_name);
			break;
		case 3:
			current_playing( track_out, pl_type );
			break;
		case 4:
			net_get(&network, netw[0].iface_name);
			net_get(&network2, netw[1].iface_name);
			mm_used(&si, &usage_m);
			iterator = 0;
			break;
		}

		/* Left side */

		/* Playback control */
		/* ALSA on/off indicator */
		delimeter_open( 4, 1 );
		printf( "%s", BUTTON_VOLUPDOWN_OPN );
		if ( audio.mute )
			printf( "%s", ( audio.perc_vol ) ? " ∈" : " ⊄" );
		else
			printf(" ∉");

		/* ALSA volume and media binding */
		printf( " %ld⁒ %s%s ⏪ %s%s%s ⏯ %s%s%s ⏩ %s",
		        audio.perc_vol,
		        D_RSLASH,
		        pp[pl_type].bt_prev,
		        "%{A}",
		        D_RSLASH,
		        pp[pl_type].bt_ctrl,
		        BT_CTRL_CLS,
		        D_RSLASH,
		        pp[pl_type].bt_next,
		        "%{A}%{A}%{A}"
		      );

		/* Current player and song */
		printf( "%s%s %s%s %s%s%s ",
		        D_RSLASH,
		        pp[pl_type].sig,
		        pp[pl_type].name,
		        "%{A}",
		        pp[pl_type].bt_ctrl,
		        track_out,
		        BT_CTRL_CLS
		      );
		delimeter_close_r( 4 );

		/* Network */
		delimeter_open( 5, 4 );
		printf( " %.1f %s %.1f ⭫ ╔╬╗ ⭭ %.1f %s %.1f ", network.tx_speed, D_RSLASH, network2.tx_speed, network.rx_speed, D_RSLASH, network2.rx_speed );
		delimeter_close( 5 );

		/* Delimeting monitors */
		printf("%%{S2}");

		/* Right side */
		printf("%%{r}");

		/* Weather */
		delimeter_open( 2, 0 );
		printf( "%s %s %s", "%{A:mksh $HOME/.config/dwm/forecast.sh:}", fcast, "%{A}" );
		delimeter_close( 2 );

		/* Swap and RAM */
		delimeter_open( 5, 4 );
		printf( " ⇊ %.1fM %s %.2fM ⌆ ", fkilo(usage_m.swap), D_BSLASH, fkilo(usage_m.used) );
		delimeter_close_r( 5 );

		/* CPU */
		delimeter_open_r( 4, 1 );
		printf(" %.2f %s %.2f %s %.2f ", LD_FLT(0), D_BSLASH, LD_FLT(1), D_BSLASH, LD_FLT(2));
		delimeter_close_r( 4 );

		/* Close output */
		printf("\n");
		fflush(stdout);
	}

	return 0;
}
