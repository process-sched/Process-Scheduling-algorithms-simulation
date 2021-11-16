import matplotlib.pyplot as plt
data_file = open('data.csv')
dataList = []
for row in data_file:
    tempList=list(row[:-2].split(","))
    dataList.append(tempList)
data_file.close()
completionTime=[float(i) for i in dataList[0]]
TAT=[float(i) for i in dataList[1]]
waitingTime=[float(i) for i in dataList[2]]
responseTime=[float(i) for i in dataList[3]]
process=[ i for i in range(0,len(dataList[0]))]
plt.plot(process, completionTime)
plt.plot(process, TAT)
plt.plot(process, waitingTime)
plt.plot(process, responseTime)
plt.title('SJF')
plt.xlabel('Process')
plt.ylabel('Time Taken')
plt.legend(['CT','TAT','WT','RT'])
plt.grid(True)
plt.savefig("SJFN.jpeg",dpi=1000)
plt.show()
