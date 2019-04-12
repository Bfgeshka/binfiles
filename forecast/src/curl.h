#ifndef CURL_H_
#define CURL_H_

#include <curl/curl.h>

#define DS_DOMAIN "https://api.darksky.net/forecast"
#define DS_APIKEY "2ac716c2abd787485b12804051cdf195"

struct curl_payload
{
	char * payload;
	size_t size;
};


CURLcode curlfetch( const char *, struct curl_payload * );
int  fc_curl_init ( void );
int  fc_request ( const char *, struct curl_payload * );
size_t curl_callback( void *, size_t, size_t, void * );
void fc_curl_cleanup ( void );


#endif
