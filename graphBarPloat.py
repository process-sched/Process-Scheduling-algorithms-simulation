import matplotlib.pyplot as plt
import numpy as np
data_file = open('data.csv')
dataList = []
for row in data_file:
    tempList=list(row[:-2].split(","))
    tempList = [float(i) for i in tempList]
    dataList.append(tempList)
data_file.close()
averageCT=[item[0] for item in dataList]
averageTAT=[item[1] for item in dataList]
averageWT=[item[2] for item in dataList]
averageRT=[item[3] for item in dataList]
header=['SJFN','SJFP','Pn','Pp','FCFS','RR 1s','RR 2s','RR 3s','MLFQ','MLQ']
print("",end="\t")
for i in header:
	print(i,end="\t")
print()
print("CT",end="\t")
for i in averageCT:
	print("{:.2f}".format(i),end="\t")
print()
print("TAT",end="\t")
for i in averageTAT:
	print("{:.2f}".format(i),end="\t")
print()
print("WT",end="\t")
for i in averageWT:
	print("{:.2f}".format(i),end="\t")
print()
print("RT",end="\t")
for i in averageRT:
	print("{:.2f}".format(i),end="\t")
print()
plt.xlabel('Scheduling Algorithms')
plt.ylabel('Average Time Taken')
plt.title("Avg. CT")
plt.bar(header,averageCT,color='grey',width = 0.4)
plt.savefig("graph/averageCT.jpeg",dpi=1000)
plt.clf()
plt.xlabel('Scheduling Algorithms')
plt.ylabel('Average Time Taken')
plt.title("Avg. TAT")
plt.bar(header,averageTAT,color='grey',width = 0.4)
plt.savefig("graph/averageTAT.jpeg",dpi=1000)
plt.clf()
plt.xlabel('Scheduling Algorithms')
plt.ylabel('Average Time Taken')
plt.title("Avg. WT")
plt.bar(header,averageWT,color='grey',width = 0.4)
plt.savefig("graph/averageWT.jpeg",dpi=1000)
plt.clf()
plt.xlabel('Scheduling Algorithms')
plt.ylabel('Average Time Taken')
plt.title("Avg. RT")
plt.bar(header,averageRT,color='grey',width = 0.4)
plt.savefig("graph/averageRT.jpeg",dpi=1000)
plt.clf()

###############################################
r1 = np.arange(len(header))
barWidth = 0.18
r2 = [x + barWidth for x in r1]
r3 = [x + barWidth for x in r2]
r4 = [x + barWidth for x in r3]
plt.bar(r1, averageCT, width=barWidth, edgecolor='white', label='Avg. CT')
plt.bar(r2, averageTAT, width=barWidth, edgecolor='white', label='Avg. TAT')
plt.bar(r3, averageWT, width=barWidth, edgecolor='white', label='Avg. WT')
plt.bar(r4, averageRT, width=barWidth, edgecolor='white', label='Avg. RT')

plt.xlabel('Scheduling Algorithms', fontweight='bold')
plt.ylabel('Average Time Taken', fontweight='bold')
plt.xticks([r + barWidth for r in range(len(header))], header)
plt.legend()
plt.savefig("graph/multiBar.jpeg",dpi=1000)
