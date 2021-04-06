import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os


def convert_to_df(file_path):
    col = {'exp': [], 'tay': [], 'T1': [], 'T2': [], 'time': [], 'prog_time': []}
    df = pd.DataFrame(col)

    f = open(file_path, "r")

    while True:
        line = f.readline()
        if not line:
            break
        raw = line.rsplit(sep='\t')
        raw[0] = raw[0].replace("D:\\CMC\\res\\", '')
        raw[0] = raw[0].replace(".txt", '')
        raw[0] = raw[0].rsplit(sep='_')
        data = []
        for l in raw[0]:
            data.append(int(l.rsplit(sep='=')[1]))
        data.append(float(raw[2].replace('ms', '')))
        df.loc[len(df)] = data
    f.close()
    return df


def get_list_of_files(file_path):
    f = open(file_path, 'r')
    files = []
    while True:
        line = f.readline()
        if not line:
            break
        files.append(line.rsplit(sep='\t')[0].replace('D:\\CMC\\res\\', ''))
    return files    
    
df = convert_to_df(r"D:\CMC\res\run_time.txt")
files = get_list_of_files(r"D:\CMC\res\run_time.txt")

i = 1
index = 0
for file in files:
    f = open(file, 'r')
    distrub = [0] * 70
    while True:
        line = f.readline()
        if not line:
            break
        if line == 'Длины цепочек:\n':
            line = f.readline()
            number = len(line.rsplit(sep='|'))
            if line.rsplit(sep='|')[-1] == ' \n':
                number -= 1
            distrub[number - 1] += 1
            if i < 0:
                print(line.replace('\n', ''))
                print(number)
                print(distrub)
                i += 1
        if line == '-1\n':
            text = []
            while True:
                line = f.readline()
                if not line:
                    break
                text.append(line)
    f.close()
    df.loc[index, range(1, 71)] = distrub
    df.loc[index, 'text'] = ''.join(text)
    index += 1

df = df.sort_values(['T2', 'time', 'tay'], ascending=[False, False, True]).reset_index()
print(df)
gr = df.iloc[:, 7: 77]
gr = gr.transpose()


for i in gr.columns:  # нормировка
    gr[i] /= gr[i].sum()
print(gr.to_string())

c = ['b', 'y', 'g', 'r', 'p', 'brown']

figure, ax = plt.subplots()
plt.xlim(25, 56)


for i in range(0, 6, 1):
    ax.plot(gr[i], label=df.iloc[i, [2]].to_string())  #, color=c[i]

    ax.set_title(df.iloc[0, [4, 5]].to_string().replace('    ', '=').replace('\n', '; '))
ax.legend()
ax.grid()
ax.set_xlim([25, 56])
figure.suptitle('Распределение количества цепочек', fontsize=14)
plt.show()