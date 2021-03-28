#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed
#define MAXL 1000

//compute the maximum of three unsigned long
inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}
typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

//edge list structure:
typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
	unsigned long *cd;//cumulative degree cd[0]=0 length=n+1
	unsigned long *adj;//concatenated lists of neighbors of all nodes
} adjlist;

//relabel_nodes
typedef struct
{   unsigned long n;//number of nodes
    unsigned long e;//number of edges
    unsigned long* NewNodes;
    unsigned long* OldNodes;
}New_Nodes;

New_Nodes* relabel_nodes(char* file_name){ //function to change the label of nodes for easier manipulation later on
    // each node i in the text file is represented by nodes[i], for instance the first node is relabled 1 , the second node 2 and so on and so forth
    FILE* file = fopen (file_name, "r");
    unsigned long i = 0;
    unsigned long j =0;
    unsigned long n = 0;
    unsigned long e = 0;
    while (fscanf(file,"%lu %lu", &i, &j)==2 )
    {
        n=max3(n,i,j);
        e++;
    }
    fclose (file);
    n++;
    New_Nodes* new_nodes = malloc(sizeof(New_Nodes));

	new_nodes->NewNodes=malloc(n*sizeof(unsigned long));
    new_nodes->OldNodes=malloc(n*sizeof(unsigned long));

	for (i=0;i<n;i++) {
		new_nodes->NewNodes[i]=2*n;
	}

	j=0;
    file = fopen (file_name, "r");
    unsigned long t = 0 ;
    unsigned long s=0;
    while (fscanf(file,"%lu %lu", &s, &t)==2 )
    {
        if (new_nodes->NewNodes[s]==2*n){
			new_nodes->NewNodes[s]=j;
			new_nodes->OldNodes[j]=s;
			j++;
		}
		if (new_nodes->NewNodes[t]==2*n){
			new_nodes->NewNodes[t]=j;
			new_nodes->OldNodes[j]=t;

			j++;
		}
    }

    new_nodes->n=j;
	new_nodes->e=e;

    new_nodes->OldNodes = realloc(new_nodes->OldNodes,new_nodes->n*sizeof(unsigned long));

	return (new_nodes);


}
void free_New_Nodes(New_Nodes *A){
	free(A->NewNodes);
	free(A->OldNodes);
	free(A);
}





//ADJ_Array
typedef struct {
    unsigned long n;
    unsigned long m;
    unsigned long* cd;
    unsigned long* adj;
} adjarray;

adjarray* adjacency_array(char* input)
{
    New_Nodes* new_nodes = relabel_nodes(input);
    unsigned long n = new_nodes->n;
    unsigned long* Degrees = malloc(n*sizeof(unsigned long));
    unsigned long k=0;
    for (k=0; k<n ; k++)
    {
        Degrees[k]=0;
    }
    FILE* file = fopen (input, "r");
    unsigned long s = 0;
    unsigned long t =0 ;
    while (fscanf(file,"%lu %lu", &s, &t)==2)
    {
        Degrees[new_nodes->NewNodes[s]]++;
        Degrees[new_nodes->NewNodes[t]]++;
    }
    fclose (file);
    adjarray *ADJ_ARRAY = malloc(sizeof(adjarray)) ;
    ADJ_ARRAY->n = new_nodes->n;
    ADJ_ARRAY->m = new_nodes->e;

    ADJ_ARRAY->cd = malloc((ADJ_ARRAY->n +1)*sizeof(unsigned long)) ;
    ADJ_ARRAY->adj= malloc((2*ADJ_ARRAY->m)*sizeof(unsigned long));
    unsigned long p =0;
    for (p=0; p<ADJ_ARRAY->n +1; p++)
    {
        ADJ_ARRAY->cd[p] = 0 ;
    }

    for (p=1; p<ADJ_ARRAY->n +1 ; p++)
    {
        ADJ_ARRAY->cd[p] = Degrees[p-1] + ADJ_ARRAY->cd[p-1] ;

    }
    unsigned long* d = malloc(ADJ_ARRAY->n*sizeof(unsigned long));
    for (k=0;k<ADJ_ARRAY->n;k++) { d[k] = 0 ;}
    s =0;
    t =0;
    unsigned long s1=0;
    unsigned long t1=0;
    file = fopen (input, "r");
    while (fscanf(file,"%lu %lu", &s, &t)==2)
    {
        s1 = new_nodes->NewNodes[s];
        t1 = new_nodes->NewNodes[t];
        ADJ_ARRAY->adj[ ADJ_ARRAY->cd[s1] + d[s1] ] =t1;
        d[s1] ++;
        ADJ_ARRAY->adj[ ADJ_ARRAY->cd[t1] + d[t1] ] =s1;
        d[t1] ++;


    }
    fclose (file);

    free(Degrees);
    free_New_Nodes(new_nodes);
    free(d);
    return ADJ_ARRAY;


}

void free_adjarray(adjarray* a){
	free(a->cd);
	free(a->adj);
	free(a);
}

