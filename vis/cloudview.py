import json
import sys
import random
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
f = open(sys.argv[1])
data = json.load(f)
xs = []
ys = []
zs = []
for d in data["data"]:
  if random.random() < 0.05:
    xs.append(d["a"])
    ys.append(d["p"])
    zs.append(d["d"])
ax.set_xlabel("child angle")
ax.set_ylabel("parent angle")
ax.set_zlabel("terminal distance")
ax.scatter(xs,ys,zs,marker="x")
plt.show()