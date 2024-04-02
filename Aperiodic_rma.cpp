#include <bits/stdc++.h>
using namespace std;

long long MAX_TIME = 500;
ofstream outputFile("schedule_rma_AP.csv");

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


    sort(order.begin(), order.end());
     
    ScheduleTasks(order,aperiodic_tasks);
    cout << "Scheduled saved in schedule_rma.csv\n";
    return 0;
}
