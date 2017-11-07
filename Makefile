all : interpolation realrefs new_generation complete_gff prob
	chmod ugo+x *.sh
	echo "Complete"

interpolation : interpolation.cpp
	g++ -std=c++11 -o interpolation interpolation.cpp
realrefs : realrefs.cpp
	g++ -std=c++11 -o realrefs realrefs.cpp
new_generation : new_generation.cpp
	g++ -std=c++11 -o new_generation new_generation.cpp 
complete_gff : complete_gff.cpp
	g++ -std=c++11 -o complete_gff complete_gff.cpp
prob : prob.c
	gcc -o prob prob.c

clean :
	rm -f interpolation realrefs new_generation complete_gff prob
