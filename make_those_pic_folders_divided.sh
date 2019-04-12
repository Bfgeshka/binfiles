#! /bin/sh

# Requires dir path in $1
# Sort files in dirs by size, places resulting dirs in the same level as $1


[ -z $1 ] && exit 1

cd $1
PICPATH=$(pwd)
NEWPATH=../../sl/"$(basename $PICPATH)"

zffind()
{
	WDIR=$NEWPATH/$1
	mkdir -p $WDIR
	find $PICPATH -type f -size $2 -exec mv -n -t $WDIR '{}' +
}

zffind 9999-8192$2 +8192k #$1
zffind 8192-4096$2 +4096k #$1
zffind 4096-2048$2 +2048k #$1
zffind 2048-1024$2 +1024k #$1
zffind 1024-0512$2 +512k #$1
zffind 0512-0256$2 +256k #$1
zffind 0256-0128$2 +128k #$1
zffind 0128-0064$2 +64k #$1
zffind 0000-0064$2 +0k #$1

exit 0
