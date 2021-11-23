#include<bits/stdc++.h>
#include<vector>
#include <Python.h>
#include<cstdlib>
#include <string> 
#include <algorithm>
#include <iterator>
using namespace std;
struct processP
{
	int pid;
	float arrivalTime;
	float burstTime;
	float priority;
	string name;
	string cpuio;
};
float convertP(string word)
{
	stringstream geek(word);
	float temp;	
	geek>>temp;
	return temp;
}
vector<processP> inputFromCSVP(string filename)
{
	ifstream inputFile(filename);
	string temp,word;
	vector<processP> heapVector;
	processP processPHold;
	while (inputFile >> temp) 
	{
        stringstream s(temp);
        getline(s,word,',');
        processPHold.pid=convertP(word);
        getline(s,word,',');
        processPHold.name=word;
        getline(s,word,',');
        processPHold.cpuio=word;
        getline(s,word,',');
        processPHold.priority=convertP(word);
        getline(s,word,',');
        processPHold.arrivalTime=convertP(word);
        getline(s,word,',');
        processPHold.burstTime=convertP(word);
        heapVector.push_back(processPHold);
  	}
  	inputFile.close();
  	return heapVector;
}
bool compareP(processP p1, processP p2)
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
struct comparitorP
{
    bool operator()(processP p1,processP p2) const
    {
       return (p1.burstTime>p2.burstTime);
    }
};
float *SJFP(string filename)
{
	vector<processP> heapVector=inputFromCSVP(filename);
	sort(heapVector.begin(),heapVector.end(),compareP);
	float completionTime[heapVector.size()];
	float TAT[heapVector.size()];
	float waitingTime[heapVector.size()];
	float responseTime[heapVector.size()]; 
	float complete=0;
	int len=heapVector.size();
	float maxArrival=0;
	for(int i=0;i<len;i++)
	{
		waitingTime[i]=0;
	}
	vector<processP>:: iterator itr;
	for(itr=heapVector.begin();itr!=heapVector.end();itr++)
	{
		if(maxArrival<itr->arrivalTime)
		{
			maxArrival=itr->arrivalTime;
		}
		if(itr->cpuio=="I")
		{
			completionTime[itr->pid]=itr->arrivalTime+itr->burstTime;
			responseTime[itr->pid]=0;
			waitingTime[itr->pid]=0;
			TAT[itr->pid]=itr->burstTime;
			heapVector.erase(itr);
		}
	}
	vector<bool> responseIndicator(len,0);
	priority_queue<processP, vector<processP>, comparitorP> minHeap;
	int i;
	for(i=0;i<=maxArrival;i++)
	{
		itr=heapVector.begin();
		while(i>=itr->arrivalTime&&itr!=heapVector.end())
		{
			minHeap.push(*itr);
			heapVector.erase(heapVector.begin());
		}
		if(!minHeap.empty())
		{
			processP temp=minHeap.top();
			minHeap.pop();
			if(responseIndicator[temp.pid]==false)
			{
				responseIndicator[temp.pid]=true;
				responseTime[temp.pid]=i-temp.arrivalTime;
			}
			temp.burstTime--;
			waitingTime[temp.pid]--;
			if(temp.burstTime==0)
			{
				completionTime[temp.pid]=i+1;
				TAT[temp.pid]=completionTime[temp.pid]-temp.arrivalTime;
				waitingTime[temp.pid]+=TAT[temp.pid]-temp.burstTime;
			}
			else
			{
				minHeap.push(temp);
			}
		}
	}
	maxArrival=i;
	while(!minHeap.empty())
	{
		processP temp=minHeap.top();
		minHeap.pop();
		if(responseIndicator[temp.pid]==false)
		{
			responseIndicator[temp.pid]=true;
			responseTime[temp.pid]=maxArrival-temp.arrivalTime;
			completionTime[temp.pid]=maxArrival+temp.burstTime;
			TAT[temp.pid]=completionTime[temp.pid]-temp.arrivalTime;
			waitingTime[temp.pid]=TAT[temp.pid]-temp.burstTime;
			maxArrival=completionTime[temp.pid];
		}
		else
		{
			completionTime[temp.pid]=maxArrival+temp.burstTime;
			TAT[temp.pid]=completionTime[temp.pid]-temp.arrivalTime;
			waitingTime[temp.pid]+=TAT[temp.pid]-temp.burstTime;
			maxArrival=completionTime[temp.pid];
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
	system("python3 graphPloat.py SJFP.jpeg SJFP");
	system("python3 graphStackPlot.py SJFP_Stack.jpeg SJFP");
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
