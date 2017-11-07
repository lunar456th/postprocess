#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH_OF_GENE_NAME 100

int main(int argc, char * argv[])
{
	if(argc != 4)
	{
		printf("usage: prob [ALL_genes] [xxx_genes] [xxx_probs]\n");
		return -1;
	}

	FILE * ALL_genes = fopen(argv[1], "r");
	FILE * STR_genes = fopen(argv[2], "r");
	FILE * STR_probs = fopen(argv[3], "r");

	char * all_gene = (char *)calloc(LENGTH_OF_GENE_NAME, sizeof(char));
	char * str_gene = (char *)calloc(LENGTH_OF_GENE_NAME, sizeof(char));
	char * str_prob = (char *)calloc(LENGTH_OF_GENE_NAME, sizeof(char));
	int compare;

	fscanf(STR_genes, "%s\n", str_gene);
	fscanf(STR_probs, "%s\n", str_prob);

	while (!feof(ALL_genes))
	{
		if (fscanf(ALL_genes, "%s\n", all_gene) == -1) return 0;
		if (all_gene != NULL && str_gene != NULL)
		{
			compare = strcmp(all_gene, str_gene);
		}
		if (compare == 0)
		{
			//printf("%s\n", str_prob);
			printf("%s\n", str_prob);
			fscanf(STR_genes, "%s\n", str_gene);
			fscanf(STR_probs, "%s\n", str_prob);
		}
		else
		{
			//printf("0\n");
			printf("0\n");
			if (compare < 0)
			{
				//if (fscanf(ALL_genes, "%s\n", all_gene) == -1) return 0;
			}
			else
			{
				if (all_gene == NULL) return 0;
				fscanf(STR_genes, "%s\n", str_gene);
				fscanf(STR_probs, "%s\n", str_prob);
			}
		}
	}
	return 0;
}
