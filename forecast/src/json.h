#ifndef JSON_H_
#define JSON_H_

#include <jansson.h>

#define FREE(x) do { if ( x != NULL ) free(x); } while(0)

const char * icon ( const char * );
const char * js_get_str ( json_t *, char * );
const char * timewrite ( long long );
double       js_get_dbl ( json_t *, char * );
int          do_a_thing ( void );
int          fc_long_current ( void );
int          fc_short_current ( void );
long long    js_get_int ( json_t *, char * );
void         fc_clear_memory ( void );
void         tmppath ( void );
void         write_data ( const char *, char * );
const char * wind_direction ( long long );
#endif
