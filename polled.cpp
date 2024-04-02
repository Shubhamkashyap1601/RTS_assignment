#include <bits/stdc++.h>
using namespace std;

long long MAX_TIME = 500;
ofstream outputFile("polled.csv");

struct Task {
    int id;
    int period;
    int execution;
    int remaining_time;
    int deadline;
    int release_time;
    int isAperiodic = false;
    bool operator<(const Task& other) const {
        if (release_time != other.release_time)
            return release_time < other.release_time; // Sort by release time first
        else if(period == other.period){
            return execution>other.execution;
        }
        else
            return period < other.period; // If release times are the same, sort by period
    }
};

struct AP_Task {
    int id;
    int release_time;
    int execution;

    bool operator<(const AP_Task& other) const {
        if (release_time != other.release_time)
            return release_time < other.release_time; // Sort by release time first
        return id < other.id; // if release times are the same, sort by id
    }
};

bool compare(AP_Task a, AP_Task b){
    return a.release_time < b.release_time;
}


long long calcHP(vector<Task>& task){
    long long temp = 1;
    for(int i=0;i<task.size();i++){
        temp = (temp*task[i].period)/__gcd(temp,(long long)task[i].period);
    }
    outputFile << "Hyperperiod: " << temp << "\n";
    return temp;
}

// Function to read task information from a CSV file
vector<Task> ReadTaskInformation(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open " << filename << endl;
        exit(1);
    }

    vector<Task> tasks;
    string line;
    getline(file, line); // Skip the header line

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Task task;
        getline(ss, token, ',');
        task.id = stoi(token);
        getline(ss, token, ',');
        task.period = stoi(token);
        getline(ss, token, ',');
        task.execution = stoi(token);
        getline(ss, token, ',');
        task.deadline = stoi(token);
        task.remaining_time = task.execution;
        task.release_time = 0;
        tasks.push_back(task);
    }
    file.close();

    return tasks;
}

vector<AP_Task> ReadAPTaskInformation(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open " << filename << endl;
        exit(1);
    }
    vector<AP_Task> tasks;
    string line;
    getline(file, line); // Skip the header line
    while(getline(file, line)){
        stringstream ss(line);
        string token;
        AP_Task task;
        getline(ss, token, ',');
        task.id = stoi(token);
        getline(ss, token, ',');
        task.release_time = stoi(token);
        getline(ss, token, ',');
        task.execution = stoi(token);
        tasks.push_back(task);
    }
    file.close();
    return tasks;
}

bool isSchedulable(const vector<Task>& tasks) {
    double utilization = 0;
    for (const auto& task : tasks) {
        utilization += (double)task.execution / (double)task.period;
    }
    return utilization <= 1;
}

void ScheduleTasks(vector<Task>& order, vector<AP_Task>& aperiodic_tasks, long long frame, int budget, int AP_period) {
  int startFrame = frame;
  cout << "Scheduling tasks...\n";
  priority_queue<Task> pq; // Priority queue for periodic tasks
  priority_queue<AP_Task> apq; // Priority queue for aperiodic tasks
  int n = order.size();
  int time = 0;

  outputFile << "----------- SCHEDULING NOW -----------\n";
  outputFile << "Time\t\tTask\n";
  int i = 0;
  int ai = 0;
  int serverBudget = 0; // Budget for the current server period

  while (time <= MAX_TIME) {
    // Add upcoming tasks (periodic & aperiodic) to their respective queues
    while (startFrame >= order[i].release_time && time <= MAX_TIME && i < n) {
      pq.push(order[i]);
      i++;
    }
    while (time == aperiodic_tasks[ai].release_time && time <= MAX_TIME && ai < aperiodic_tasks.size()) {
      apq.push(aperiodic_tasks[ai]);
      ai++;
    }

    // Check for server period and update budget
    if (time % AP_period == 0) {
      serverBudget = aperiodic_tasks.size() > 0 ? budget : 0; // Reset budget based on aperiodic task queue
    }

    // Process tasks until budget is exhausted or no tasks available
    int timeLeft = startFrame - time; // Remaining time within the frame
    while (!pq.empty() || !apq.empty()) {
      // Priority: Aperiodic tasks first (if any & budget allows)
      if (!apq.empty() && serverBudget > 0) {
        AP_Task currentTask = apq.top();
        apq.pop();

        // Check if task execution fits within budget, deadline, and frame
        if (currentTask.execution <= serverBudget && currentTask.release_time + currentTask.execution <= time + frame && currentTask.execution <= timeLeft) {
          outputFile << time << "\t\tAP-" << currentTask.id << "\n";
          serverBudget -= currentTask.execution;
          timeLeft -= currentTask.execution;
        }
      } else {
        // Process periodic task from the queue
        Task currentTask = pq.top();
        pq.pop();

        // Handle periodic task execution
        if (timeLeft > 0) {
          currentTask.remaining_time = min(currentTask.remaining_time, timeLeft); // Limit execution within frame
          outputFile << time << "\t\t" << currentTask.id << "\n";
          timeLeft -= currentTask.remaining_time;
          if (currentTask.remaining_time > 0) {
            pq.push(currentTask); // Push back remaining periodic task if needed
          }
        }
      }

      // Stop processing if budget is exhausted or frame time limit is reached
      if (serverBudget <= 0 || timeLeft <= 0) {
        break;
      }
    }

    // Handle no tasks available
    if (pq.empty() && apq.empty()) {
      outputFile << "No task available at time " << time << "\n";
    }

    time++;
  }

  outputFile << "----------- SCHEDULING ENDS -----------\n";
}


float util(vector<Task> tasks){
    double utilization = 0;
    for (const auto& task : tasks) {
        utilization += (double)task.execution / (double)task.period;
    }
    cout << "Utilization: " << utilization << endl;
    return utilization;
}

vector<long long> findFactors(long long num) {
    cout<<("num: \n",num);
    vector<long long> factors;
    for (long long i = 1; i <= sqrt(num); ++i) {
        if (num % i == 0) {
            factors.push_back(i);
            if (num / i != i) {
                factors.push_back(num / i);
            }
        }
    }
    return factors;
}

int maxExecution(vector<Task> tasks) {
    int maxEx = 0;
    for (const auto& task : tasks) {
        maxEx = max(maxEx, task.execution);
    }
    return maxEx;
}

bool checkCondition(vector<Task> tasks, long long frame) {
    for(auto i:tasks){
        if(frame<i.execution){
            return false;
        }
        if(2*frame - __gcd((long long)i.period,frame) < i.deadline){
            return false;
        }
    }
    return true;
}

long long findFrame(vector<Task> tasks, vector<long long>& factors) {

    int maxEx = maxExecution(tasks);
    long long frame = -1;
    for(auto i:factors){
        if(checkCondition(tasks,i)){
            frame = i;
            break;
        }
    }
    cout<<"\nFrame: "<<frame<<"\n";
    return frame;
}

string intToRoman(int num) {
    if(num==0){
        return "@";
    }
    vector<pair<int, string>> roman = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };

    string result;
    for (auto& value : roman) {
        while (num >= value.first) {
            num -= value.first;
            result += value.second;
        }
    }
    return result;
}


int main() {
    vector<Task> tasks = ReadTaskInformation("tasks.csv");
    if (!isSchedulable(tasks)) {
        cout << "Tasks are not schedulable using Rate Monotonic Scheduling." << endl;
        return 1;
    }
    vector<AP_Task> aperiodic_tasks = ReadAPTaskInformation("AP_tasks.csv");
    sort(aperiodic_tasks.begin(), aperiodic_tasks.end());
    vector<Task> order;
    // MAX_TIME = calcHP(tasks);
    for(auto t:tasks) {
        for(int i=0;i<=MAX_TIME;i+=t.period) {
            t.release_time = i;
            order.push_back(t);
        }
    }

    float  u = util(tasks);
    int p = max(50,tasks[0].period-1);
    int execution = p*(1-u);//getting excecution budget
    cout<<"util: "<<u<<" period : "<<p<<" execution: "<<execution<<"\n";
    int c = 0;
    for(int i=0;i<=MAX_TIME;i+=p){
        Task temp;

        temp.id = c++;
        temp.period = p;
        temp.execution = execution;
        temp.deadline = p;
        temp.remaining_time = execution;
        temp.release_time = i;
        temp.isAperiodic = true;
        order.push_back(temp);
    }

    sort(order.begin(), order.end());

    

    long long HP = calcHP(tasks);
    vector<long long> factors =  findFactors(HP);
    sort(factors.begin(),factors.end());
   
    long long frame = findFrame(tasks,factors);
    if(frame == -1){
        cout<<"No frame found\n";
        cout<<"taking frame as HP\n";
        frame = HP;
    }
     
    ScheduleTasks(order,aperiodic_tasks,frame,execution,p);
    cout << "Scheduled saved in polled.csv\n";
    return 0;
}
