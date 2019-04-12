#ifndef CONFIG_H_
#define CONFIG_H_
/*
 * Theme technicolor
 * BG 0:			black
 * BG 1:			dark blue
 * BG 2:			dark grey
 * BG 3:			light blue
 * BG 4:			blue
 *
 * FG 0:			black
 * FG 1:			white
 *
 * #define COLOR_BG_0 "%{B#ff111111}"
 * #define COLOR_BG_1 "%{B#ff5e7177}"
 * #define COLOR_BG_2 "%{B#ff333c43}"
 * #define COLOR_BG_3 "%{B#ff8bad9d}"
 * #define COLOR_BG_4 "%{B#ff214d58}"
 * #define COLOR_FG_0 "%{F#ff000000}"
 * #define COLOR_FG_1 "%{F#ffaaaaaa}"
*/
/*
#define COLOR_BG_0 "%{B#ff352f2f}"
#define COLOR_BG_1 "%{B#ff668c71}"
#define COLOR_BG_2 "%{B#ff41647b}"
#define COLOR_BG_3 "%{B#ffb2885d}"
#define COLOR_BG_4 "%{B#ff854340}"
#define COLOR_FG_0 "%{F#ff352f2f}"
#define COLOR_FG_1 "%{F#ffcacaca}"
#define UNDERLINE_SET "%{U#ff854340}"


#define COLOR_BG_0 "%{B#ff191919}"
#define COLOR_BG_1 "%{B#ffcacaca}"
#define COLOR_BG_2 "%{B#ff3f4c72}"
#define COLOR_BG_3 "%{B#fffc9700}"
#define COLOR_BG_4 "%{B#ff00899b}"
#define COLOR_FG_0 "%{F#ff191919}"
#define COLOR_FG_1 "%{F#fffafafa}"
#define UNDERLINE_SET "%{U#ff00899b}"*/
/* Internet connection interface */
/*#define net_iface "enp3s0"*/
//#define net_iface "wlp2s0"

struct n_iface
{
	char alias[4];
	char iface_name[8];
};

struct
{
	char c0[10];
	char c1[10];
	char c2[10];
	char c3[10];
	char c4[10];
	char c5[10];
} COLOURS;


/* Refresh delta */
#define DEF_TIMER 500000

/* Alsa soundcard */
#define card "default"
#define selem_name "Master"


#endif
