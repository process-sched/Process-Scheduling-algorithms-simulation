#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

class Process4{
	int pid;
	int burst_time;
	int arrival_time;
	int Process4_completion_time;
	bool completed;
	string pname;
	int priority;
	bool started;
	int response_time;
	int total_burst_time;
public:
	Process4(){
		arrival_time = 0;
		burst_time = 0;
		completed = true;
		pid = 0;
		pname = "";
		priority = 0;
		started = false;
		response_time = -1;
		total_burst_time = 0;
	}

	Process4(int id, string name, int at, int bt, int p){
		pid = id;
		arrival_time = at;
		burst_time = bt;
		completed = false;
		pname = name;
		priority = p;
		started = false;
		response_time = -1;
		total_burst_time = bt;
	}

	int run(int start_time){
		int end_time = start_time + burst_time;
		burst_time = 0;
		Process4_completion_time = end_time;
		response_time = start_time;
		started = true;
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
			Process4_completion_time = end_time;
			burst_time = 0;
			completed = true;
		}
		if(!started){
			started = true;
			response_time = start_time;
		}
		return end_time;
	}

	string get_pname(){
		return pname;
	}

	int get_priority(){
		return priority;
	}

	int get_response_time(){
		return response_time - arrival_time;
	}

	bool is_completed(){
		return completed;
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

	int get_Process4_completion_time(){
		return Process4_completion_time;
	}

	int get_total_burst_time(){
		return total_burst_time;
	}

};

class arrivalTimeComparator1
{
   public:
   bool operator() (Process4* p1, Process4* p2) const
   { 
   	return p1->get_arrival_time() > p2->get_arrival_time();
   }

};

class burstTimeComparator1
{
   public:
   bool operator() (Process4* p1, Process4* p2) const
   { 
   	return p1->get_burst_time() > p2->get_burst_time();
   }

};

class RoundRobinScheduler{
	int time_quantum;
	priority_queue<Process4*, vector<Process4*>, arrivalTimeComparator1> new_Process4es;
	queue<Process4*> arrived_Process4es;
	queue<Process4*> completed_Process4es;
	int current_time;
public:
	RoundRobinScheduler(int start_time = 0, int tq=2){
		current_time = start_time;
		time_quantum = tq;
	}

	void addProcess4(Process4 *p){
		if(p->get_arrival_time() <= current_time)
			arrived_Process4es.push(p);
		else
			new_Process4es.push(p);
	}

	int run_single_time_quantum(){
		Process4* current_Process4 = NULL;
		if(!arrived_Process4es.empty()){
			current_Process4 = arrived_Process4es.front();
			current_time = current_Process4->run(current_time, time_quantum);
			arrived_Process4es.pop();
		}
		else{
			if(new_Process4es.empty()){
				current_time = current_time + time_quantum;
				return current_time;
			}
			else{	
				Process4*p = new_Process4es.top();
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
		while(!new_Process4es.empty() && new_Process4es.top()->get_arrival_time()<=current_time){
			Process4* p =new_Process4es.top();
			new_Process4es.pop();
			arrived_Process4es.push(p);
		}
		if(current_Process4 != NULL){
			if(current_Process4->is_completed()){
				completed_Process4es.push(current_Process4);
			}
			else{
				arrived_Process4es.push(current_Process4);	
			}
		}
		return current_time;
	}

	int get_next_scheduled_Process4_time(){
		if(!new_Process4es.empty()){
			if(!arrived_Process4es.empty()){
				return current_time;
			}
			else{
				Process4 *p = new_Process4es.top();
				return p->get_arrival_time();
			}
		}
		else{
			return -1;
		}
	}

	void update_current_time(int ct){
		current_time = ct;
		while(!new_Process4es.empty() && new_Process4es.top()->get_arrival_time()<=current_time){
			Process4* p =new_Process4es.top();
			new_Process4es.pop();
			arrived_Process4es.push(p);
		}
	}

	bool is_waiting(){
		return arrived_Process4es.empty();
	}

	bool is_idle(){
		return (arrived_Process4es.empty() && new_Process4es.empty());
	}

	queue<Process4*> get_completed_Process4es(){
		return completed_Process4es;
	}

	queue<Process4*> get_arrived_Process4es(){
		return arrived_Process4es;
	}

	void view_state(){
		queue<Process4*> temp = arrived_Process4es;
		while(!temp.empty()){
			//cout << "(" << to_string(temp.front()->get_pid()) << "," << to_string(temp.front()->get_burst_time()) << ") ";
			temp.pop();
		}
		//cout << endl;
	}
};

class MultilevelQueueScheduler{
	vector<RoundRobinScheduler*> levels;
	int n_levels;
	int current_time;
public:
	MultilevelQueueScheduler(int start_time=0, int ql=3, int tq=2){
		current_time = start_time;
		n_levels = ql;
		for(int i=0;i<ql;i++)
			levels.push_back(new RoundRobinScheduler(current_time, tq));
	}

	void addProcess4(Process4* p, int priority){
		levels[priority]->addProcess4(p);
	}

	int run(){
		bool run = false;
		for(int i=0;i<n_levels;i++){
			if(!levels[i]->is_waiting()){
				current_time = levels[i]->run_single_time_quantum();
				run = true;
				break;
			}
		}
		if(!run){
			int min_time = INT_MAX;
			int null_levels = 0;
			for(int i=0;i<n_levels;i++){
				int cur_level_next_time = levels[i]->get_next_scheduled_Process4_time();
				if(cur_level_next_time == -1)
					null_levels++;
				else
					min_time = min(min_time, cur_level_next_time);
			}
			if(null_levels == n_levels){
				current_time = levels[0]->run_single_time_quantum();
			}
			else{
				current_time = min_time;
			}
		}
		for(int i=0;i<n_levels;i++)
			levels[i]->update_current_time(current_time);
		return current_time;
	}

	bool is_idle(){
		bool result = true;
		for(int i=0;i<n_levels;i++){
			result = (result&levels[i]->is_idle());
		}
		return result;
	}

	vector<RoundRobinScheduler*> get_level_schedulers(){
		return levels;
	}

	void view_state(){
		for(int i=0;i<n_levels;i++){
			//cout << i << " : ";
			queue<Process4*> temp = levels[i]->get_arrived_Process4es();
			while(!temp.empty()){
				//cout << "(" << to_string(temp.front()->get_pid()) << "," << to_string(temp.front()->get_burst_time()) << ") ";
				temp.pop();
			}
			//cout << endl;
		}
	}
};

float* compute_statistics(queue<Process4*> completed_Process4es, int n, string outfile){
	int total_completion = 0;
	int total_turnaround = 0;
	int total_waiting = 0;
	int total_response = 0;

	// while(!completed_Process4es.empty()){
	// 	Process4* temp = completed_Process4es.front();
	// 	int ct = temp->get_Process4_completion_time();
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
	// 	completed_Process4es.pop();
	// }

	int no_of_Process4es = n;
	float *completionTime = new float[no_of_Process4es];
	float *TAT = new float[no_of_Process4es];
	float *waitingTime = new float[no_of_Process4es];
	float *responseTime = new float[no_of_Process4es];
	while(!completed_Process4es.empty()){
		Process4* temp = completed_Process4es.front();
		int ct = temp->get_Process4_completion_time();
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
		completed_Process4es.pop();
	}

	/*for(int i=0;i<no_of_Process4es;i++)
		cout << completionTime[i] << " ";
	cout << endl;

	for(int i=0;i<no_of_Process4es;i++)
		cout << TAT[i] << " ";
	cout << endl;

	for(int i=0;i<no_of_Process4es;i++)
		cout << waitingTime[i] << " ";
	cout << endl;

	for(int i=0;i<no_of_Process4es;i++)
		cout << responseTime[i] << " ";
	cout << endl;*/

	ofstream out(outfile.c_str(), ios_base::app);
	copy(completionTime, completionTime + no_of_Process4es, ostream_iterator<float>(out, ","));
	out << endl;
	copy(TAT, TAT + no_of_Process4es, ostream_iterator<float>(out, ","));
	out << endl;
	copy(waitingTime, waitingTime + no_of_Process4es, ostream_iterator<float>(out, ","));
	out << endl;
	copy(responseTime, responseTime + no_of_Process4es, ostream_iterator<float>(out, ","));
	out << endl;
	out.close();
	system("python3 graphPloat.py multilevel.jpeg MLQ");
	system("python3 graphStackPlot.py multilevel_Stack.jpeg MLQ");
	remove("data.csv");

	float *result = (float *)malloc(sizeof(float) * 4);
	//vector<float> result(5);
	for (int i = 0; i < 4; i++) {
	  result[i] = 0;
	}

	for (int i = 0; i < no_of_Process4es; i++) {
	  result[0] += completionTime[i];
	  result[1] += TAT[i];
	  result[2] += waitingTime[i];
	  result[3] += responseTime[i];
	}

	for (int i = 0; i < 4; i++) {
	  result[i] /= no_of_Process4es;
	}

	/*for (int i = 0; i < 4; i++) {
	  cout << result[i] << " ";
	}*/
	return result;
}

float *multilevel(string input_file, int tq, int ql){

	ifstream in(input_file);
	string line;
	string output_file="data.csv";
	int n = 0;
	MultilevelQueueScheduler ms(0,ql,tq);
	vector<vector<int>> input;
	while (getline(in, line)) {
	   stringstream ss(line);
	   string word;
		Process4 p;
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
			Process4 *p = new Process4(pid, pname, at, bt, priority%3);
			ms.addProcess4(p, priority%3);
		}
		n++;
	}

	while(!ms.is_idle()){
		ms.run();
		//ms.view_state();
		//cout << endl;
	}

	//cout << "PID\tNAME\tTYPE\tPRIO\tAT\tBT\tCT\tTAT\tWT\tRT\n";
	vector<RoundRobinScheduler*> schedulers = ms.get_level_schedulers();
	queue<Process4*> all_Process4;
	for(int i=0;i<schedulers.size();i++){
		RoundRobinScheduler* rs = schedulers[i];
		queue<Process4*> temp = rs->get_completed_Process4es();
		while(!temp.empty()){
			all_Process4.push(temp.front());
			temp.pop();
		}
	}

	return compute_statistics(all_Process4, n, "data.csv");
}
