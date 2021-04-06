import os
import pandas as pd
import csv
'''
    настраемывый запуск chain_pr.exe
    с известным временем выполнения одного эксперемента (файл csv) 
'''
f = open(r"D:\CMC\run_time.txt")

myData = [['exp', 'tay', 'T1', 'T2', 'time', 'prog_time']]
while True:
    line = f.readline()
    if not line:
        break
    raw = line.rsplit(sep='\t')
    raw[0] = raw[0].replace("D:\\5 семестр\\projects\\project_test2\\res\\", '')
    raw[0] = raw[0].replace(".txt", '')
    raw[0] = raw[0].rsplit(sep='_')
    data = []
    for l in raw[0]:
        data.append(int(l.rsplit(sep='=')[1]))
    data.append(float(raw[2].replace('ms', '')))
    myData.append(data)
lines = f.readlines()
f.close()

myFile = open(r"D:\CMC\prog_time.csv", 'w')
with myFile:
    writer = csv.writer(myFile)
    writer.writerows(myData)

df = pd.DataFrame(myData)
df.columns = df.iloc[0, :]
df = df.loc[1::]
df.reset_index()


i = 0

for prog_time in df.prog_time:
    chain_call = 'chain_pr.exe ' + str(500) + ' ' + str(df.iloc[i, 1]) + ' ' + str(df.iloc[i, 2]) \
    + ' ' + str(df.iloc[i, 3]) + ' ' + str(df.iloc[i, 4])
    i += 1
    os.system(chain_call)
    print(i)


# for line in lines:
    # if line != lines[0]:
        # line = line.replace('\n', '')
        # line = line.rsplit(sep=',')
        # exp = 180000 / float(line[6])
        # if exp > 500:
            # continue
        # else:
            # exp = 500
        # tay = line[2]
        # T1 = line[3]
        # T2 = line[4]
        # time = line[5]
        # chain_call = 'chain_pr.exe ' + str(exp) + ' ' + tay + ' ' + T1 + ' ' + T2 + ' ' + time
        # print(chain_call)
        ## os.system(chain_call)
        # print(i)
        # i += 1




