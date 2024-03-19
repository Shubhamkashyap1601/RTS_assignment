#include <bits/stdc++.h>
using namespace std;

#define MAX_TIME 100

struct Task {
    int id;
    int period;
    int execution;
    int remaining_time;
    int deadline;

    // Overloading '<' operator for set
    bool operator<(const Task& other) const {
        return period < other.period; // Shorter period means higher priority
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
        tasks.push_back(task);
    }
    file.close();

    return tasks;
}



void ScheduleTasks(set<Task>& taskSet) {
    
}

int main() {
    vector<Task> tasks = ReadTaskInformation("tasks.csv");
    if (!isSchedulable(tasks)) {
        cout << "Tasks are not schedulable using Rate Monotonic Scheduling." << endl;
        return 1;
    }

    set<Task> taskSet;
    for (const auto& task : tasks) {
        taskSet.insert(task);
    }

    int time = 0;
    
    ScheduleTasks(taskSet);

    return 0;
}
