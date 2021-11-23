import matplotlib.pyplot as plt
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
header=['SJFN','SJFP','Pn','Pp','FCFS','RR 1s','RR 2s','RR 3s']
plt.xlabel('Scheduling Algorithms')
plt.ylabel('Average Time Taken')
plt.title("Avg. CT")
plt.bar(header,averageCT,color='grey',width = 0.4)
plt.savefig("graph/averageCT.jpeg",dpi=1000)
plt.title("Avg. TAT")
plt.bar(header,averageTAT,color='grey',width = 0.4)
plt.savefig("graph/averageTAT.jpeg",dpi=1000)
plt.title("Avg. WT")
plt.bar(header,averageWT,color='grey',width = 0.4)
plt.savefig("graph/averageWT.jpeg",dpi=1000)
plt.title("Avg. RT")
plt.bar(header,averageRT,color='grey',width = 0.4)
plt.savefig("graph/averageRT.jpeg",dpi=1000)
