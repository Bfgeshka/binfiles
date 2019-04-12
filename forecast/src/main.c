#include <stdlib.h>
#include <stdio.h>

#include "args.h"
#include "curl.h"
#include "json.h"

int
main ( int argc, char ** argv )
{
	if ( fc_curl_init() == EXIT_FAILURE )
	{
		perror("curl failure");
		return EXIT_FAILURE;
	}

	if ( fc_get_args( argc, argv ) < 0 )
		return -1;


	do_a_thing();

	fc_clear_memory();
	fc_curl_cleanup();
	return EXIT_SUCCESS;
}
