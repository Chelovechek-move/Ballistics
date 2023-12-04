import numpy as np
from matplotlib import pyplot as plt

f1 = open('/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/test29Data.txt', 'r')

pos1 = []
pos2 = []
pos3 = []

i = 0
for line in f1:
    oneLine = line.split()
    pos1.append(float(oneLine[0]))
    pos2.append(float(oneLine[1]))
    pos3.append(float(oneLine[2]))

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot(pos1, pos2, pos3, label='Explicit Runge Kutta', color='green')
ax.legend()
plt.show()
