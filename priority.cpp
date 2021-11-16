// priority process scheduling
#include <bits/stdc++.h>
using namespace std;

struct Process {
  int id;
  string name;
  string type;
  int arrival_time;
  int burst_time;
  int priority;
  int waiting_time;
  int turnaround_time;
  int completion_time;
  int response_time;
};
map<int, Process> process_map;
struct pr_cmp {
  bool operator()(Process const &a, Process const &b) {
    return a.priority < b.priority; // max heap
  }
};
struct at_cmp {
  bool operator()(Process const &a, Process const &b) {
    return a.arrival_time > b.arrival_time; // min heap
  }
};

float *priority_sched() {
string input_file="process.csv";
string output_file="data.csv";
  /*
  if (argc != 2) {
    cout << "Usage: ./a.out <input file>" << endl;
    return 0;
  }
  */

  // int n;
  // cout << "Enter number of processes: ";
  /*
  cin >> n;
  priority_queue<Process, vector<Process>, at_cmp> job_queue;
  for (int i = 0; i < n; i++) {
    Process p;
    cin >> p.id >> p.arrival_time >> p.burst_time >> p.priority;
    job_queue.push(p);
    process_map[p.id] = p;
  }
  */
  // ifstream in(argv[1]);
  ifstream in(input_file);
  string line;
  int n = 0;
  priority_queue<Process, vector<Process>, at_cmp> job_queue;
  while (getline(in, line)) {
    stringstream ss(line);
    string word;
    Process p;
    getline(ss, word, ',');
    p.id = stoi(word);
    getline(ss, word, ',');
    p.name = word;
    getline(ss, word, ',');
    p.type = word;
    getline(ss, word, ',');
    p.priority = stoi(word);
    getline(ss, word, ',');
    p.arrival_time = stoi(word);
    getline(ss, word, ',');
    p.burst_time = stoi(word);
    job_queue.push(p);
    process_map[p.id] = p;
    n++;
  }
  /*
  auto job_queue_copy = job_queue;
  while(!job_queue_copy.empty()){
      Process p = job_queue_copy.top();
      job_queue_copy.pop();
      cout << "Process " << p.id << ": " << p.priority << " " << p.arrival_time
  << " " << p.burst_time << endl;
  }
  */

  priority_queue<Process, vector<Process>, pr_cmp> ready_queue;
  auto curr_proc = job_queue.top();
  job_queue.pop();
  int time = curr_proc.arrival_time;
  ready_queue.push(curr_proc);
  while (!ready_queue.empty() || !job_queue.empty()) {
    /*
    cout << "Current ready Q\n";
    auto ready_queue_copy = ready_queue;
    while (!ready_queue_copy.empty()) {
      auto p = ready_queue_copy.top();
      ready_queue_copy.pop();
      cout << "Process " << p.id << ": " << p.priority << " " << p.arrival_time
           << " " << p.burst_time << endl;
    }
    cout << "----------------------------\n";
    */

    // cout << "Time: " << time << endl;
    curr_proc = ready_queue.top();
    ready_queue.pop();
    /*    cout << "Curr Proc" << curr_proc.id << ": " << curr_proc.priority << "
       "
             << curr_proc.arrival_time << " " << curr_proc.burst_time << endl;
     */
    time += curr_proc.burst_time;

    curr_proc.completion_time = time;
    curr_proc.turnaround_time =
        curr_proc.completion_time - curr_proc.arrival_time;
    curr_proc.waiting_time = curr_proc.turnaround_time - curr_proc.burst_time;
    curr_proc.response_time = curr_proc.waiting_time; // non pre emptive
    process_map[curr_proc.id] = curr_proc;

    while (!job_queue.empty() && job_queue.top().arrival_time <= time) {
      ready_queue.push(job_queue.top());
      job_queue.pop();
    }
    // no process available
    if (ready_queue.empty() && !job_queue.empty()) {
      curr_proc = job_queue.top();
      job_queue.pop();
      time += curr_proc.arrival_time;
      ready_queue.push(curr_proc);
    }
  }
  // END OF ALGORITHM
  int total_waiting_time = 0;
  int total_turnaround_time = 0;

  float completionTime[n];
  float TAT[n];
  float waitingTime[n];
  float responseTime[n];
  // print process map

  //cout << "pid\tname\ttype\tpririty\tAT\tBT\tCT\tTAT\tWT\tRT\n";
  for (auto it = process_map.begin(); it != process_map.end(); it++) {
    /*cout << it->second.id << "\t" << it->second.name << "\t" << it->second.type
         << "\t" << it->second.priority << "\t" << it->second.arrival_time
         << "\t" << it->second.burst_time << "\t" << it->second.completion_time
         << "\t" << it->second.turnaround_time << "\t"
         << it->second.waiting_time << "\t" << it->second.response_time << endl;*/
    total_waiting_time += it->second.waiting_time;
    total_turnaround_time += it->second.turnaround_time;
    //to plot
    completionTime[it->second.id-1] = it->second.completion_time;
    TAT[it->second.id-1] = it->second.turnaround_time;
    waitingTime[it->second.id-1] = it->second.waiting_time;
    responseTime[it->second.id-1] = it->second.response_time;
  }
  /*cout << "TAT: " << total_turnaround_time << endl;
  cout << "TWT: " << total_waiting_time << endl;

  cout << "Average TAT: " << total_turnaround_time / float(n) << endl;
  cout << "Average WT: " << total_waiting_time / float(n) << endl;*/

  ofstream out(output_file, ios_base::app);
  copy(completionTime, completionTime + n, ostream_iterator<float>(out, ","));
  out << endl;
  copy(TAT, TAT + n, ostream_iterator<float>(out, ","));
  out << endl;
  copy(waitingTime, waitingTime + n, ostream_iterator<float>(out, ","));
  out << endl;
  copy(responseTime, responseTime + n, ostream_iterator<float>(out, ","));
  out << endl;
  out.close();
  system("python3 graphPloat.py priority.jpeg Priority");
  remove(output_file.c_str());
  float *result = (float *)malloc(sizeof(float) * 4);
  for (int i = 0; i < 4; i++) {
    result[i] = 0;
  }
  for (int i = 0; i < n; i++) {
    result[0] += completionTime[i];
    result[1] += TAT[i];
    result[2] += waitingTime[i];
    result[3] += responseTime[i];
  }
  for (int i = 0; i < 4; i++) {
    result[i] /= n;
  }
  return result;
  return 0;
}

