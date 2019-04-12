#!/bin/mksh

# TMPFSPATH is environment variable for fast nonpersistent RAM storage like tmpfs
if [ -n $TMPFSPATH ]; then
	TMPFSPATH=/mnt/tmp
	if [ ! -d $TMPFSPATH/sxiv_cache ]; then
		mkdir $TMPFSPATH/sxiv_cache
	fi
	if [ ! -h ~/.cache/sxiv ]; then
		ln -s $TMPFSPATH/sxiv_cache ~/.cache/sxiv
	fi
	TMP_FILELIST=$TMPFSPATH/sxiv_filelist
fi

if [ $# -eq 0 ]; then
	echo "Usage: ${0##*/} PICTURES"
	exit
fi

[ "$1" == '--' ] && shift

SXIV="sxiv"

# protection from accidents
if [ $(pgrep $SXIV | wc -l) -ge 6 ]; then
	exit
fi

abspath () {
	case "$1" in
		/*) printf "%s\n" "$1";;
		*)  printf "%s\n" "$PWD/$1";;
	esac
}

target="$(abspath "$1")"
filenm="$(basename "$1")"
find -L "$(dirname "$target")" -maxdepth 1 -type f -regextype egrep -regex '.*(png|jpe?g?|gif|bmp)$' -print | sort > $TMP_FILELIST
count="$(cat "$TMP_FILELIST" | grep -m 1 -n -- "$filenm" | cut -d: -f1)"

options="-a -i"

if [ -n "$count" ]; then
	$SXIV $options -n "$count" -- < $TMP_FILELIST
else
	$SXIV -- "$@" # fallback
fi
