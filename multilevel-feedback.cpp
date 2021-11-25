#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

class Process5{
	int pid;
	int burst_time;
	int arrival_time;
	int Process5_completion_time;
	int total_burst_time;
	bool completed;
	string pname;
	int response_time;
	int priority;
	bool started;
public:
	Process5(){
		arrival_time = 0;
		burst_time = 0;
		completed = true;
		pid = 0;
		response_time = -1;
		started = false;
		priority = 0;
		pname = "";
	}

	Process5(int id, string name, int at, int bt, int p){
		pid = id;
		arrival_time = at;
		burst_time = bt;
		total_burst_time = bt;
		completed = false;
		pname = name;
		priority = p;
	}

	int run(int start_time){
		int end_time = start_time + burst_time;
		burst_time = 0;
		Process5_completion_time = end_time;
		started = true;
		response_time = start_time;
		completed = true;
		return end_time;
	}

	int run(int start_time, int time_quantum){
		int end_time;
		if(burst_time > time_quantum){
			end_time = start_time + time_quantum;
			burst_time = burst_time - time_quantum;
		}
		else{
			end_time = start_time + burst_time;
			Process5_completion_time = end_time;
			burst_time = 0;
			completed = true;
		}
		if(!started){
			started = true;
			response_time = start_time;
		}
		return end_time;
	}

	bool is_completed(){
		return completed;
	}

	string get_pname(){
		return pname;
	}

	int get_priority(){
		return priority;
	}

	int get_response_time(){
		return response_time;
	}

	int get_pid(){
		return pid;
	}

	int get_arrival_time(){
		return arrival_time;
	}

	int get_burst_time(){
		return burst_time;
	}

	int get_run_time(){
		return (total_burst_time-burst_time);
	}

	int get_total_burst_time(){
		return total_burst_time;
	}

	int get_Process5_completion_time(){
		return Process5_completion_time;
	}

};

class arrivalTimeComparator2
{
   public:
   bool operator() (Process5* p1, Process5* p2) const
   { 
   	return p1->get_arrival_time() > p2->get_arrival_time();
   }

};

class burstTimeComparator2
{
   public:
   bool operator() (Process5* p1, Process5* p2) const
   { 
   	return p1->get_burst_time() > p2->get_burst_time();
   }

};

class FsfcScheduler{
	int current_time;
	priority_queue<Process5*, vector<Process5*>, arrivalTimeComparator2> new_Process5es;
	queue<Process5*> arrived_Process5es;
	queue<Process5*> completed_Process5es;
public:
	FsfcScheduler(int start_time=0){
		current_time = start_time;
	}
	
	void addProcess5(Process5 *p){
		if(p->get_arrival_time() <= current_time)
			arrived_Process5es.push(p);
		else
			new_Process5es.push(p);
	}

	int run(){
		Process5* current_Process5 = NULL;
		if(!arrived_Process5es.empty()){
			current_Process5 = arrived_Process5es.front();
			current_time = current_Process5->run(current_time);
			arrived_Process5es.pop();
		}
		else{
			if(new_Process5es.empty()){
				return current_time;
			}
			else{	
				Process5*p = new_Process5es.top();
				int next_scheduled_time = p->get_arrival_time();
				current_time = next_scheduled_time;
			}
		}
		while(!new_Process5es.empty() && new_Process5es.top()->get_arrival_time()<=current_time){
			Process5* p =new_Process5es.top();
			new_Process5es.pop();
			arrived_Process5es.push(p);
		}
		if(current_Process5 != NULL && current_Process5->is_completed()){
			completed_Process5es.push(current_Process5);
		}
		return current_time;
	}

	int get_next_scheduled_Process5_time(){
		if(!new_Process5es.empty()){
			if(!arrived_Process5es.empty()){
				return current_time;
			}
			else{
				Process5 *p = new_Process5es.top();
				return p->get_arrival_time();
			}
		}
		else{
			return -1;
		}
	}

	void update_current_time(int ct){
		current_time = ct;
		while(!new_Process5es.empty() && new_Process5es.top()->get_arrival_time()<=current_time){
			Process5* p =new_Process5es.top();
			new_Process5es.pop();
			arrived_Process5es.push(p);
		}
	}

	bool is_waiting(){
		return arrived_Process5es.empty();
	}

	bool is_idle(){
		return (arrived_Process5es.empty() && new_Process5es.empty());
	}

	queue<Process5*> get_arrived_Process5es(){
		return arrived_Process5es;
	}

	queue<Process5*> get_completed_Process5es(){
		return completed_Process5es;
	}

	void view_state(){
		queue<Process5*> temp = arrived_Process5es;
		while(!temp.empty()){
			//cout << "(" << to_string(temp.front()->get_pid()) << "," << to_string(temp.front()->get_burst_time()) << ") ";
			temp.pop();
		}
		//cout << endl;
	}
};

class MultilevelFeedbackQueueScheduler{
	vector<queue<Process5*>> levels;
	FsfcScheduler* last_level;
	int n_levels;
	int current_time;
	int feedback_duration;
	int time_quantum;
	priority_queue<Process5*, vector<Process5*>, arrivalTimeComparator2> new_Process5es;
	queue<Process5*> completed_Process5es;
public:
	MultilevelFeedbackQueueScheduler(int start_time=0, int ql=3, int tq=2, int fd=2){
		current_time = start_time;
		feedback_duration = fd;
		time_quantum = tq;
		n_levels = ql;
		for(int i=0;i<ql-1;i++)
			levels.push_back(queue<Process5*>{});
		last_level = new FsfcScheduler(current_time);
	}

	void addProcess5(Process5* p){
		if(p->get_arrival_time() <= current_time)
			levels[0].push(p);
		else
			new_Process5es.push(p);
	}

	int run(){
		bool run = false;
		Process5* current_Process5 = NULL;
		int current_Process5_level = -1;
		for(int i=0;i<n_levels-1;i++){
			if(!levels[i].empty()){
				current_Process5 = levels[i].front();
				current_Process5_level = i;
				current_time = current_Process5->run(current_time, time_quantum);
				levels[i].pop();
				run = true;
				break;
			}
		}
		if(!run && !last_level->is_idle()){
			current_time = last_level->run();
			run = true;
		}
		if(!run){
			if(new_Process5es.empty()){
				return current_time;
			}
			else{	
				Process5 *p = new_Process5es.top();
				int next_scheduled_time = p->get_arrival_time();
				if(next_scheduled_time > current_time+time_quantum){
					current_time = current_time + time_quantum;
					return current_time;
				}
				else{
					current_time = next_scheduled_time;
				}
			}
		}
		while(!new_Process5es.empty() && new_Process5es.top()->get_arrival_time()<=current_time){
			Process5* p =new_Process5es.top();
			new_Process5es.pop();
			levels[0].push(p);
		}
		if(current_Process5 != NULL){
			if(current_Process5->is_completed()){
				completed_Process5es.push(current_Process5);
			}
			else{
				if(current_Process5->get_run_time()>= feedback_duration && !current_Process5->is_completed()){
					if(current_Process5_level < n_levels-2){
						levels[current_Process5_level+1].push(current_Process5);
					}
					else{
						last_level->addProcess5(current_Process5);
					}
				}
				else if(!current_Process5->is_completed()){
					levels[current_Process5_level].push(current_Process5);
				}
			}
		}
		last_level->update_current_time(current_time);
		return current_time;
	}

	bool is_idle(){
		bool result = true;
		for(int i=0;i<n_levels-1;i++){
			result = (result&levels[i].empty());
		}
		result = (result&last_level->is_idle());
		return (result&new_Process5es.empty());
	}

	bool is_waiting(){
		bool result = true;
		for(int i=0;i<n_levels-1;i++){
			result = (result&levels[i].empty());
		}
		result = (result&last_level->is_waiting());
		return result;
	}

	queue<Process5*> get_completed_Process5es(){
		queue<Process5*> fcfs_queue = last_level->get_completed_Process5es();
		queue<Process5*> levelq = completed_Process5es;
		queue<Process5*> temp;
		while(!levelq.empty()){
			temp.push(levelq.front());
			levelq.pop();
		}
		while(!fcfs_queue.empty()){
			temp.push(fcfs_queue.front());
			fcfs_queue.pop();
		}
		return temp;
	}

	void view_state(){
		for(int i=0;i<n_levels-1;i++){
			queue<Process5*> temp = levels[i];
			//cout << i << " : ";
			while(!temp.empty()){
				//cout << "(" << to_string(temp.front()->get_pid()) << "," << to_string(temp.front()->get_burst_time()) << ") ";
				temp.pop();
			}
			//cout << endl;
		}
		queue<Process5*> temp = last_level->get_arrived_Process5es();
		//cout << n_levels-1 << " : ";
		while(!temp.empty()){
				//cout << "(" << to_string(temp.front()->get_pid()) << "," << to_string(temp.front()->get_burst_time()) << ") ";
				temp.pop();
			}
		//cout << endl;
	}
};

float* compute_statistics(queue<Process5*> completed_Process5es, int n, string outfile){
	int total_completion = 0;
	int total_turnaround = 0;
	int total_waiting = 0;
	int total_response = 0;

	// while(!completed_Process5es.empty()){
	// 	Process5* temp = completed_Process5es.front();
	// 	int ct = temp->get_Process5_completion_time();
	// 	int at = temp->get_arrival_time();
	// 	int bt = temp->get_total_burst_time();
	// 	int prio = temp->get_priority();
	// 	int pid = temp->get_pid();
	// 	string pname = temp->get_pname();
	// 	int rt = temp->get_response_time();
	// 	string type = "C";
	// 	int tat = ct-at;
	// 	int wt = tat-bt;
	// 	total_completion += ct;
	// 	total_turnaround += tat;
	// 	total_waiting += wt;
	// 	total_response += rt;
	// 	cout << pid << "\t" << pname << "\t" << type << "\t" << prio << "\t" << at << "\t" << bt << "\t"
	// 	 << ct << "\t" << tat << "\t" << wt << "\t" << rt << endl;
	// 	completed_Process5es.pop();
	// }

	int no_of_Process5es = n;
	float *completionTime = new float[no_of_Process5es];
	float *TAT = new float[no_of_Process5es];
	float *waitingTime = new float[no_of_Process5es];
	float *responseTime = new float[no_of_Process5es];
	while(!completed_Process5es.empty()){
		Process5* temp = completed_Process5es.front();
		int ct = temp->get_Process5_completion_time();
		int at = temp->get_arrival_time();
		int bt = temp->get_total_burst_time();
		int prio = temp->get_priority();
		int pid = temp->get_pid();
		string pname = temp->get_pname();
		int rt = temp->get_response_time();
		string type = "C";
		int tat = ct-at;
		int wt = tat-bt;
		total_completion += ct;
		total_turnaround += tat;
		total_waiting += wt;
		total_response += rt;

		completionTime[temp->get_pid()] = ct;
		TAT[temp->get_pid()] = tat;
		waitingTime[temp->get_pid()] = wt;
		responseTime[temp->get_pid()] = rt;
		/*cout << pid << "\t" << pname << "\t" << type << "\t" << prio << "\t" << at << "\t" << bt << "\t"
		 << ct << "\t" << tat << "\t" << wt << "\t" << rt << endl;*/
		completed_Process5es.pop();
	}

	/*for(int i=0;i<no_of_Process5es;i++)
		cout << completionTime[i] << " ";
	cout << endl;

	for(int i=0;i<no_of_Process5es;i++)
		cout << TAT[i] << " ";
	cout << endl;

	for(int i=0;i<no_of_Process5es;i++)
		cout << waitingTime[i] << " ";
	cout << endl;

	for(int i=0;i<no_of_Process5es;i++)
		cout << responseTime[i] << " ";
	cout << endl;*/

	ofstream out(outfile.c_str(), ios_base::app);
	copy(completionTime, completionTime + no_of_Process5es, ostream_iterator<float>(out, ","));
	out << endl;
	copy(TAT, TAT + no_of_Process5es, ostream_iterator<float>(out, ","));
	out << endl;
	copy(waitingTime, waitingTime + no_of_Process5es, ostream_iterator<float>(out, ","));
	out << endl;
	copy(responseTime, responseTime + no_of_Process5es, ostream_iterator<float>(out, ","));
	out << endl;
	out.close();
	system("python3 graphPloat.py multilevel-feedback.jpeg MLFQ");
	system("python3 graphStackPlot.py multilevel-feedback_Stack.jpeg MLFQ");
	remove("data.csv");

	float *result = (float *)malloc(sizeof(float) * 4);
	//vector<float> result(5);
	for (int i = 0; i < 4; i++) {
	  result[i] = 0;
	}

	for (int i = 0; i < no_of_Process5es; i++) {
	  result[0] += completionTime[i];
	  result[1] += TAT[i];
	  result[2] += waitingTime[i];
	  result[3] += responseTime[i];
	}

	for (int i = 0; i < 4; i++) {
	  result[i] /= no_of_Process5es;
	}

	/*for (int i = 0; i < 4; i++) {
	  cout << result[i] << " ";
	}*/
	return result;
}

float *multilevel_feedback(string input_file , int tq, int ql, int fd){

	ifstream in(input_file);
	string line;
	string output_file="data.csv";
	int n = 0;
	vector<vector<int>> input;
	MultilevelFeedbackQueueScheduler mfs(0, ql, tq, fd);
	
	while (getline(in, line)) {
	   stringstream ss(line);
	   string word;
		Process5 p;
		getline(ss, word, ',');
		int pid = stoi(word);
		getline(ss, word, ',');
		string pname = word;
		getline(ss, word, ',');
		string type = word;
		getline(ss, word, ',');
		int priority = stoi(word);
		getline(ss, word, ',');
		int at = stoi(word);
		getline(ss, word, ',');
		int bt = stoi(word);
		if(type == "C"){
			input.push_back({pid, at, bt});
			Process5* p = new Process5(pid, pname, at, bt, priority);
			mfs.addProcess5(p);
		}
		n++;
	}


	while(!mfs.is_idle()){
		mfs.run();
		// mfs.view_state();
		// cout << endl;
	}

	//cout << "PID\tNAME\tTYPE\tPRIO\tAT\tBT\tCT\tTAT\tWT\tRT\n";
	queue<Process5*> temp = mfs.get_completed_Process5es();
	return compute_statistics(temp, n, "data.csv");
}
