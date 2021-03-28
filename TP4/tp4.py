###################################################
###################################################
#################                 #################
#################    exercice1    #################
#################                 #################
###################################################
###################################################

import numpy as np
import networkx as nx
from matplotlib import pyplot as plt


def generate_graph(p=0.8,q=0.2,n=400,k=4):
    """

    Parameters
    ----------
    p : float, optional
        Each pair of nodes in the same cluster is connected with a probability p. The default is 0.8.
    q : float, optional
        Each pair of nodes in different clusters is connected with a probability q<p. The default is 0.2.
    n : TYPE, optional
        number of nodes. The default is 400.
    k : TYPE
        number of clusters.

    Returns
    -------
    G : TYPE
        A graph with n nodes with k clusters where each pair of nodes in the same cluster is connected with a probability p and each Each pair of nodes in different clusters is connected with a probability q<p

    """
    Clusters=list()
    for h in range(k):
        Clusters = Clusters + (n//k)*[h]
    G=nx.nx.Graph()
    for i in range(n):
        for j in range(i):
            r = np.random.random()
            if (Clusters [i] == Clusters[j]) and (r<p):
                G.add_edge(i,j)
            elif (Clusters [i] != Clusters[j]) and (r<q):
                G.add_edge(i,j)
    return G



# Main
num_nodes = 400
num_clusters = 4
p = 0.8
q = 0.2
G = generate_graph(p=p,q=q,n=num_nodes,k=num_clusters) # generating the graph

Clusters = sum([(num_nodes//num_clusters)*[h] for h in range(num_clusters)],[])  # the considered clusters

#drawing the graph
Colors=[]
for i in range(G.number_of_edges()):
    if (Clusters[list(G.edges)[i][0]]  == Clusters[list(G.edges)[i][1]] ):
        Colors.append('r')
    else: 
        Colors.append('b')
        

plt.figure(figsize=(6,6))
nx.draw_networkx(G,arrows=False, with_labels = False , node_size =30, alpha = 0.5, node_color =Clusters, width =0.05,edge_color =Colors)


#Graph implementation for different values of p and q

# p = 0.02 and q = 0.02
p = 0.02
q = 0.02
G1 = generate_graph(p=p,q=q,n=num_nodes,k=num_clusters) # generating the graph

Clusters = sum([(num_nodes//num_clusters)*[h] for h in range(num_clusters)],[])  # the considered clusters

#drawing the graph
Colors1=[]
for i in range(G1.number_of_edges()):
    if (Clusters[list(G1.edges)[i][0]]  == Clusters[list(G1.edges)[i][1]] ):
        Colors1.append('r')
    else: 
        Colors1.append('b')
        

plt.figure(figsize=(6,6))
nx.draw_networkx(G1,arrows=False, with_labels = False , node_size =30,
                 alpha = 0.5, node_color =Clusters, width =0.05,edge_color =Colors1)
plt.savefig("TP4-EX1_Figure2_p=002_q=002.png")
plt.show()


# p = 0.2 and q = 0.02
p = 0.2
q = 0.02
G2 = generate_graph(p=p,q=q,n=num_nodes,k=num_clusters) # generating the graph

Clusters = sum([(num_nodes//num_clusters)*[h] for h in range(num_clusters)],[])  # the considered clusters

#drawing the graph
Colors2=[]
for i in range(G2.number_of_edges()):
    if (Clusters[list(G2.edges)[i][0]]  == Clusters[list(G2.edges)[i][1]] ):
        Colors2.append('r')
    else: 
        Colors2.append('b')
        

plt.figure(figsize=(6,6))
nx.draw_networkx(G2,arrows=False, with_labels = False , node_size =30,
                 alpha = 0.5, node_color =Clusters, width =0.05,edge_color =Colors2)
plt.savefig("TP4-EX1_Figure3_p=02_q=002.png")
plt.show()



# p = 0.6 and q = 0.002
p = 0.6
q = 0.002
G3 = generate_graph(p=p,q=q,n=num_nodes,k=num_clusters) # generating the graph

Clusters = sum([(num_nodes//num_clusters)*[h] for h in range(num_clusters)],[])  # the considered clusters

#drawing the graph
Colors3=[]
for i in range(G3.number_of_edges()):
    if (Clusters[list(G3.edges)[i][0]]  == Clusters[list(G3.edges)[i][1]] ):
        Colors3.append('r')
    else: 
        Colors3.append('b')


plt.figure(figsize=(6,6))
nx.draw_networkx(G3,arrows=False, with_labels = False , node_size =30,
                 alpha = 0.5, node_color =Clusters, width =0.05,edge_color =Colors3)
plt.savefig("TP4-EX1_Figure4_p=06_q=002.png")
plt.show()

###################################################
###################################################
#################                 #################
#################    exercice2    #################
#################                 #################
###################################################
###################################################


def LabelPropagation(G,max_iteration=1000):
    """
    

    Parameters
    ----------
    G :  Networkx Graph
    max_iteration : TYPE, optional
        DESCRIPTION. The default is 1000.

    Returns a tuple (Labels, mapping)
    -------
    Labels contains clusters of each node with respect to the order of G.nodes()
    If you want to get  the cluster of node i, you can use Labels[mapping[i]]

    """
    nx.ordered
    n = G.number_of_nodes()
    mapping = dict(zip(G, range(n)))
    A = nx.relabel_nodes(G, mapping) 
    
    Labels = np.arange(n)
    New_Labels = np.arange(n)
    Nodes = np.arange(n)  

    Frequency_Labels = np.zeros(n)
    
    k=0
    while (k<max_iteration):
        
        np.random.shuffle(Nodes)
        for i in range(n):
            for j in A.neighbors(Nodes[i]):
                Frequency_Labels[Labels[j]]+=1
            if (Frequency_Labels[Labels[Nodes[i]]]!= np.max(Frequency_Labels)):
                zz = np.argwhere(Frequency_Labels==np.max(Frequency_Labels))
                zz = zz.flatten()
                New_Labels[Nodes[i]] = np.random.choice(zz)
            else:
                
                New_Labels[Nodes[i]] = Labels[Nodes[i]]
            Frequency_Labels = np.zeros(n)
        k+=1
        if (np.array_equal(New_Labels,Labels)):
            break
        Labels = np.copy(New_Labels)

    print("nombre d'iteration = ",k)
    return(Labels, mapping)

# p = 0.8 & q = 0.2

LP = LabelPropagation(G)
Labels = LP[0] # labels in new_node_labeling
Mapping = LP[1]

## Clusters in old_node_labeling
N= max(G)
Clusters_LP = []
for i in range(N+1):
    if (i in G.nodes()):
        Clusters_LP.append(Labels[Mapping[i]])

plt.figure(figsize=(6,6))
nx.draw_networkx(G,arrows=False, with_labels = False,
                 node_size =30, alpha = 0.5,
                 node_color =Clusters_LP,
                 width =0.05,edge_color =Colors)

plt.savefig("TP4-EX2_Figure1_p=08_q=02.png")
plt.show()

# p = 0.02 and q = 0.02

LP1 = LabelPropagation(G1)
Labels1 = LP1[0] # labels in new_node_labeling
Mapping1 = LP1[1]

## Clusters in old_node_labeling
N1= max(G1)
Clusters_LP1 = []
for i in range(N1+1):
    if (i in G1.nodes()):
        Clusters_LP1.append(Labels1[Mapping1[i]])

plt.figure(figsize=(6,6))
nx.draw_networkx(G1,arrows=False, with_labels = False,
                 node_size =30, alpha = 0.5,
                 node_color =Clusters_LP1,
                 width =0.05,edge_color =Colors1)

plt.savefig("TP4-EX2_Figure2_p=002_q=002.png")
plt.show()

# p = 0.2 and q = 0.02

LP2 = LabelPropagation(G2)
Labels2 = LP2[0] # labels in new_node_labeling
Mapping2 = LP2[1]

## Clusters in old_node_labeling
N2= max(G2)
Clusters_LP2 = []
for i in range(N2+1):
    if (i in G2.nodes()):
        Clusters_LP2.append(Labels2[Mapping2[i]])

plt.figure(figsize=(6,6))
nx.draw_networkx(G2,arrows=False, with_labels = False,
                 node_size =30, alpha = 0.5,
                 node_color =Clusters_LP2,
                 width =0.05,edge_color =Colors2)

plt.savefig("TP4-EX2_Figure3_p=02_q=002.png")
plt.show()

# p = 0.6 and q = 0.002

LP3 = LabelPropagation(G3)
Labels3 = LP3[0] # labels in new_node_labeling
Mapping3 = LP3[1]

## Clusters in old_node_labeling
N3= max(G3)
Clusters_LP3 = []
for i in range(N3+1):
    if (i in G3.nodes()):
        Clusters_LP2.append(Labels3[Mapping3[i]])

plt.figure(figsize=(6,6))
nx.draw_networkx(G3,arrows=False, with_labels = False,
                 node_size =30, alpha = 0.5,
                 node_color =Clusters_LP3,
                 width =0.05,edge_color =Colors3)

plt.savefig("TP4-EX2_Figure4_p=06_q=0002.png")
plt.show()




###################################################
###################################################
#################                 #################
#################    exercice3    #################
#################                 #################
###################################################
###################################################


import community as community_louvain
import matplotlib.cm as cm

partition = community_louvain.best_partition(G)
# draw the graph
pos = nx.spring_layout(G)
# color the nodes according to their partition
cmap = cm.get_cmap('viridis', max(partition.values()) + 1)
nx.draw_networkx_nodes(G, pos, partition.keys(), node_size=40,
                       cmap=cmap, node_color=list(partition.values()))
nx.draw_networkx_edges(G, pos, alpha=0.5)
plt.savefig("TP4-EX3_Figure_p=08_q=02.png")
plt.show()
