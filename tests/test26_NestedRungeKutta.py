from matplotlib import pyplot as plt

f = open('/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/test26Data.txt', 'r')

x = []
y = []
for line in f:
    oneLine = line.split()
    x.append(float(oneLine[0]))
    y.append(float(oneLine[1]))

f, a = plt.subplots()
a.scatter(x, y)
a.set(xlabel='t', ylabel='u')
plt.show()
