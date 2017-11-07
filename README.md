# PostProcessing
Source codes of AGAPE post processing

Usage

> make

> post_process.sh [blastx.out] [.gff file] [assembled sequence file( .fsa .fasta etc..)] [reference .gff file]

The output file is $(name)_undef.gff.


Function

1. selection of many genes corresponding according to prev/next context.

2. removal of introns and rematching.

3. restoration of genes which are short but very similar to corresponding reference gene.

