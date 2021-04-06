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

i = 0
for file in files:
    f = open(file, 'r')
    line = f.readline()
    while line != '(Длина, число отсчетов)\n':
        line = f.readline()
    line = f.readline()
    while line != '-1\n':
        line = f.readline()
        if len(line.rsplit()) == 2:
            if(int(line.rsplit()[1]) != 0):
                df.loc[i, line.rsplit()[0]] = int(line.rsplit()[1])
    text = []
    while True:
        line = f.readline()
        if not line:
            break
        text.append(line)
    # df.loc[i, 'text'] = ''.join(text)
    i += 1
    f.close()

print (df)

gr = df.iloc[:, 6:50]
gr = gr.transpose()

gr.replace(np.nan, 0, inplace=True)
for c in gr.columns:
    gr[c] /= gr[c].sum()
    
print(gr)

figure, ax = plt.subplots()
for index in range(0, 6, 1):
    ax.plot(gr[index], label=df.iloc[index, [1]].to_string())

ax.set_title(df.iloc[0, [2,3,4]].to_string().replace('    ', '=').replace('\n', '; '))
ax.legend(bbox_to_anchor=(1,1))
ax.grid()

plt.xticks(np.arange(0, 40, 2))
plt.yticks(np.arange(0, 0.09, 0.005))
plt.xlim(0,40)
plt.ylim(0, 0.09)
plt.xlabel('Длина цепочки')
plt.ylabel('Частота')

plt.show()

#MaxNLocator
#