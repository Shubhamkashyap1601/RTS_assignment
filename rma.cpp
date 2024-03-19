#include <bits/stdc++.h>
using namespace std;

#define MAX_TIME 80

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
        else
            return period < other.period; // If release times are the same, sort by period
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
    
}

int main() {
    vector<Task> tasks = ReadTaskInformation("tasks.csv");
    if (!isSchedulable(tasks)) {
        cout << "Tasks are not schedulable using Rate Monotonic Scheduling." << endl;
        return 1;
    }
     vector<Task> order;
    for(auto t:tasks) {
        for(int i=0;i<=MAX_TIME;i+=t.period) {
            t.release_time = i;
            order.push_back(t);
        }
    }

    sort(order.begin(), order.end());

    for(auto t:order) {
        cout<<t.id<<" "<<t.period<<" "<<t.execution<<" "<<t.deadline<<" "<<t.remaining_time<<" "<<t.release_time<<endl;
    }
    
    ScheduleTasks(order);

    return 0;
}
