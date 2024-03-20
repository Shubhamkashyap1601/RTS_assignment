#include <bits/stdc++.h>
using namespace std;

long long MAX_TIME = 100;
ofstream outputFile("schedule.csv");

struct Task {
    int id;
    int period;
    int execution;
    int remaining_time;
    int deadline;
    int release_time;
    int abs_deadline;
    int slack_time;
};

long long calcHP(vector<Task>& task){
    long long temp = 1;
    for(int i=0;i<task.size();i++){
        temp = (temp*task[i].period)/__gcd(temp,(long long)task[i].period);
        // cout << "temp: " << temp << endl;
    }
    // cout << "Hyperperiod: " << temp << endl;
    outputFile << "Hyperperiod: " << temp << "\n";
    return temp;
}

bool compare_release(Task& task1, Task& task2){
    if (task1.release_time == task2.release_time)
        return task1.slack_time < task2.slack_time; // Sort by absolute deadline if release times are equal
    else
        return task1.release_time < task2.release_time; // Otherwise, sort by release time
}

struct CompareDeadlines {
    bool operator()(const Task& task1, const Task& task2) {
        if(task1.slack_time == task2.slack_time)
            return task1.release_time < task2.release_time;
        else
            return task1.slack_time > task2.slack_time; // Sort by absolute deadline in ascending order
    }
};


bool isSchedulable(const vector<Task>& tasks) {
    double utilization = 0;
    for (const auto& task : tasks) {
        utilization += (double)task.execution / (double)task.period;
    }
    return utilization <= 1;
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



void ScheduleTasks(vector<Task>& order) {
    cout << "Scheduling tasks...\n";
    priority_queue<Task, vector<Task>, CompareDeadlines> pq;
    int n = order.size();
    int time = 0;
    // for(auto it : order){
    //     cout << it.id << " ";
    // }
    // cout << "----------- SCHEDULING NOW -----------\n";
    outputFile << "----------- SCHEDULING NOW -----------\n";
    // cout << "Time\t\tTask"<<endl;
    outputFile << "Time\t\tTask"<< "\n";
    int i = 0;
    while(time<=MAX_TIME){
        while(time==order[i].release_time && time <=MAX_TIME && i < n){
            pq.push(order[i]);
            // cout<<"order: " << order[i].id<<"\t"<<time<<endl;
            i++;
        }
        

        if (!pq.empty()) {
            Task temp = pq.top();
            pq.pop();
            // cout << time << "\t\tT" << temp.id << endl;
            outputFile << time << "\t\t\tT" << temp.id << "\n";
            temp.remaining_time--;

            queue<Task> qtemp;
            while (!pq.empty()) {
                Task t = pq.top();
                pq.pop();
                t.slack_time--;
                qtemp.push(t);
            }
            if (temp.remaining_time > 0) {
                pq.push(temp);
            }
            while (!qtemp.empty()) {
                Task t = qtemp.front();
                qtemp.pop();
                pq.push(t);
            }
        } else {
            // cout << "No task available at time " << time << endl;
            outputFile << "No task available at time " << time << "\n";
        }

        time++;
    }
    // cout << "----------- SCHEDULING ENDS -----------\n";
    outputFile << "----------- SCHEDULING ENDS -----------\n";
}

int main() {
    vector<Task> tasks = ReadTaskInformation("tasks.csv");
    if (!isSchedulable(tasks)) {
        cout << "Tasks are not schedulable using Least Slack Time First Scheduling." << endl;
        return 1;
    }
    
    vector<Task> order;
    for(auto t:tasks) {
        for(int i=0;i<=MAX_TIME;i+=t.period) {
            t.abs_deadline = i + t.deadline;
            t.release_time = i;
            t.slack_time = t.abs_deadline - t.release_time - t.execution;
            order.push_back(t);
        }
    }
    MAX_TIME = calcHP(tasks);

    sort(order.begin(), order.end(),compare_release);
    // cout << "Printing order...\n";
    // cout << "id | period | execution | deadline | abs_deadline | remaining_time | release_time| st\n";
    // for(auto t:order) {
        // cout<<t.id<<" "<<t.period<<" "<<t.execution<<" "<<t.deadline<<" " << t.abs_deadline << " " <<t.remaining_time<<" "<<t.release_time<<" "<< t.slack_time<< endl;
    // }
    // cout << "\n\n\n";
    
    
    ScheduleTasks(order);
    cout << "Scheduled saved in schedule.csv\n";

    return 0;
}
