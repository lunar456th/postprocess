#!/bin/bash

if [ $# -eq 0 ]
then
	echo "Usage: array.sh [reference gff file] [gff files] ..."
	exit 1
fi


##### make requisites #####

ref=$1
shift
prob=prob
touch all_genes
pwd=`pwd`
allarg=$*

for arg in $allarg
do
	name=${arg%.gff}
	cut -d' ' -f9 $arg > $name'_tmp1'
	cut -d, -f1,8 $name'_tmp1' > $name'_tmp2'
	cut -d';' -f1 $name'_tmp2' > $name'_tmp3'
	sort -u $name'_tmp3' > $name'_tmp4'
	cat $name'_tmp4' | fgrep -v 'UNDEF' > $name'_tmp5'
	cut -d, -f1 $name'_tmp5' > $name'_genes'
	cut -d, -f2 $name'_tmp5' > $name'_probs'

	cat $name'_genes' >> all_tmp

	rm -f $name'_tmp1'
	rm -f $name'_tmp2'
	rm -f $name'_tmp3'
	rm -f $name'_tmp4'
	rm -f $name'_tmp5'

	shift
done

cut -f9 $ref > ref1
cut -d';' -f1 ref1 > ref2
fgrep -v 'Parent' ref2 > ref3
cut -d= -f2 ref3 > ref4
cat ref4 >> all_tmp

rm -f ref1 ref2 ref3 ref4


##### make array #####

sort -u all_tmp > all_genes
rm -f all_tmp

rows=
names=`echo -e '\t'`

cd $pwd

touch array.txt
touch array_temp.txt

for arg in $allarg
do
	name=${arg%.gff}
	name_row=$name'_row'
	$pwd/$prob all_genes $name'_genes' $name'_probs' > $name_row 
	rows="$rows $name_row"
	echo $name > $name'_file'
	paste array_temp.txt $name'_file' > array_human_readable.txt
	mv -f array_human_readable.txt array_temp.txt

	rm -f $name'_genes'
	rm -f $name'_probs'
	rm -f $name'_file'
done

paste $rows > array.txt
paste all_genes array.txt > array_tmp.txt
cat array_tmp.txt >> array_temp.txt
mv -f array_temp.txt array_human_readable.txt
rm -f array_tmp.txt
rm -f $rows

