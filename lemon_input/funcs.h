#ifndef FUNCS_H_
#define FUNCS_H_

#include <sys/sysinfo.h>

#define LD_FLT(i) si.loads[i]/65536.0
#define MEMINFO_PATH "/proc/meminfo"
#define kilo(i) (i)/1024
#define fkilo(i) (i)/1024.0

//#define PLAYER_0_INVOKE "deadbeef --nowplaying \"%a ▚ %t ▚ %e/%l\" 2>/dev/null"
#define PLAYER_0_INVOKE "deadbeef --nowplaying \"╒╛ %a ╒╛ %t ╒╛ %e/%l\" 2>/dev/null"
#define PLAYER_1_INVOKE "sh ~/.config/dwm/cmus.sh"

struct network
{
	unsigned rxbytes;
	unsigned txbytes;
	unsigned rxbytes_old;
	unsigned txbytes_old;
	float rx_speed;
	float tx_speed;
};

struct usage_m
{
	unsigned long total;
	unsigned long free;
	unsigned long buffers;
	unsigned long cache;
	unsigned long swap;
	unsigned long shmem;
	unsigned long sreclaim;
	unsigned long used;
};

struct audio
{
	long volume;
	long perc_vol;
	long min;
	long max;
	int mute;
};


char * set_col          ( const char, const int );
void   alsa_get         ( struct audio * );
void   alsa_init        ( struct audio * );
void   current_playing  ( char *, int );
void   mm_used          ( struct sysinfo *, struct usage_m * );
void   net_get          ( struct network *, const char * );
void   delimeter_close  ( const int );
void   delimeter_close_r( const int );
void   delimeter_open   ( const int, const int );
void   delimeter_open_r ( const int, const int );
void   forecast         ( void );


#endif
