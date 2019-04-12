#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "curl.h"

static CURL * curl;

int
fc_curl_init ()
{
	curl = curl_easy_init();
	if ( !curl )
	{
		fprintf( stderr, "Curl initialisation error.\n" );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void
fc_curl_cleanup ()
{
	curl_easy_cleanup(curl);
}

int
fc_request ( const char * req, struct curl_payload * cpl )
{
	char * url = malloc(BUFSIZ);
	if ( url == NULL )
	{
		fprintf( stderr, "malloc() error" );
		return -1;
	}

	sprintf( url, "%s/%s/%s?units=si", DS_DOMAIN, DS_APIKEY, req );

	CURLcode code;
	cpl->size = 0;
	cpl->payload = malloc(0);

	code = curlfetch( url, cpl );
	curl_easy_reset( curl );

	if ( code != CURLE_OK || cpl->size < 1 )
		fprintf( stderr, "GET error: %s\n", curl_easy_strerror( code ) );
	if ( !cpl->payload )
		fprintf( stderr, "Callback is empty, nothing to do here\n" );

	free(url);
	return 0;
}

CURLcode
curlfetch( const char * url, struct curl_payload * fetch_str )
{
	CURLcode code;

	if ( fetch_str->payload == NULL )
	{
		fprintf( stderr, "Allocation failed in curlfetch()\n" );
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt( curl, CURLOPT_URL, url );
	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, curl_callback );
	curl_easy_setopt( curl, CURLOPT_WRITEDATA, ( void * ) fetch_str );
	curl_easy_setopt( curl, CURLOPT_USERAGENT, "libcurl-agent/1.0" );
	curl_easy_setopt( curl, CURLOPT_VERBOSE, 0L );
	code = curl_easy_perform(curl);
	return code;
}

size_t
curl_callback( void * content, size_t wk_size, size_t wk_nmemb, void * upoint )
{
	size_t rsize = wk_nmemb * wk_size;
	struct curl_payload * p = ( struct curl_payload * ) upoint;

	/* allocation for new size */
	p->payload = realloc( p->payload, p->size + rsize + 1 );
	if ( p->payload == NULL )
	{
		fprintf( stderr, "Reallocation failed in curl_callback()\n" );
		return -1;
	}

	/* making valid string */
	memcpy( &( p->payload[p->size] ), content, rsize );
	p->size += rsize;
	p->payload[p->size] = 0;
	return rsize;
}
