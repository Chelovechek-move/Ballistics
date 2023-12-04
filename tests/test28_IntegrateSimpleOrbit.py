from matplotlib import pyplot as plt

f = open('/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/test28Data.txt', 'r')
lines = f.read().split("/n")

pos1 = [float(el) for el in lines[0].split()]
pos2 = [float(el) for el in lines[1].split()]
pos3 = [float(el) for el in lines[2].split()]
pos4 = [float(el) for el in lines[3].split()]
pos5 = [float(el) for el in lines[4].split()]
pos6 = [float(el) for el in lines[5].split()]
pos7 = [float(el) for el in lines[6].split()]
pos8 = [float(el) for el in lines[7].split()]
pos9 = [float(el) for el in lines[8].split()]

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(pos1, pos2, pos3, label='Explicit Runge Kutta', color='green', s=3)
ax.scatter(pos4, pos5, pos6, label='Implicit Runge Kutta', color='red', s=3)
ax.scatter(pos7, pos8, pos9, label='Dormand Prince', color='blue', s=3)
ax.legend()
plt.show()