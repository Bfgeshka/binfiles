#!/bin/sh
# Same as url_separator, but used for pp.vk-like links (same domain, but first part after it varies)
# Usage: sh ./vk_separator.sh FILE


OUTPUT_DIR="./urls/ppvk_sep"

# I'm using tmpfs for /tmp
INPUT_TXT=/tmp/vk_sep.txt
TEMP_FILE=/tmp/tail_vk.tmp
mv "$1" $INPUT_TXT

mkdir -p $OUTPUT_DIR
echo Separating PP.VK.ME URLs in $1...

CNTR=$(wc -l $INPUT_TXT | awk '{print $1}')

	until [ $CNTR -eq 0 ]
	do

	CUR_LINE=$(head -n1 $INPUT_TXT)
	CUR_DOMAIN=$(echo $CUR_LINE | awk -F/ '{print $4}')
	echo $CUR_LINE >> $OUTPUT_DIR/$CUR_DOMAIN.txt

#	tail -n +2 $INPUT_TXT > /tmp/tail_tmp; mv /tmp/tail_tmp $INPUT_TXT
	sed '1d' $INPUT_TXT > $TEMP_FILE && mv $TEMP_FILE $INPUT_TXT
	CNTR=$(wc -l $INPUT_TXT | awk '{print $1}')
	echo -ne "\t--$CNTR--\r"

	done

rm $INPUT_TXT
echo -e "\nDone."


exit 0
