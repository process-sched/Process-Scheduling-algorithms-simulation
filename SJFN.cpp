#include<bits/stdc++.h>
#include<vector>
#include <Python.h>
#include<cstdlib>
#include <string> 
#include <algorithm>
#include <iterator>
using namespace std;
struct process
{
	int pid;
	float arrivalTime;
	float burstTime;
	float priority;
	string name;
	string cpuio;
};
float convert(string word)
{
	stringstream geek(word);
	float temp;	
	geek>>temp;
	return temp;
}
vector<process> inputFromCSV(string filename)
{
	ifstream inputFile(filename);
	string temp,word;
	vector<process> heapVector;
	process processHold;
	while (inputFile >> temp) 
	{
        stringstream s(temp);
        getline(s,word,',');
        processHold.pid=convert(word);
        getline(s,word,',');
        processHold.name=word;
        getline(s,word,',');
        processHold.cpuio=word;
        getline(s,word,',');
        processHold.priority=convert(word);
        getline(s,word,',');
        processHold.arrivalTime=convert(word);
        getline(s,word,',');
        processHold.burstTime=convert(word);
        heapVector.push_back(processHold);
  	}
  	inputFile.close();
  	return heapVector;
}
bool compare(process p1, process p2)
{
    if(p1.arrivalTime==p2.arrivalTime&&p2.burstTime==p2.burstTime)
    {
    	return p1.priority>p2.priority;
	}
	if(p1.arrivalTime==p2.arrivalTime)
	{
		return p1.burstTime<p2.burstTime;
	}
	return p1.arrivalTime<p2.arrivalTime;
}
struct comparitor
{
    bool operator()(process p1,process p2) const
    {
       return (p1.burstTime>p2.burstTime);
    }
};
float *SJFN(string filename)
{
	vector<process> heapVector=inputFromCSV(filename);
	sort(heapVector.begin(),heapVector.end(),compare);
	vector<process>:: iterator itr;
	float completionTime[heapVector.size()];
	float TAT[heapVector.size()];
	float waitingTime[heapVector.size()];
	float responseTime[heapVector.size()]; 
	float complete=0;
	int len=heapVector.size();
	for(itr=heapVector.begin();itr!=heapVector.end();itr++)
	{
		if(itr->cpuio=="I")
		{
			completionTime[itr->pid]=itr->arrivalTime+itr->burstTime;
			responseTime[itr->pid]=0;
			waitingTime[itr->pid]=0;
			TAT[itr->pid]=itr->burstTime;
			heapVector.erase(itr);
		}
	}
	priority_queue<process, vector<process>, comparitor> minHeap;
	minHeap.push(heapVector[0]);
	heapVector.erase(heapVector.begin());
	while(heapVector.size()>0||minHeap.size()>0)
	{
		process temp=minHeap.top();
		minHeap.pop();
		if(complete>=temp.arrivalTime)
		{
			responseTime[temp.pid]=complete-temp.arrivalTime;
			complete+=temp.burstTime;
			completionTime[temp.pid]=complete;
			TAT[temp.pid]=complete-temp.arrivalTime;
			waitingTime[temp.pid]=TAT[temp.pid]-temp.burstTime;
		}
		else
		{
			responseTime[temp.pid]=0;
			complete=temp.arrivalTime;
			completionTime[temp.pid]=complete+temp.burstTime;
			complete+=temp.burstTime;
			TAT[temp.pid]=complete-temp.arrivalTime;
			waitingTime[temp.pid]=TAT[temp.pid]-temp.burstTime;
		}
		vector<process>:: iterator itr=heapVector.begin();
		while(itr!=heapVector.end())
		{
			if(complete>=itr->arrivalTime)
			{
				minHeap.push(*itr);
				heapVector.erase(heapVector.begin());
			}
			else
			{
				break;
			}
		}
		if(heapVector.size()>0&&minHeap.size()==0)
		{
			minHeap.push(heapVector[0]);
			heapVector.erase(heapVector.begin());
		}
	}
	ofstream out( "data.csv",ios_base::app );
    copy( completionTime,completionTime+len, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( TAT,TAT+len, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( waitingTime,waitingTime+len, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( responseTime,responseTime+len, ostream_iterator<float>( out, "," ) );
    out<<endl;
    out.close();
	system("python3 graphPloat.py SJFN.jpeg SJFN");
	system("python3 graphStackPlot.py SJFN_Stack.jpeg SJFN");
	remove("data.csv");
	float *result=(float *)malloc(sizeof(float)*4);
	for(int i=0;i<4;i++)
	{
		result[i]=0;
	}
	for(int i=0;i<len;i++)
	{
		result[0]+=completionTime[i];
		result[1]+=TAT[i];
		result[2]+=waitingTime[i];
		result[3]+=responseTime[i];
	}
	for(int i=0;i<4;i++)
	{
		result[i]/=len;
	}
  	return result;
}
