// minimum
#include<bits/stdc++.h>
using namespace std;
#define MAX_TIME 100
struct Task{
    int id;
    int period;
    int execution;
    int remaining_time;
    int deadline;

    // Overloading '<' operator for priority queue
    bool operator<(const Task& other) const {
        return period > other.period; // Shorter period means higher priority
    }
};

bool comparePeriod(const Task& t1, const Task& t2) {
    return t1.period < t2.period;
}

bool isSchedulable(const vector<Task>& tasks) {
    double utilization = 0;
    for (const auto& task : tasks) {
        utilization += (double)task.execution / (double)task.period;
    }
    return utilization <= 1;
}

// Function to read task information from a CSV file
vector<Task> ReadTaskInformation(const string& filename) {
    ifstream file("tasks.csv");
    if (!file.is_open()) {
        cout << "Failed to open tasks.csv" << endl;
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
        tasks.push_back(task);
    }
    file.close();

    return tasks;
}

void ScheduleTasks(priority_queue<Task> &taskQueue) {
    int time = 0;
    int totalExecutionTime = 0;
    while (time < MAX_TIME) {
        // Execute the task with the highest priority (shortest period)
        if (!taskQueue.empty()) {
            Task currentTask = taskQueue.top();
            taskQueue.pop();
            totalExecutionTime += currentTask.execution;
            cout << "Time: " << time << ", Total Execution Time: " << totalExecutionTime << endl;
            currentTask.remaining_time--;
            if (currentTask.remaining_time > 0) {
                taskQueue.push(currentTask); // Push the task back into the queue if it's not completed
            }
        }

        time++; // Increment simulation time
    }
}






int main(){
    vector<Task> tasks = ReadTaskInformation("tasks.csv");
    if (!isSchedulable(tasks)) {
        cout << "Tasks are not schedulable using Rate Monotonic Scheduling." << endl;
        return 1;
    }

    priority_queue<Task> taskQueue;
    for (const auto& task : tasks) {
        taskQueue.push(task);
    }

    int time = 0;
    // cout << "Time\tTask ID\tPeriod\tExecution Remaining\tDeadline" << endl;
    
    ScheduleTasks(taskQueue);

    
    return 0;
}