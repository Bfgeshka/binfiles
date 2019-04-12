#! /bin/bash
#	I suck at bash-scripting, so i'm using C for random number.
IFS=$'\n'
HUM=/home/bfg
RANDOMIZE_PATH=$HUM/bin/random
SOUND_PATH=$HUM/bin/alarms

cd $SOUND_PATH

SOUND_FILES_COUNT=$(ls -1 | wc -l)
RANDOMIZED=$($RANDOMIZE_PATH $SOUND_FILES_COUNT)

ALARM_FILE="$(find -type f | head -n$RANDOMIZED | tail -n1)"
mpv --loop-file=no "$ALARM_FILE"

exit 0
