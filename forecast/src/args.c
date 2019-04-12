#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "args.h"

void
help_print ()
{
	puts("I'm helping");
}

int
fc_get_args ( int argc, char ** argv )
{
	ushort i;
	(void)argv;

	if ( argc < 2 )
	{
		help_print();
		return -1;
	}

	for ( i = 0; i < argc; ++i )
	{
		if ( argv[i][0] != '-' )
			continue;

		switch ( argv[i][1] )
		{
			case('h'):
				help_print();
				return -1;
			case('l'):
				if ( get_location( argv, i ) < 0 )
					return -2;
				break;
			default:
				puts("unknown argument");
				return -3;
		}
	}

	return 0;
}

int
get_location ( char ** argv, ushort index )
{
	if ( argv[index + 1] == NULL || argv[index + 2] == NULL )
	{
		perror("bad arguments input");
		latitude = default_lat;
		longitude = default_long;
		return -1;
	}

	latitude = atof( argv[index + 1] );
	if ( latitude > 90 || latitude < -90 )
	{
		perror("bad latitude");
		latitude = default_lat;
	}

	longitude = atof( argv[index + 2] );
	if ( longitude > 180 || longitude < -180 )
	{
		perror("bad longitude");
		longitude = default_long;
	}

	return 0;
}
