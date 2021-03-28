#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>//to estimate the runing time
#include <string.h>
#include "adjarray.h"
//maximum number of edges for memory allocation, will increase if needed
#define MAXL 1000
//size_graph
typedef struct
{   unsigned long n;//number of nodes
    unsigned long e;//number of edges
}Graph;

Graph getSize(char* input)
{
    New_Nodes* nodes = relabel_nodes(input);
    Graph G;
    G.n = nodes->n;
    G.e = nodes->e;
    return(G);
}







/******************************************FIFO**********************************/
typedef struct FIFO_Element FIFO_Element;
struct FIFO_Element
{
    unsigned long data;
    FIFO_Element *next;
};

typedef struct {
    FIFO_Element *First;
} FIFO;

FIFO *Initialization ()
{
    FIFO *fifo=malloc(sizeof(*fifo));
    FIFO_Element *element=malloc(sizeof(*element));
    if (fifo==NULL || element==NULL)
    {
        exit(0);
    }
    element=NULL;
    fifo->First=element;
    return(fifo);
}

void insert(FIFO *fifo,unsigned long newdata)
{
    FIFO_Element *element=malloc(sizeof(*element));
    if (fifo==NULL || element==NULL)
    {
        exit(0);
    }
    element->data=newdata;
    element->next=NULL;

    FIFO_Element* current_node=fifo->First;
    if(current_node==NULL){
            fifo->First=element;

    }
    else {
        while (current_node->next!=NULL)
        {
            current_node=current_node->next;
        }
        current_node->next=element;
    }
}
void pop(FIFO *fifo)
{
    if (fifo==NULL)
    {
        exit(0);
    }
    fifo->First=fifo->First->next;
}
int isempty(FIFO *fifo)
{
    if (fifo==NULL || fifo->First==NULL)
    {
        return(1);
    }
    else
    {
        return(0);
    }
}
void free_FIFO(FIFO *fifo){
	while (isempty(fifo)==0){
		 pop(fifo);
		
	}
}

/*********************************************************/
/*                 BFS                                   */
/*********************************************************/                


bool* BFS(char* input, unsigned long s){ //return a table that tels us if each node is visited or not.
    
	New_Nodes* new_nodes = relabel_nodes(input);
    unsigned long n=new_nodes->n;
    printf("Number of nodes = %d \n",new_nodes->n);
    printf("Number of edges = %d \n",new_nodes->e);
    unsigned long u=0;
    bool* Marked=malloc(n*sizeof(bool));
    int i=0;
    for (i=0;i<n;i++)
    {
        Marked[i]=0;
    }
    adjarray* A = adjacency_array(input);
	printf("Source node : %d \n",s);
	printf("Starting exploration : \n");
    FIFO  *fifo = NULL;
    fifo = Initialization();
    insert(fifo,new_nodes->NewNodes[s]);
    
    Marked[new_nodes->NewNodes[s]]=1;
    while (isempty(fifo)==0)
    {
        u=fifo->First->data;
     //   printf("%lu |",u);
        pop(fifo);
        for (i=A->cd[u];i<A->cd[u+1];i++){
            if ( Marked[A->adj[i]]==0 ){
                insert(fifo, A->adj[i]);
                printf("Node %d marked \n",A->adj[i]);
                Marked[A->adj[i]]=1;
            }
        }
    }
   free_adjarray(A);
   free_New_Nodes(new_nodes);
   printf("==done==");
   return (Marked);

}


/***********************************************************/
/*          Listing Triangles                             */
/*********************************************************/


int ListTriangles(char* input)
{	
    // Initialize result
  
    int count_Triangle = 0;
   
     New_Nodes* nodes = relabel_nodes(input);
    
    printf("n = %d \n",nodes->n);
    printf("e = %d \n",nodes->e);
    adjarray *A  = adjacency_array(input) ;

    unsigned long k=0;
    unsigned long l=0;

    FILE* file = fopen (input, "r");
    unsigned long i = 0;
    unsigned long j =0;
	int idx = 0;
	int p = 0;
    while (fscanf(file,"%lu %lu", &i, &j)==2)
    {	
        for (k=A->cd[nodes->NewNodes[i]]; k<A->cd[nodes->NewNodes[i]+1]; k++){
        	
            for (l=A->cd[nodes->NewNodes[j]]; l<A->cd[nodes->NewNodes[j]+1];l++){
                if ( (A->adj[k]==A->adj[l]) && (nodes->NewNodes[i]<nodes->NewNodes[j]) && (nodes->NewNodes[j]<A->adj[k])) {count_Triangle++;
                
                
				}
            }
        }
        p++;

    }
    fclose (file);
    free_New_Nodes(nodes);
    free_adjarray(A);
    
    return  count_Triangle;
}

/***********************************************************/
/*          Bounds on Graph diameter                      */
/*********************************************************/

typedef struct{
	int min;
	int max;
}minmax;

long* GetDistance(char* input, unsigned long s){ //return a table of distances of each node from s based on the BFS algorithm 
    New_Nodes* new_nodes = relabel_nodes(input);
    unsigned long n=new_nodes->n;
    unsigned long u=0;
    long* Distance=malloc(n*sizeof(long));
    int i=0;
    for (i=0;i<n;i++)
    {
        Distance[i]=-1;
    }
    adjarray* A = adjacency_array(input);
    FIFO  *fifo = NULL;
    fifo = Initialization();
    insert(fifo,new_nodes->NewNodes[s]);
    Distance[new_nodes->NewNodes[s]]=0;
    while (isempty(fifo)==0)
    {
        u=fifo->First->data;
       // printf("%lu |",u);
        pop(fifo);
        for (i=A->cd[u];i<A->cd[u+1];i++){
            if ( Distance[A->adj[i]]==-1 ){
                insert(fifo, A->adj[i]);
                Distance[A->adj[i]]=Distance[u] + 1;
            }
        }
    }
   free_adjarray(A);
   free_New_Nodes(new_nodes);
   return (Distance);

};

void quicksort(int number[],int first,int last){ //function to quicksort a table of integers 
   int i, j, pivot, temp;

   if(first<last){
      pivot=first;
      i=first;
      j=last;

      while(i<j){
         while(number[i]<=number[pivot]&&i<last)
            i++;
         while(number[j]>number[pivot])
            j--;
         if(i<j){
            temp=number[i];
            number[i]=number[j];
            number[j]=temp;
         }
      }

      temp=number[pivot];
      number[pivot]=number[j];
      number[j]=temp;
      quicksort(number,first,j-1);
      quicksort(number,j+1,last);

   }

   
}

int find_max(long a[], long n){ // function to find the maximum of a table in o(n) time 
  int c, index = 0;
  int max=a[0];
  for (c = 1; c < n; c++)
    if (a[c] > max)
      max = a[c];

  return max;
}
int find_maximum(long a[], long n){ // function to find the maximum of a table based on the quicksort 
  quicksort(a,0,n-1);
  return a[n-1];

}

long computeMean(long* a ,long n){ //function to compute the mean of a given table 
	int i;
	long sum = 0;
	for(i=0;i<n;i++)
		sum = sum + a[i];
	long result = (long) ((sum / n) + 0.5);
	return result;
}

minmax GetBounds(char* input, int niter){ //function to compute the lower and upper bounds of a graph 
	New_Nodes* nodes = relabel_nodes(input);
	printf("Number of nodes = %d \n",nodes->n);
    printf("Number of edges = %d \n",nodes->e);
    printf("Number of iterations (number of source nodes tested) = %d \n",niter);
	int n = nodes->n;
	int i ;
	int max,min;
	int j;
	long maxDistances[niter];
	//int minDistances[niter];
	printf("Computing bounds :");
	for(i=0;i<niter;i++)
	{		j = rand() % n;
			maxDistances[i] = find_max(GetDistance(input,nodes->OldNodes[j]),n);
			//minDistances[i] = computeMean(GetDistance(input,nodes->OldIndex[i]),n);
			printf("=");
	}
	printf("\n");		
			
	
	//long lowerbound = computeMean(minDistances,niter);
	long upperbound = find_max(maxDistances,niter);
	minmax bounds;
	bounds.min=upperbound;
	bounds.max= 2*upperbound;
	free_New_Nodes(nodes);
	return bounds;
		
}


int main(){
	time_t t1,t2,t3,t4;

	
    //length_graph L;
    //L = size_graph("com-orkut.ungraph.txt");
	//bool* Marked;
	//t1=time(NULL);
	//Marked = BFS("D:/small.txt",1);
		char* input;
	input = "D:/com-friendster.all.cmty.txt";
	printf("Testing on com-friendster.all.cmty graph \n");
	

	int LT;
	t1=time(NULL);
	LT = ListTriangles(input);
	printf(" Number of triangles in graph %d \n",LT); 
	t2=time(NULL);
	printf("- Overall time for Listing triangles = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
/*	
	minmax bounds ;
	t3=time(NULL);
	bounds = GetBounds(input,1);
	t4=time(NULL);
	printf("- Overall time for computing bounds = %ldh%ldm%lds\n",(t4-t3)/3600,((t4-t3)%3600)/60,((t4-t3)%60));
	printf("Lower Bound = %d \n",bounds.min);
	printf("Upper Bound = %d \n",bounds.max);
*/	



	
	return(0);
}
