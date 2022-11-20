import sys, os
import numpy as np
import plotly.offline
from PyQt5.QtCore import QUrl
import plotly.graph_objects as go
from PyQt5.QtWidgets import QApplication
from PyQt5.QtWebEngineWidgets import QWebEngineView


def show_in_window(_fig):
    plotly.offline.plot(_fig, filename='molecule_plot.html', auto_open=False)
    app = QApplication(sys.argv)
    web = QWebEngineView()
    file_path = os.path.abspath(os.path.join(os.path.dirname(__file__), 'molecule_plot.html'))
    web.load(QUrl.fromLocalFile(file_path))
    web.resize(800, 800)
    web.show()
    sys.exit(app.exec_())


n, m = map(int, input().split())
coordinates = np.fromiter(map(float, input().split()), dtype=float).reshape((n, 3))
edge_list = np.fromiter(map(int, input().split()), dtype=int).reshape((m, 2))

# Lists with the starting and ending coordinates of each edge:
x_edges, y_edges, z_edges = [], [], []
for edge in edge_list:
    beging, end = coordinates[edge[0]], coordinates[edge[1]]
    x_edges += [beging[0], end[0], None]
    y_edges += [beging[1], end[1], None]
    z_edges += [beging[2], end[2], None]

axis = dict(showbackground=False, showline=False, zeroline=True, showgrid=False, showticklabels=False, title='')
layout = go.Layout(title='Molecule 3D Structure',
                   width=750, height=750, showlegend=False,
                   scene=dict(xaxis=dict(axis),
                              yaxis=dict(axis),
                              zaxis=dict(axis)),
                   margin=dict(t=100))
fig = go.Figure(layout=layout)

fig.add_scatter3d(x=x_edges, y=y_edges, z=z_edges,
                  mode='lines', line=dict(color='black', width=2))
fig.add_scatter3d(x=coordinates[:, 0], y=coordinates[:, 1], z=coordinates[:, 2],
                  mode='markers', marker=dict(symbol='circle', size=3, color='green'))

if 'win' in sys.argv:
    show_in_window(fig)  # show in a separete windown
else:
    fig.show()  # show in browser
