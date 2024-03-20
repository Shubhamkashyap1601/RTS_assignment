#include<bits/stdc++.h>
using namespace std;

struct Task{
    int id;
    int period;
    int execution;
    int deadline;
};

bool isSchedulable(const vector<Task>& tasks) {
    double utilization = 0;
    for (const auto& task : tasks) {
        utilization += (double)task.execution / (double)task.period;
    }
    cout << "Utilization: " << utilization << endl;
    return utilization <= 1;
}

Task generator(int n) {
    Task New_task;
    do {
        New_task.period = (rand() % (n*10))+1;
        New_task.execution = (rand() % 5)+1;
        New_task.deadline = New_task.period;
    } while (New_task.period < New_task.execution); // Ensure period >= execution
    return New_task;
}

int main(){
    int n;
    cout<<"Enter the number of periodic tasks that you want to generate ";
    cin>>n;
    vector<Task> task(n);
    for(int i=0;i<n;i++){
        task[i] = generator(n);
    }

    while(!isSchedulable(task)){
        cout << "Taskset not schedulable, Regenerating taskset...\n";
        // task.clear();
        for(int i=0;i<n;i++){
            task[i] = generator(n);
        }
    }
    // cout << "----------------------------------\n";
    // cout << "| Period | Execution |\n";
    // cout << "----------------------------------\n";
    // for(int i = 0; i < n; i++) {
    //     auto [x, y] = task[i];
    //     printf("|%7d |%10d |\n", x, y);
    // }
    // cout << "----------------------------------\n";

    ofstream outputFile("tasks.csv");

    outputFile << "id,Period,Execution,deadline\n";
    
    for (int i = 0; i < n; i++) {
        auto [id,period, execution,deadline] = task[i];
        outputFile << i+1 <<","<< period << "," << execution<<","<<deadline << "\n";
    }

    outputFile.close();

    cout << "Data written to tasks.csv successfully." << endl;
    return 0;
}