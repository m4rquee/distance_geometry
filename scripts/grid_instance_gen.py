import sys
import random
from math import sqrt, log2

random.seed(42)
n, m_extra = sys.argv[1:]
n = int(n)
n_points = n ** 3

if m_extra.isdigit():
    m_extra = int(m_extra)
elif m_extra == 'zero':
    m_extra = 0
elif m_extra == 'line':
    m_extra = n_points
elif m_extra == 'full':
    m_extra = (n_points - 1) * (n_points - 2) / 2
elif m_extra == 'sqrt':
    m_extra = sqrt(n_points)
elif m_extra == 'log2':
    m_extra = log2(n_points)
elif m_extra == 'nlog2':
    m_extra = n_points * log2(n_points)
elif m_extra == 'nsqrt':
    m_extra = n_points ** (3 / 2)
else:
    raise ValueError('Invalid number of extra edges!')
m = int(m_extra + n_points - 1)
m = min(m, (n - 1) * n // 2)
print(n_points, m)


def distance(u, v):
    N = n
    u_x, u = u // (N * N), u % (N * N)
    u_y, u_z = u // N, u % N

    v_x, v = v // (N * N), v % (N * N)
    v_y, v_z = v // N, v % N

    return (u_x - v_x) ** 2 + (u_y - v_y) ** 2 + (u_z - v_z) ** 2


edges = set((i + 1, i + 2, distance(i, i + 1)) for i in range(n_points - 1))  # make sure the graph is connected
while len(edges) < m:
    u, v = sorted(random.sample(range(n_points), 2))
    d_sqr = distance(u, v)
    e = (u + 1, v + 1, d_sqr)
    if e not in edges:
        edges.add(e)

edges = sorted(list(edges))
for u, v, d in edges:
    print(u, v, d)
