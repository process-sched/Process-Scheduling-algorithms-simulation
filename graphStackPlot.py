import matplotlib.pyplot as plt
import numpy as np
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
y1 = np.array(completionTime)
y2 = np.array(TAT)
y3 = np.array(waitingTime)
y4 = np.array(responseTime)
plt.bar(process, y1, color='r')
plt.bar(process, y2, bottom=y1, color='b')
plt.bar(process, y3, bottom=y1+y2, color='y')
plt.bar(process, y4, bottom=y1+y2+y3, color='g')
plt.title(sys.argv[2])
plt.xlabel('Process')
plt.ylabel('Time Taken')
plt.legend(['CT','TAT','WT','RT'])
plt.grid(True)
plt.savefig("graph/"+sys.argv[1] ,dpi=1000)
#plt.show()
