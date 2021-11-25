import matplotlib.pyplot as plt
import sys
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
plt.scatter(process,completionTime,label="Completion Time")
plt.scatter(process,TAT,label="Turn Around Time")
plt.scatter(process,waitingTime,label="Waiting Time")
plt.scatter(process,responseTime,label="Response Time")

plt.plot(process, completionTime,'--')
plt.plot(process, TAT,'--')
plt.plot(process, waitingTime,'--')
plt.plot(process, responseTime,'--')

plt.title(sys.argv[2])
plt.xlabel('Process')
plt.ylabel('Time Taken')
plt.legend(['CT','TAT','WT','RT'])
plt.grid(True)
plt.savefig("graph/"+sys.argv[1] ,dpi=1000)
