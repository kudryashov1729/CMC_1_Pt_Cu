import matplotlib.pyplot as plt

f = open(r"output.txt", "r")
line = f.readline()
while line != "(Длина, число отсчетов)\n":
    cur_pos = f.tell()
    line = f.readline()

line = f.readline()

x = []
y = []

while line:
    line = f.readline()
    if len(line.rsplit()) > 0:
        x.append(int(line.rsplit()[0]))
    if len(line.rsplit()) >= 1:
        y.append(int(line.rsplit()[1]))
f.close()

ax = plt.axes()
ax.grid()

plt.ylabel("Число отсчетов")
plt.xlabel("Длина цепочки")

ax.bar(x, y)
plt.show()