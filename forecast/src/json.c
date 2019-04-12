#include <jansson.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "json.h"
#include "curl.h"

char * json_storage;
char * forecast_current;
char * forecast_hourly;
char * tmppath_cur;
char * tmppath_verb;

const char * icons[] = { "clear-day", "clear-night", "rain", "snow", "sleet", "wind", "fog", "cloudy", "partly-cloudy-day", "partly-cloudy-night" };
const char * icons_pict[] = { "☀", "★", "☂", "☃", "☂/☃", "⥽", "░▒▓", "☁", "☁/☀", "☁/★" };
//json_error_t json_err;

int
do_a_thing ()
{
	extern float latitude;
	extern float longitude;
	struct curl_payload cpl_storage;
	struct curl_payload * cpl = &cpl_storage;
	char * url = malloc(BUFSIZ);

	sprintf( url, "%f,%f", latitude, longitude );
	fc_request( url, cpl );
	FREE(url);

	json_storage = malloc( cpl->size + 1 );
	sprintf( json_storage, "%s", cpl->payload );
	json_storage[ cpl->size ] = 0;
	FREE(cpl->payload);

	forecast_current = malloc(BUFSIZ);
	forecast_hourly  = malloc( BUFSIZ * 256 );

	tmppath();

	if ( fc_short_current() < 0)
		return -1;
	if ( fc_long_current() < 0)
		return -2;
	return 0;
}

int
fc_short_current ()
{
	json_auto_t * j_main;
	json_auto_t * j_current;
	json_error_t  json_err;

	j_main = json_loads( json_storage, 0, &json_err );
	if ( !j_main )
	{
		fprintf( stderr, "JSON parsing error: main response.\n%d:%s\n", json_err.line, json_err.text );
		json_decref(j_main);
		return -1;
	}

	j_current = json_object_get( j_main, "currently" );
	if ( !j_current )
	{
		fprintf( stderr, "JSON parsing error: current forecast.\n%d:%s\n", json_err.line, json_err.text );
		return -2;
	}

	sprintf( forecast_current,
	         "%s%.1f˚C as %.1f˚C, %s %s",
	         timewrite( js_get_int( j_current, "time" ) ),
	         js_get_dbl( j_current, "temperature" ),
	         js_get_dbl( j_current, "apparentTemperature" ),
	         icon(js_get_str( j_current, "icon" )),
	         js_get_str( j_current, "summary" )
	       );

	write_data( forecast_current, tmppath_cur );

	return 0;
}

int
fc_long_current ()
{
	json_auto_t * j_main;
	json_auto_t * j_current;
	json_auto_t * j_hourly;
	json_auto_t * j_h_data;
	json_error_t  json_err;

	j_main = json_loads( json_storage, 0, &json_err );
	if ( !j_main )
	{
		fprintf( stderr, "JSON parsing error: main response.\n%d:%s\n", json_err.line, json_err.text );
		json_decref(j_main);
		return -1;
	}

	j_hourly = json_object_get( j_main, "hourly" );
	if ( !j_hourly )
	{
		fprintf( stderr, "JSON parsing error: hourly forecast.\n%d:%s\n", json_err.line, json_err.text );
		return -3;
	}
	j_h_data = json_object_get( j_hourly, "data" );
	if ( !j_h_data )
	{
		fprintf( stderr, "JSON parsing error: hourly forecast.\n%d:%s\n", json_err.line, json_err.text );
		return -4;
	}

	json_auto_t * el;
	size_t index;
	char * t0;
	t0 = malloc(BUFSIZ);
	json_array_foreach( j_h_data, index, el )
	{
		sprintf( t0,
		         "%s%.1f( as %.1f )˚C, prec. %.0f%%, %s %s\n",
		         timewrite( js_get_int( el, "time" ) ),
		         js_get_dbl( el, "temperature" ),
		         js_get_dbl( el, "apparentTemperature" ),
		         js_get_dbl( el, "precipProbability" ) * 100,
		         icon(js_get_str( el, "icon" )),
		         js_get_str( el, "summary" )
		       );

		strcat( forecast_hourly, t0 );
		index += 3;
	}

	/* Verbose current situation */
	j_current = json_object_get( j_main, "currently" );
	if ( !j_current )
	{
		fprintf( stderr, "JSON parsing error: current forecast.\n%d:%s\n", json_err.line, json_err.text );
		return -2;
	}
	sprintf( t0,
	         "\n\n"
	         "%s%.1f˚C as %.1f˚C, %s %s\n"
	         "Visibility: %.1f km\n"
	         "Wind: %.1f %s\n"
	         "Clouds: %.0f%%\n"
	         "Humidity: %.1f%%\n"
	         "Precipation: %.0f%%\n",
	         timewrite( js_get_int( j_current, "time" ) ),
	         js_get_dbl( j_current, "temperature" ),
	         js_get_dbl( j_current, "apparentTemperature" ),
	         icon(js_get_str( j_current, "icon" )),
	         js_get_str( j_current, "summary" ),
	         js_get_dbl( j_current, "visibility" ),
	         js_get_dbl( j_current, "windSpeed" ),
	         wind_direction( js_get_int( j_current, "windBearing" ) ),
	         js_get_dbl( j_current, "cloudCover" ),
	         js_get_dbl( j_current, "humidity" ) * 100,
	         js_get_dbl( j_current, "precipProbability" ) * 100
	       );
	strcat( forecast_hourly, t0 );

	write_data( forecast_hourly, tmppath_verb );

	FREE(t0);
	return 0;
}

const char *
icon ( const char * input )
{
	ushort i;
	ushort elems = sizeof(icons) / sizeof(*icons);
	for ( i = 0; i < elems; ++i )
		if ( strcmp( input, icons[i] ) == 0 )
			return icons_pict[i];

	return icons_pict[0];
}

long long
js_get_int ( json_t * src, char * key )
{
	json_t * elem = json_object_get( src, key );
	return json_integer_value(elem);
}

double
js_get_dbl ( json_t * src, char * key )
{
	json_t * elem = json_object_get( src, key );
	return json_real_value(elem);
}

const char *
js_get_str ( json_t * src, char * key )
{
	json_t * elem = json_object_get( src, key );
	return json_string_value(elem);
}

void
fc_clear_memory ()
{
	FREE(json_storage);
	FREE(forecast_current);
	FREE(forecast_hourly);
}

const char *
timewrite ( long long epoch )
{
	time_t tc;
	struct tm * tms;
	tc = (unsigned long)epoch;
	tms = localtime(&tc);

	static char output[BUFSIZ];
	sprintf( output, "%02d/%02d, %02d:%02d: ", tms->tm_mon + 1, tms->tm_mday, tms->tm_hour, tms->tm_min );

	return output;
}

const char *
wind_direction ( long long deg )
{
	#define WIND(x, y) do { if ( deg < (x) ) return (y); } while(0)
	WIND( 22,  "⇣" );
	WIND( 77,  "⇙" );
	WIND( 112, "⇠" );
	WIND( 157, "⇖" );
	WIND( 202, "⇡" );
	WIND( 247, "⇗" );
	WIND( 292, "⇢" );
	WIND( 337, "⇘" );
	#undef WIND

	return "⇣";
}

void
tmppath ()
{
	char * dir = malloc(BUFSIZ);
	if ( getenv("TMPFSPATH") == NULL )
		strcpy( dir, "/tmp");
	else
		dir = getenv("TMPFSPATH");

	strcpy( dir, "/mnt/tmp" );

	tmppath_cur = malloc(BUFSIZ);
	sprintf(tmppath_cur, "%s/forecast_short", dir);

	tmppath_verb = malloc(BUFSIZ);
	sprintf(tmppath_verb, "%s/forecast_long", dir);
}

void
write_data ( const char * data, char * path )
{
	FILE * fp = fopen( path, "w" );
	fprintf( fp, "%s", data );
	fclose(fp);
}
