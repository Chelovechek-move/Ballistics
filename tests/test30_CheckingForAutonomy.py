import numpy as np
from matplotlib import pyplot as plt

f = open('/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/test30Data.txt', 'r')

x = []
y = []
for line in f:
    oneLine = line.split()
    y.append(float(oneLine[0]))
    x.append(float(oneLine[1]))

y = np.log10(y[1:])

k = (y[-1] - y[0]) / (x[-1] - x[0])
print(f'k = {k}')

f, a = plt.subplots()
a.scatter(x[1:], y, label='Explicit Runge Kutta', color='green')
a.set(xlabel='t', ylabel='|ΔH| / |H|')
a.legend()
plt.show()