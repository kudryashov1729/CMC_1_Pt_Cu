import matplotlib.pyplot as plt

f = open(r"output.txt", "r")
line = f.readline()
while line != "(Длина, число отсчетов)\n":
    cur_pos = f.tell()
    line = f.readline()

line = f.readline()
line = f.readline()

x = []
y = []

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
a = int(max(x))
b = int(max(y))
ax = plt.axes()
ax.grid()
plt.xlim([0, a])
plt.ylim([0, b])
ax.text(a, b, s,
        bbox = box_1,
        horizontalalignment = 'right', # выравнивание
        verticalalignment = 'top',
        color = 'black',
        fontsize = 10)
plt.ylabel("Число отсчетов")
plt.xlabel("Длинна цепочки")

ax.bar(x, y)
plt.show()
