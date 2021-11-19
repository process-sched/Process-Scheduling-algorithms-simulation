#include<bits/stdc++.h>
#include<vector>
#include <Python.h>
#include<cstdlib>
#include <string> 
#include <algorithm>
#include <iterator>
#include "SJFN.cpp"
#include "SJFP.cpp"
#include "priority.cpp"
int main()
{
	
	float *SJFNData=SJFN();
	float *SJFPData=SJFP();
	float *priority_sched_data=priority_sched();
	ofstream out( "data.csv",ios_base::app );
    copy( SJFNData,SJFNData+4, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( SJFPData,SJFPData+4, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( priority_sched_data,priority_sched_data+4, ostream_iterator<float>( out, "," ) );
    out<<endl;
    out.close();
    system("python3 graphBarPloat.py");
	remove("data.csv");
	return 0;
}
