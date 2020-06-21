import pandas as pd
import math
import matplotlib.pyplot as plt
import ellipse as el
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Ellipse

# X1, X2 = el.make_test_ellipse()

df = pd.read_csv("outputData.txt", sep=" ")
df.columns = ["x", "y", "z"]

X = np.array(list(zip(df["x"], df["y"])))
reg = el.LsqEllipse().fit(X)
center, width, height, phi = reg.as_parameters()

plt.close('all')
fig = plt.figure(figsize=(6,6))
ax = fig.add_subplot(111)
ax.axis('equal')
ax.plot(df["x"], df["y"], 'ro', label='test data', zorder=1)

ellipse = Ellipse(xy=center, width=2*width, height=2*height, angle=np.rad2deg(phi),
               edgecolor='b', fc='None', lw=2, label='Fit', zorder = 2)
ax.add_patch(ellipse)
plt.legend()
plt.show()

print(phi * 180 / math.pi)
circ = df.copy()
phi = phi
for i,row in circ.iterrows():
    circ.iloc[i,0] = row["x"].copy() * math.cos(phi) - row["y"].copy() * math.sin(phi)
    circ.iloc[i,1] = row["y"].copy() * math.cos(phi) + row["x"].copy() * math.sin(phi)

for i,row in circ.iterrows():
    circ.iloc[i,0] = row["x"].copy() * width / height

X = np.array(list(zip(circ["x"], circ["y"])))
reg = el.LsqEllipse().fit(X)
center, width, height, phi = reg.as_parameters()

plt.close('all')
fig = plt.figure(figsize=(6,6))
ax = fig.add_subplot(111)
ax.axis('equal')
ax.plot(circ["x"], circ["y"], 'ro', label='test data', zorder=1)

ellipse = Ellipse(xy=center, width=2*width, height=2*height, angle=np.rad2deg(phi),
                  edgecolor='b', fc='None', lw=2, label='Fit', zorder = 2)
ax.add_patch(ellipse)
plt.legend()
plt.show()
