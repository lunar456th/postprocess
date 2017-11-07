#!/bin/bash

if [ $# -ne 4 ]
then
	echo "Usage: post_process.sh [blastx.out] [gff file] [fsa file] [reference gff file]"
	exit 0
fi

blastx=$1
gff=$2
fsa=$3
refgff=$4


##

./consider_context.sh $gff # output file is _context.txt

mv _context.txt $gff


## interpolation

./interpolation $blastx $gff

cat refs_tmp.txt | sed 's/, /{/g' | cut -d'{' -f1,2 | sed 's/{/ /g' | cut -d'{' -f1,2 | tr "\t" " " | cut -d' ' -f1,2,3,4,6,7,9 | sed 's/e-/{/g' | sed 's/-/ /g' | sed 's/{/e-/g' | sed 's/Chr /chr/g' > refs_tmp2.txt

sed -i 's/ A /-A /g' refs_tmp2.txt
sed -i 's/ B /-B /g' refs_tmp2.txt
sed -i 's/ C /-C /g' refs_tmp2.txt
sed -i 's/ D /-D /g' refs_tmp2.txt
sed -i 's/ E /-E /g' refs_tmp2.txt
sed -i 's/ F /-F /g' refs_tmp2.txt

cut -d' ' -f1-6 refs_tmp2.txt > refs_left.txt
cut -d' ' -f7 refs_tmp2.txt > refs_right_tmp.txt
cut -d, -f1 refs_right_tmp.txt > refs_right.txt
paste refs_left.txt refs_right.txt > refs.txt
sed -i 's/\t/ /g' refs.txt
sed -i 's/ NONE//g' refs.txt

cut -d' ' -f3,4 undefs.txt | sed 's/-/ /g' > undefs_tmp.txt
mv undefs_tmp.txt undefs.txt

rm -f refs_tmp.txt refs_tmp2.txt refs_left.txt refs_right_tmp.txt refs_right.txt


## realrefs

name=${gff%.gff}
_undef=$name'_undef.txt'

fgrep UNDEF $gff | fgrep gene | cut -d' ' -f1,4,5 > $_undef
./realrefs $_undef undefs.txt refs.txt > real_refs.txt


## posneg

cat $gff | fgrep UNDEF | fgrep CDS | cut -d' ' -f1,4,5,7 > posneg.txt


## dog

./dog.sh $gff # output file is dognumber.txt


## undef CDS

fgrep UNDEF $gff | fgrep CDS | cut -d' ' -f1,4,5 > undefs.txt


## new generation

mv real_refs.txt refs.txt

./new_generation $fsa undefs.txt posneg.txt $refgff refs.txt dognumber.txt > $name"_undef.gff"


## complete step

name_gene=$name"_gene.gff"
fgrep " gene " $gff > $name_gene
./complete_gff $name_gene $name"_undef.gff" > $name"_Complete.gff"

