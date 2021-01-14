import networkx as nx 
import random
from scipy.stats import truncnorm
import sys

'''
command should be like:
either
python3 testcase_geenrator.py
or
python3 testcase_geenrator.py [w_mean] [w_sd] [s_mean] [s_sd] [d_mean] [d_sd] [dependency_mean] [dependency_sd]
e.g. python3 testcase_geenrator.py 32 20 4 2 48 20 0.5 0.2

w_mean = [0, 64] 
s_mean = [1, 8]
d_mean = [1, 96]
dep_mean = [0, 1]
'''

def random_dag(nodes, edges):
    G = nx.DiGraph()
    for i in range(nodes):
        G.add_node(i)
    while edges > 0:
        a = random.randint(0,nodes-1)
        b=a
        while b==a:
            b = random.randint(0,nodes-1)
        G.add_edge(a,b)
        if nx.is_directed_acyclic_graph(G):
            edges -= 1
        else:
            # we closed a loop!
            G.remove_edge(a,b)
    return G

def get_truncated_normal(mean, sd, min, max):
    return truncnorm((min - mean) / sd, (max - mean) / sd, loc=mean, scale=sd)

# assign the l and n
l = random.randint(1, 8)
n = random.randint(1, 30)
#print("l = {}, n = {}".format(l, n))

# set the parameter
if (len(sys.argv) == 1):
    w_mean = 32
    w_sd = 20
    s_mean = int(l/2)
    s_sd = 2
    d_mean = 48
    d_sd = 20
    dependency_mean = 0.5
    dependency_sd = 0.2

else:
    parametrs = sys.argv[1:]
    [w_mean, w_sd, s_mean, s_sd, d_mean, d_sd, dependency_mean, dependency_sd] = [float(i) for i in parametrs]
    
    if (s_mean > l):
        s_mean = int(s_mean * l / s_mean)


# assign the w
w_var = get_truncated_normal(w_mean, w_sd, 0, 64)
w = []
for i in range(n):
    w.append(w_var.rvs())

#print("w = ", w)

# assign the m (# of operations for each job)
dividers = sorted(random.sample(range(1, 100), n-1))
m = [a - b for a, b in zip(dividers + [100], [0] + dividers)]

#print("m = ", m)

# assign the s and d
s_var = get_truncated_normal(s_mean, s_sd, 1, l)
d_var = get_truncated_normal(d_mean, d_sd, 1, 96)
s, d = [], []
for i in range(100):
    s.append(int(s_var.rvs()))
    d.append(int(d_var.rvs()))

# assign the depedency (p)
p = []
a = []
dep_var = get_truncated_normal(dependency_mean, dependency_sd, 0, 1)
for i in range(n):
    DAG = random_dag(m[i], int((m[i]**2)*dep_var.rvs()))
    a_ij = [[] for j in range(m[i])] # for job i
    p_ij = []
    
    for node in DAG.nodes(): # ascending order
        for edge in DAG.in_edges(node):
            a_ij[node].append(edge[0])
    
    for j in range(m[i]):
        a_ij[j] = list(set(a_ij[j]))
        p_ij.append(len(a_ij[j]))

    a.append(a_ij)
    p.append(p_ij)


# output
counter = 0
f = open("00.in", "w")
f.write(str(l)+'\n')
f.write(str(n)+'\n')
for i in range(n):
    f.write(str(m[i])+'\n')
    f.write('{0:.6f}'.format(w[i])+'\n')
    for j in range(m[i]):
        f.write("{} {} {}".format(s[counter], d[counter], p[i][j]))
        for k in range(p[i][j]):
            f.write(" {}".format(a[i][j][k]+1))
        f.write('\n')
        counter += 1
f.close()

print("Complete generating private test case\n")