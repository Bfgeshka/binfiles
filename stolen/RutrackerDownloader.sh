#!/bin/bash
## Rutracker Downloader by goooseman.ru ##
## Created on 13th of April 2013 ##


## Variables ##
TORRENTID=$1

## Variables checking ##
if [ ! $TORRENTID ];
  then
  echo Please use it as "scriptname.sh <topicid>"
	echo Topicid is the number at the end of the topic link. 
	echo Example: link is "http://rutracker.org/forum/viewtopic.php?t=4246831"
	echo Command is: ./RutrackerDownloader.sh 4246831
	exit 1
fi

if  [ -z $USERNAME ] || [ -z $PASSWORD ];
	then 
	echo Please setup USERNAME and PASSWORD variables 
	exit 1
fi

## Saving cookies ##
if [ ! -f cookies.txt ];
	then
	echo
	echo Saving cookies to cookies.txt...
	wget --user-agent=Mozilla/5.0 --save-cookies cookies.txt --post-data "login_username=$USERNAME&login_password=$PASSWORD&login=Вход" --no-check-certificate http://login.rutracker.org/forum/login.php
	echo Cookies saved...
	echo 
fi

## Downloading file ##
echo
echo Downloading .torrent...
wget --keep-session-cookies --load-cookies cookies.txt  --referer='http://rutracker.org/forum/viewtopic.php?t='$TORRENTID --header='Content-Type:application/x-www-form-urlencoded' --header='t:'$TORRENTID --post-data='t='$TORRENTID http://dl.rutracker.org/forum/dl.php?t=$TORRENTID -O [rutracker.org].t$TORRENTID.torrent
echo .torrent saved
echo

## Removing tmp files ##

if [ -f login.php ];
	then
	echo 
	echo Removing login.php...
	rm login.php*
	echo login.php deleted
fi
