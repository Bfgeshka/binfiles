#! /bin/bash
# Requires 'ag' aka 'the silver searcher'

WTHCACHE=$HOME/.cache/curweather


# Облачность, осадки, ветер
INTEL_DETAIL=$(													\
	cat $WTHCACHE											|	\
	grep -o "\"today-forecast\">[[:alnum:][:blank:]\,\/]*"	|	\
	sed -e 's/\"today-forecast\">//g'							\
)


TEMPER=$(										\
	cat $WTHCACHE							|	\
	ag -o ";:[-]?[0-9][0-9]?}}"				|	\
	sed -e 's/;://'								\
		-e 's/}}//'								\
	)


DETAIL=$(										\
	echo "$INTEL_DETAIL"					|	\
	grep -o "\,\ [[:alnum:][:blank:]]*\,"	|	\
	sed -e 's/\,$//'							\
		-e 's/пасмурно//'						\
		-e 's/переменная облачность//'			\
		-e 's/ясно//'							\
		-e 's/малооблачно//'					\
		-e 's/облачно с прояснениями//'			\
		-e 's/облачно//'						\
		-e 's/временами снег/☃\/☃/'			\
		-e 's/небольшой снег/☃/'				\
		-e 's/снег/☃\/☃\/☃/'					\
		-e 's/временами дождь/\/☂\//'			\
		-e 's/небольшой дождь/☂\/☂/'			\
		-e 's/дождь/☂\/☂\/☂/'					\
	)

WIND=$(											\
	echo "$INTEL_DETAIL"					|	\
	grep -o "ветер [0-9]* м/с"				|	\
	sed -e 's/ветер //'							\
		-e 's/ м\/с/mps/'						\
		-e 's/штиль /0mps/'						\
)

test -s $WTHCACHE && echo  $TEMPER\°C, $WIND$DETAIL
exit 0
