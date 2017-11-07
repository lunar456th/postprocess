#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage dog.sh [gff file]"
	exit 0
fi

bool=0
gff=$1

cat $gff | fgrep UNDEF > dogtemp.txt 

rm -f dognumber.txt # number of CDS
touch dognumber.txt

while read line
do
	temp=`echo $line | grep -o 'gene' | wc -w`
	if [ $temp -eq 1 ]
	then
		if [ $bool -ne 0 ]
		then
			echo $count >> dognumber.txt
			bool=1
		fi
		count=0
	#elif [ "$line" == *"CDS"* ]
	#then
	else
		count=`expr $count + 1`
		bool=1
	fi
done < dogtemp.txt

echo $count >> dognumber.txt

rm -f dogtemp.txt

