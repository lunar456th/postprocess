#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: consider_context.sh [gff file]"
	exit 0;
fi

gff=$1
saved_gene=

#total=`cat $gff | wc -l`
#count=0

rm -f _context.txt
touch _context.txt

while read line
do
	#count=`expr $count + 1`;echo $count'/'$total;
	semicoloncheck=`echo $line | fgrep ';'`
	#echo $saved_gene
	if [ "$semicoloncheck" ]
	then
		before_gene=`echo $line | cut -d' ' -f1-8`
		gene=`echo $line | cut -d' ' -f9`
		first=`echo $gene | cut -d';' -f1 | cut -d, -f1`
		first_rest=`echo $gene | cut -d';' -f1 | cut -d, -f2-8`
		second=`echo $gene | cut -d';' -f2 | cut -d, -f1`
		second_rest=`echo $gene | cut -d';' -f2 | cut -d, -f2-8`

		first_name=`echo ${first:0:1}`
		second_name=`echo ${second:0:1}`


		if [ "$first_name" = "Y" ]
		then
			first_name=`echo ${first:0:3}`
		fi

		if [ "$second_name" = "Y" ]
		then
			second_name=`echo ${second:0:3}`
		fi


		saved_name=`echo ${saved_gene:0:1}`
		if [ "$saved_name" = "Y" ]
		then
			saved_name=`echo ${saved_gene:0:3}`
		fi


		if [ $saved_name = $first_name ]
		then 
			echo $before_gene $first","$first_rest >> _context.txt
			saved_gene=$first
		elif [ $saved_name = $second_name ]
		then
			echo $before_gene $second","$second_rest >> _context.txt
			saved_gene=$second
		else
			first_prob=`echo $first_rest | cut -d, -f7 | tr -d '.'`
			second_prob=`echo $second_rest | cut -d, -f7 | tr -d '.'`
			echo $first_prob 'and' $second_prob
			if [ $first_prob -ge $second_prob ]
			then
				echo $before_gene $first","$first_rest >> _context.txt
				saved_gene=$first
			else
				echo $before_gene $second","$second_rest >> _context.txt
				saved_gene=$second
			fi
		fi

	else
		echo $line >> _context.txt
		saved_gene=`echo $line | cut -d' ' -f9 | cut -d, -f1`
	fi
	
	
done < $gff
