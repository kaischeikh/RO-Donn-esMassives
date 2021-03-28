#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "adjarray.h"



unsigned long *degree_list(adjlist* g){
    /* Return the list of degrees of all nodes */
    unsigned long *degree=calloc(g->n,sizeof(unsigned long));
    for (int i=0;i<g->n;i++){degree[i] = g->cd[i+1] - g->cd[i];}
    return degree;
}


unsigned long int Nb_linked_nodes(adjlist* g){

    /* Compute the number of linked nodes in a graph*/

    unsigned long int nb_linked_nodes = 0;
    unsigned long int nbnodes = g->n;
    unsigned long int nbedges = g->e;
    unsigned int * d_out = malloc((nbnodes)*sizeof(unsigned int));
    unsigned int * d_in = malloc((nbnodes)*sizeof(unsigned int));
    for (unsigned long int i = 0; i < nbnodes; i++){d_out[i]=0;d_in[i]=0;}

    // count degree_in and degree_out for each node
    for (unsigned long int i = 0; i < nbedges; i++){
        d_out[g->edges[i].s]+=1;
        d_in[g->edges[i].t]+=1;
    }
    for (unsigned long int i = 0; i < nbnodes; i++){
        if(d_out[i]+d_in[i]!=0){nb_linked_nodes+=1;}
    }
        
    return nb_linked_nodes;
}

unsigned long int search_min_node(adjlist* g, unsigned long *d){
    /* Return the node with the minimum degree in a graph */
    unsigned long int min_node = 0;
    unsigned long int min_degree = d[0];
    for (unsigned long int i=1;i<g->n;i++){
        if (d[i]<min_degree){
            min_degree = d[i];
            min_node = i;
        }
    }
    return min_node;
}


int k_core(adjlist* g){
    /* Compute the k_core decomposition algorithm on a graph g*/
    unsigned long *d = degree_list(g);
    int min_node = search_min_node(g, d);
    int min_degree = d[min_node];
    int nb_edge = g->e;
    int nb_node = Nb_linked_nodes(g);

    // Initialize output variables
    int size_densest_core = Nb_linked_nodes(g);
    float max_density = (float)nb_edge/nb_node;
    float edge_density = (float)2*nb_edge/(nb_node*(nb_node-1));

    int i = g->n;
    int c = 0;
    while(min_degree==0){
        min_node = search_min_node(g, d);
        min_degree = d[min_node];
        if(min_degree==0){
            d[min_node] = g->n;
            i = i - 1;
        }
    }
    while(i > 0){
        min_node = search_min_node(g, d);
        min_degree = d[min_node];
        nb_node = nb_node - 1;
        nb_edge = nb_edge - min_degree;
        float degree_density = (float)nb_edge/nb_node;
        if (degree_density>max_density){
            max_density = degree_density;
            size_densest_core = nb_node;
            edge_density = (float)2*nb_edge/(nb_node*(nb_node-1));
        }
        c = (min_degree>c) ? min_degree : c;
        d[min_node] = g->n;
        for (int j = g->cd[min_node]; j < g->cd[min_node + 1]; j++){
            int neigh = g->adj[j];
            d[neigh] = d[neigh] - 1;
        }
        i = i - 1;
    }
    free(d);
    printf("K-core decomposition : \n");
    printf("the size of a densest core ordering prefix: %d\n",size_densest_core);
    printf("the edge density: %.6f\n",edge_density);
    printf("the average degree density: %.6f\n",max_density);
    return c;
}


int main(int argc,char** argv)
    {
        adjlist* g;
        time_t t1,t2,t3;
        t1=time(NULL);
        g=readedgelist(argv[1]);
        mkadjlist(g);     

        t2=time(NULL);   
        printf("the core value: %d\n",k_core(g));
        free_adjlist(g);

        t3=time(NULL);
        printf(" K-core decomposition time = %ldh%ldm%lds\n",(t3-t2)/3600,((t3-t2)%3600)/60,((t3-t2)%60));
    	printf("- Overall time = %ldh%ldm%lds\n",(t3-t1)/3600,((t3-t1)%3600)/60,((t3-t1)%60));
        return 0;
    }


  


/******* AMAZON GRAPH **********
gcc core.c -O9 -o core
./core ../com-amazon.ungraph.txt

K-core decomposition : 
the size of a densest core ordering prefix: 36
the edge density: 0.234921
the average degree density: 4.111111
the core value: 6
 K-core decomposition time = 0h4m51s
- Overall time = 0h4m51s

*********** LJ GRAPH ************
gcc core.c -O9 -o core
./core ../data/com-lj.ungraph.txt

K-core decomposition :
the size of a densest core ordering prefix: 386
the edge density: 0.992127
the average degree density: 190.984451
the core value: 360
 K-core decomposition time = 2h37m59s
- Overall time = 2h38m8s

*********** Orkut GRAPH ************
gcc core.c -O9 -o core
./core ../data/com-orkut.ungraph.txt

K-core decomposition : 
the size of a densest core ordering prefix: 25816
the edge density: 0.017654
the average degree density: 227.872177
the core value: 253
 K-core decomposition time = 1h54m2s
- Overall time = 1h55m53s
*************************************/