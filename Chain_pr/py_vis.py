import matplotlib.pyplot as plt
import numpy as np

f = open(r"exp=100_tay=1_T1=130_T2=80_time=1000.txt", "r")
line = f.readline()
while line != "(Длина, число отсчетов)\n":
    cur_pos = f.tell()
    line = f.readline()

line = f.readline()
line = f.readline()

x = []
y = []

line = f.readline()

while line != "-1\n":
    if len(line.rsplit()) > 0:
        x.append(int(line.rsplit()[0]))
    if len(line.rsplit()) == 2:
        y.append(int(line.rsplit()[1]))
    line = f.readline()


text = []
while line:
    line = f.readlines()
    text.append(line)

f.close()



box_1 = {'facecolor':'white',    #  цвет области
       'edgecolor': 'black',     #  цвет крайней линии
       'boxstyle': 'round'}    #  стиль области
s = ''.join(map(str, text[0]))


y = np.array(y, dtype=float)
y /= np.sum(y)

a = int(max(x))


ax = plt.axes()

ax.bar(x, y)
ax.grid()

b, t = plt.ylim()
plt.xlim([0,a])

ax.text(a, t, s,
        bbox = box_1,
        horizontalalignment = 'right', # выравнивание
        verticalalignment = 'top',
        color = 'black',
        fontsize = 10)
plt.ylabel("Число отсчетов")
plt.xlabel("Длина цепочки")


plt.show()
