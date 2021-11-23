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
#include "roundAndFcfs.cpp"
int main(int argc, char **argv)
{
	if (argc != 2) 
	{
	    cout << "Usage: ./a.out <input file>" << endl;
	    return 0;
  	}
	float *SJFNData=SJFN(argv[1]);
	float *SJFPData=SJFP(argv[1]);
	float *priority_sched_data=priority_np(argv[1]);
	float *priority_preem_data = priority_p(argv[1]);
	float *fcfs_data=fcfs(argv[1]);
	float *roundrobin_data_1=roundrobin(argv[1],1);
	float *roundrobin_data_2=roundrobin(argv[1],2);
	float *roundrobin_data_3=roundrobin(argv[1],3);
	ofstream out( "data.csv",ios_base::app );
    copy( SJFNData,SJFNData+4, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( SJFPData,SJFPData+4, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( priority_sched_data,priority_sched_data+4, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( priority_preem_data,priority_preem_data+4, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( fcfs_data,fcfs_data+4, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( roundrobin_data_1,roundrobin_data_1+4, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( roundrobin_data_2,roundrobin_data_2+4, ostream_iterator<float>( out, "," ) );
    out<<endl;
    copy( roundrobin_data_3,roundrobin_data_3+4, ostream_iterator<float>( out, "," ) );
    out<<endl;
    out.close();
    system("python3 graphBarPloat.py");
	remove("data.csv");
	return 0;
}
