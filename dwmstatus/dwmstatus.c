#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define CHAR_BUFFER 512
#define USLEEP_INT 500000 /* 100k = 0.1 second */
#define NOTIFY_FILE "/mnt/tmp/notif"
#define NOTIFY_TICKER_DELTA 5
#define TIMER_DELTA_TIMES 60

#define IND_IFEL(a, b) if ( !strcmp( sym, (a) ) ) {return (b);} else

Display * dpy;
Window root;

char *
change_indicator(char * sym)
{
	IND_IFEL("⢇","⢏");
	IND_IFEL("⢏","⡋");
	IND_IFEL("⡋","⡛");
	IND_IFEL("⡛","⠝");
	IND_IFEL("⠝","⠽");
	IND_IFEL("⠽","⠺");
	IND_IFEL("⠺","⢺");
	IND_IFEL("⢺","⢱");
	IND_IFEL("⢱","⣱");
	IND_IFEL("⣱","⣨");
	IND_IFEL("⣨","⣬");
	IND_IFEL("⣬","⣔");
	IND_IFEL("⣔","⣖");
	IND_IFEL("⣖","⡦");
	IND_IFEL("⡦","⡧");
	IND_IFEL("⡧","⢇");
	return ("⢇");
}

void
x_setroot_init()
{
	dpy = XOpenDisplay(NULL);
	if ( dpy == NULL )
	{
		fprintf(stderr, "ERROR: could not open display\n");
		exit(1);
	}
	root = XRootWindow( dpy, DefaultScreen(dpy) );
}

int
main()
{
	x_setroot_init();

	time_t curtime;
	char status[CHAR_BUFFER];
	char * wday;
	char notif[CHAR_BUFFER] = "dwmstatus initiated, welcome!";
	char temp_ch[CHAR_BUFFER] = "\0";
	char * indic = "";
	unsigned i;			/* Ticker */
	unsigned timer;
	FILE * fd = fopen ( NOTIFY_FILE, "w" );
	if ( fd != NULL ) fclose(fd);
	struct tm * ttime;

	for ( i = 0, timer = 0 ;; status[0] = '\0', ++i)
	{

		curtime = time(0);
		ttime = localtime(&curtime);
		switch ( ttime->tm_wday )
		{
		case 1:
			wday = "MON";
			break;
		case 2:
			wday = "TUE";
			break;
		case 3:
			wday = "WED";
			break;
		case 4:
			wday = "THU";
			break;
		case 5:
			wday = "FRI";
			break;
		case 6:
			wday = "SAT";
			break;
		case 0:
			wday = "SUN";
			break;
		}

		/* Runs before update checker for saving fresh notifications,
		 * limit is (multiplied by delta) steps */
		if ( timer == TIMER_DELTA_TIMES )
			notif[0] = '\0';

		/* Checking for updates each NOTIFY_TICKER_DELTA times */
		if ( i == NOTIFY_TICKER_DELTA )
		{
			++timer;
			i = 0;

			fd = fopen( NOTIFY_FILE, "r" );
			fgets( temp_ch, CHAR_BUFFER, fd );
			if ( fd != NULL ) fclose(fd);
			if ( strlen(temp_ch) > 1 )
			{
				fd = fopen( NOTIFY_FILE, "r");
				if (fgets( notif, CHAR_BUFFER, fd ) == NULL ) printf("fgets error\n");
				if ( fd != NULL ) fclose(fd);
				fd = fopen( NOTIFY_FILE, "w");
				if ( fd != NULL ) fclose(fd); /* Truncate file */
				notif[strlen(notif)-1] = '\0';	/* Fixing dirty output */
				temp_ch[0] = '\0';
				timer = 0;
			}

		}

		/* Indicator for decoration */
		indic = change_indicator(indic);

		sprintf(status, "\x01%s\x02%02d<-%02d\x01%s\x02%s\x01%02d:%02d:%02d ", \
		        notif, ttime->tm_mday, ttime->tm_mon + 1, wday, indic, ttime->tm_hour, ttime->tm_min, ttime->tm_sec);

		XStoreName( dpy, root, status );
		XFlush(dpy);

		if (usleep(USLEEP_INT)) printf ("sleep err");
	}

	XCloseDisplay(dpy);
	return 0;
}
