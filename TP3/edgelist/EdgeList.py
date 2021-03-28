'''
    This file contains the implementaion of the class Edgelist.
    The functions and attributes are mainly related to PageRank
    algorithm.
'''
# Package import
import numpy as np
import time
from tqdm import tqdm
from copy import deepcopy
from math import sqrt

def  Normalize2P(P, P0, n, norm_1):
    '''
        This function Normalize the vector P for the 
        PageRank algorithm.
        Inputs:
            - P: vector coded as a dict with size n.
            - P0: vector coded as a dict for the Rooted 
            PageRank.
            - n: number of nodes.
            - norm_1: the norm 1 of the vector P.
        Outputs:
            - P: vector coded as a dict which is the 
            normalized vector of P.
    '''
    if P0 == None:
        for u in P:
            P[u] += (1 - norm_1) / n
    else:
        for u in P0:
            P[u] += P0[u] * (1 - norm_1)
    return P

class EdgeList:
  '''
  Class to model a graph using edgelist structure.
  This model used for the PageRank algorithm.
  '''
  def read_file_name(self, file_name):
    '''
      Read the names of nodes .
      Input:
        - file_name: file to read the node name pair.
      Add these attributes:
        - names: dict for each node it's name.
        - nodes: list of nodes in the graph.
        - n: number of nodes.
    '''
    self.names = {}
    self.nodes = []
    with(open(file_name, 'r')) as f:
      line = f.readline()
      while line:
        line = line.split()
        if len(line) != 0 and line[0] != "#":
          u = int(line[0])
          self.nodes.append(u)
          self.names[u] = " ".join(line[1:])
        line = f.readline()
    f.close()
    self.n = len(self.nodes)

  def read_file_edglist(self, file_name):
    '''
      Read a graph from file and store it as edgelist
      Input:
        - file_name: file to read the graph from.
      Add these attributes:
        - n: number of nodes.
        - m: number of edges.
        - edgelist: a dict with keys the nodes and values
                    list of neighbors.
        - deg_out: dict with keys the nodes
                  and value the out degree.
        - deg_in: dict with keys the nodes
                  and value the in degree.
    '''
    self.m = 0
    self.edgelist ={}
    with(open(file_name, 'r')) as f:
      line = f.readline()
      while line:
        line = line.split()
        if len(line) == 2:
          u = int(line[0])
          v = int(line[1])
          self.m += 1
          if not(u in self.edgelist.keys()):
            self.edgelist[u] = []
          self.edgelist[u].append(v)            
        line = f.readline()
    f.close()
    self.deg_out = {}
    self.deg_in = {}
    for u in self.nodes:
      self.deg_in[u] = 0
      if u in self.edgelist.keys():
        self.deg_out[u] = len(self.edgelist[u])
      else:
        self.deg_out[u] = 0
    for u, neighbors in self.edgelist.items():
      for v in neighbors:
        self.deg_in[v] +=1

  def __init__(self, file_names, file_graph):
    self.read_file_name(file_names)
    self.read_file_edglist(file_graph)
    self.compute_reverse_edgelist()
    self.get_end_nodes()

  def edge_exists(self, u, v):
    '''
     Return if there's an edge from u to v.
     Input:
      - edgelist: a graph stored as edgelist
      - u: starting node.
      - v: ending node.
    Output:
      - Boolean if u lead to v.
    '''
    return v in self.edgelist[u]
  
  def compute_reverse_edgelist(self):
    '''
    This function compute the pagerank algorithm based on the power iteration method.
      Inputs:
          - n: number of nodes.
          - m: number of edges.
          - edgelist: a dict with keys the nodes and values
                      list of neighbors.
          - nodes: list of nodes.
        Add this attribute:
        - reverse_edgelist: a dict with keys the nodes and values
                      list of nodes that lead to the node.
    '''
    self.reverse_edgelist = {}
    for u in self.nodes:
      if not u in self.reverse_edgelist.keys():
        self.reverse_edgelist[u] = []
      if not u in self.edgelist.keys():
        continue
      for v in self.edgelist[u]:
        if not (v in self.reverse_edgelist.keys()):
          self.reverse_edgelist[v] = []
        self.reverse_edgelist[v].append(u)

  def get_end_nodes(self):
    '''
      This function compute the pagerank algorithm based on the power iteration method.
      Inputs:
          - edgelist: a dict with keys the nodes and values
                      list of neighbors.
          - nodes: list of nodes.
          - deg_out: dict contains the out degree of every node.
        Add this attribute:
        - end_nodes: a list of nodes that have an out degree equal to 0.
    '''
    self.end_nodes = []
    for u in self.nodes:
      if self.deg_out[u] == 0:
        self.end_nodes.append(u)
  def PageRank(self, alpha= 0.1, t= 100, epsilion= 1e-3, P0= None):
    '''
     This method compute the pagerank algorithm based on the power iteration 
     method.
     Inputs:
        - alpha: damping factor, between 0 and 1.
        - t: number of iterations to be done.
        - epsilion: the limit of convergence of the graph.
        - P0: node for the rooted pagerank.
              If not initialized the regular pagerank will be used. 
     Output:
      - P:  dict of Pageranks with size n (number of node). 
    '''
    P = {u:1/self.n for u in self.nodes}
    Pt = {u:0 for u in self.nodes}
    start_time = time.time()
    history = {}
    for i in range(t):
        print("iter :", i)
        iter_time = time.time()
        norm1 = np.sum([abs(P[u]) for u in self.end_nodes]) / self.n
        end_inter = np.sum([P[u] for u in self.end_nodes]) / self.n
        for v in tqdm(self.nodes):
            inter = end_inter + np.sum([P[u]/self.deg_out[u] for u in self.reverse_edgelist[v]])
            if P0 == None:
                Pt[v] = (1 - alpha) * inter + alpha /self.n
            elif v in P0.keys():
                Pt[v] = (1 - alpha) * inter + alpha * P0[v]
            else:
                Pt[v] = (1 - alpha) * inter
            norm1 += abs(Pt[v]) # compute the norm 1
        Pt = Normalize2P(Pt, P0, self.n, norm1)
        diff = np.sum([abs(Pt[u] - P[u]) for u in self.nodes])
        P = deepcopy(Pt)
        history[i] = {}
        history[i]["norm"] = norm1
        history[i]["diff"] = sqrt(diff)
        history[i]["time"] = time.time() - iter_time
        if sqrt(diff) < epsilion:
            break
    history["total_amount"] = time.time() - start_time
    return P, history