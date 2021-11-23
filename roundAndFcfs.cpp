#include <bits/stdc++.h>

using namespace std;

class Process2 {
    private: 
        int     p_id;
        string  p_name;
        string  p_type;
        int     arrival_time;
        int     burst_time;
        int     waiting_time;
        int     turnaround_time;
        int     completion_time;
        int     response_time;
        bool    scheduled;
        int     original_burst_time;

    public:
        
        Process2() { }

        Process2(int p_id, string p_name, string p_type, int arrival_time, int burst_time) {
            this->p_id          = p_id;
            this->p_name        = p_name;
            this->p_type        = p_type;
            this->arrival_time  = arrival_time;
            this->burst_time    = burst_time;
            this->original_burst_time    = burst_time;
            this->scheduled     = 0;
        }

        int get_p_id() {
            return p_id;
        }

        string get_p_name() {
            return p_name;
        }

        string get_p_type() {
            return p_type;
        }

        int get_arrival_time() {
            return arrival_time;
        }

        int get_burst_time() {
            return burst_time;
        }

        int get_original_burst_time() {
            return this->original_burst_time;
        }

        int get_waiting_time() {
            return waiting_time;
        }

        int get_turnaround_time() {
            return turnaround_time;
        }

        int get_completion_time() {
            return completion_time;
        }

        int get_response_time() {
            return response_time;
        }

        bool is_sched() {
            return this->scheduled;
        }

        void set_waiting_time(int waiting_time) {
            this->waiting_time = waiting_time;
        }

        void set_turnaround_time(int turnaround_time) {
            this->turnaround_time = turnaround_time;
        }

        void set_completion_time(int completion_time) {
            this->completion_time = completion_time;
        }

        void set_response_time(int response_time) {
            this->response_time = response_time;
        }
        
        void set_burst_time(int burst_time) {
            this->burst_time = burst_time;
        }

        void set_original_burst_time(int original_burst_time) {
            this->original_burst_time = original_burst_time;
        }

        void set_sched(bool val) {
            this->scheduled = val;
        }

};


class Arrival_Compare {
    public:
        bool operator() (Process2 p1, Process2 p2) {
            if (p1.get_arrival_time() == p2.get_arrival_time())
                return p1.get_p_id() > p2.get_p_id();

            return p1.get_arrival_time() > p2.get_arrival_time();
        }
};

vector<Process2> Process2_array;

float* fcfs(string input_file) {
    string output_file  = "data.csv";
    priority_queue<Process2, vector<Process2>, Arrival_Compare> queue;
    ifstream in(input_file);
    string line;
    int no_of_Process2es = 0;

    while (getline(in, line)) {
        stringstream ss(line);
        string word;

        getline(ss, word, ',');
        int p_id = stoi(word);

        getline(ss, word, ',');
        string p_name = word;

        getline(ss, word, ',');
        string p_type = word;

        getline(ss, word, ',');
        string priority = word;
        
        getline(ss, word, ',');
        int arrival_time = stoi(word);

        getline(ss, word, ',');
        int burst_time = stoi(word);

        Process2 current_Process2(p_id, p_name, p_type, arrival_time, burst_time);
        queue.push(current_Process2);
        
        no_of_Process2es++;
    }


    
    int current_time = 0;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;

    float completionTime[no_of_Process2es];
    float TAT[no_of_Process2es];
    float waitingTime[no_of_Process2es];
    float responseTime[no_of_Process2es];
    

    while (!queue.empty()) {
        Process2 current_proc = queue.top();
        queue.pop();

        if (current_proc.get_arrival_time() > current_time) {
            current_time = current_proc.get_arrival_time();
        }

        current_proc.set_response_time(current_time - current_proc.get_arrival_time());
        responseTime[current_proc.get_p_id()] = current_proc.get_response_time(); 
        current_time += current_proc.get_burst_time();
        current_proc.set_completion_time(current_time);
        completionTime[current_proc.get_p_id()] = current_proc.get_completion_time();
        current_proc.set_turnaround_time(current_proc.get_completion_time() - current_proc.get_arrival_time());
        TAT[current_proc.get_p_id()] = current_proc.get_turnaround_time();
        current_proc.set_waiting_time(current_proc.get_turnaround_time() - current_proc.get_burst_time());
        waitingTime[current_proc.get_p_id()] = current_proc.get_waiting_time();
        

        total_turnaround_time += current_proc.get_turnaround_time();
        total_waiting_time += current_proc.get_waiting_time();

        Process2_array.push_back(current_proc);
    }

    ofstream out(output_file, ios_base::app);
    copy(completionTime, completionTime + no_of_Process2es, ostream_iterator<float>(out, ","));
    out << endl;
    copy(TAT, TAT + no_of_Process2es, ostream_iterator<float>(out, ","));
    out << endl;
    copy(waitingTime, waitingTime + no_of_Process2es, ostream_iterator<float>(out, ","));
    out << endl;
    copy(responseTime, responseTime + no_of_Process2es, ostream_iterator<float>(out, ","));
    out << endl;
    out.close();
    system("python3 graphPloat.py FCFS.jpeg FCFS");
    system("python3 graphStackPlot.py FCFS_Stack.jpeg FCFS");
    remove(output_file.c_str());


    float *result = (float *)malloc(sizeof(float) * 4);
    for (int i = 0; i < 4; i++) {
        result[i] = 0;
    }

    for (int i = 0; i < no_of_Process2es; i++) {
        result[0] += completionTime[i];
        result[1] += TAT[i];
        result[2] += waitingTime[i];
        result[3] += responseTime[i];
    }

    for (int i = 0; i < 4; i++) {
        result[i] /= no_of_Process2es;
    }

    return result;
}


float* roundrobin(string input_file, int time_quantun) {
	string output_file  = "data.csv";
    deque<Process2> rr_queue;
    priority_queue<Process2, vector<Process2>, Arrival_Compare> queue;
    double avg_turnaround_time;
    double avg_waiting_time;

    ifstream in(input_file);
    string line;
    int no_of_Process2es = 0;

    while (getline(in, line)) {
        stringstream ss(line);
        string word;

        getline(ss, word, ',');
        int p_id = stoi(word);

        getline(ss, word, ',');
        string p_name = word;

        getline(ss, word, ',');
        string p_type = word;

        getline(ss, word, ',');
        string priority = word;

        getline(ss, word, ',');
        int arrival_time = stoi(word);

        getline(ss, word, ',');
        int burst_time = stoi(word);

        Process2 current_Process2(p_id, p_name, p_type, arrival_time, burst_time);
        queue.push(current_Process2);
        
        no_of_Process2es++;
    }

    int current_time = 0;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;

    float completionTime[no_of_Process2es];
    float TAT[no_of_Process2es];
    float waitingTime[no_of_Process2es];
    float responseTime[no_of_Process2es];

    do {

        if (rr_queue.empty()) {
            Process2 cur = queue.top();
            queue.pop();
            rr_queue.push_back(cur);
            current_time = cur.get_arrival_time();
            if (!cur.is_sched()) {
                cur.set_sched(1);
                cur.set_response_time(current_time);
                responseTime[cur.get_p_id()] = current_time;
            }

        } else {
            Process2 cur = rr_queue.front();
            rr_queue.pop_front();

            if (!cur.is_sched()) {
                cur.set_sched(1);
                cur.set_response_time(current_time);
                responseTime[cur.get_p_id()] = current_time;
            }

            if (cur.get_burst_time() >= time_quantun) {
                cur.set_burst_time(cur.get_burst_time() - time_quantun);
                current_time += time_quantun;
                
                while (!queue.empty()) {
                    Process2 temp = queue.top();
                    if (temp.get_arrival_time() <= current_time) {
                        rr_queue.push_back(temp);
                        queue.pop();

                    } else {
                        break;
                    }
                }

                if (cur.get_burst_time() > 0) {
                    rr_queue.push_back(cur);
                } 
            }
             
            else {
                current_time += cur.get_burst_time();
                cur.set_burst_time(0);
                
                while (!queue.empty()) {
                    Process2 temp = queue.top();
                    if (temp.get_arrival_time() <= current_time) {
                        rr_queue.push_back(temp);
                        queue.pop();

                    } else {
                        break;
                    }
                }
            }

            if (cur.get_burst_time() == 0) {
                cur.set_completion_time(current_time);
                completionTime[cur.get_p_id()] = cur.get_completion_time();
                
                cur.set_turnaround_time(cur.get_completion_time() - cur.get_arrival_time());
                TAT[cur.get_p_id()] = cur.get_turnaround_time();

                cur.set_waiting_time(cur.get_turnaround_time() - cur.get_original_burst_time());
                waitingTime[cur.get_p_id()] = cur.get_waiting_time();
            }


        }



    } while (!rr_queue.empty());
    ofstream out(output_file, ios_base::app);
    copy(completionTime, completionTime + no_of_Process2es, ostream_iterator<float>(out, ","));
    out << endl;
    copy(TAT, TAT + no_of_Process2es, ostream_iterator<float>(out, ","));
    out << endl;
    copy(waitingTime, waitingTime + no_of_Process2es, ostream_iterator<float>(out, ","));
    out << endl;
    copy(responseTime, responseTime + no_of_Process2es, ostream_iterator<float>(out, ","));
    out << endl;
    out.close();
	string s="python3 graphPloat.py RR_"+to_string(time_quantun)+"s.jpeg RR_"+to_string(time_quantun);
    system(s.c_str());
    s="python3 graphStackPlot.py RR_"+to_string(time_quantun)+"s_Stack.jpeg RR_"+to_string(time_quantun);
    system(s.c_str());
    remove(output_file.c_str());
    float *result = (float *)malloc(sizeof(float) * 4);
    for (int i = 0; i < 4; i++) {
        result[i] = 0;
    }

    for (int i = 0; i < no_of_Process2es; i++) {
        result[0] += completionTime[i];
        result[1] += TAT[i];
        result[2] += waitingTime[i];
        result[3] += responseTime[i];
    }

    for (int i = 0; i < 4; i++) {
        result[i] /= no_of_Process2es;
    }

    return result;

}
