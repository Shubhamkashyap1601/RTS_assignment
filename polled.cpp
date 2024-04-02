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

void ScheduleTasks(vector<Task>& order, vector<AP_Task>& aperiodic_tasks) {
    cout << "Scheduling tasks...\n";
    priority_queue<Task> pq;
    priority_queue<AP_Task> apq;
    int n = order.size();
    int time = 0;
   
    outputFile << "----------- SCHEDULING NOW -----------\n";
    outputFile << "Time\t\tTask"<< "\n";
    int i = 0;
    int ai = 0;
    while(time<=MAX_TIME){
        while(time==order[i].release_time && time <=MAX_TIME && i < n){
            pq.push(order[i]);
            i++;
        }
        while(time==aperiodic_tasks[ai].release_time && time <=MAX_TIME && ai < aperiodic_tasks.size()){
            apq.push(aperiodic_tasks[ai]);
            ai++;
        }
        

        if (!pq.empty()) {
            Task temp = pq.top();
            pq.pop();
            outputFile << time << "\t\t\tT" << temp.id << "\n";
            temp.remaining_time--;

            if (temp.remaining_time > 0) {
                pq.push(temp);
            }
        } else {
            if(!apq.empty()){
                AP_Task temp = apq.top();
                apq.pop();
                outputFile << time << "\t\t\tAP" << temp.id << "\n";
                temp.execution--;
                if(temp.execution>0){
                    apq.push(temp);
                }

            }
            else{
                outputFile << "No task available at time " << time << "\n";
            }
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
    cout<<"in\n\n\n\n";
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
    cout<<"Frame: "<<frame<<"\n";
    return frame;
}


int main() {
    vector<Task> tasks = ReadTaskInformation("tasks.csv");
    if (!isSchedulable(tasks)) {
        cout << "Tasks are not schedulable using Rate Monotonic Scheduling." << endl;
        return 1;
    }
    vector<AP_Task> aperiodic_tasks = ReadAPTaskInformation("AP_tasks.csv");
    sort(aperiodic_tasks.begin(), aperiodic_tasks.end());
    for(auto t:aperiodic_tasks){
        cout << t.id << " " << t.release_time << " " << t.execution << "\n";
    }
    vector<Task> order;
    // MAX_TIME = calcHP(tasks);
    for(auto t:tasks) {
        for(int i=0;i<=MAX_TIME;i+=t.period) {
            t.release_time = i;
            order.push_back(t);
        }
    }

    float  u = util(tasks);
    int p = max(10,tasks[0].period-1);
    int execution = p*(1-u);
    cout<<"util: "<<u<<" period : "<<p<<" execution: "<<execution<<"\n";

    sort(order.begin(), order.end());
    long long HP = calcHP(tasks);
    vector<long long> factors =  findFactors(HP);
    sort(factors.begin(),factors.end());
    cout<<"Factors of HP: ";
    for(auto i:factors){
        cout<<i<<" ";
    }
    long long frame = findFrame(tasks,factors);
    if(frame == -1){
        cout<<"No frame found\n";
        cout<<"taking frame as HP\n";
        frame = HP;
    }
     
    ScheduleTasks(order,aperiodic_tasks);
    cout << "Scheduled saved in polled.csv\n";
    return 0;
}
