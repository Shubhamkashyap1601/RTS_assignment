#include<bits/stdc++.h>
using namespace std;

struct Task{
    int id;
    int period;
    int execution;
    int deadline;
};

Task generator() {
    Task New_task;
    do {
        New_task.period = (rand() % 80)+1;
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
        task[i] = generator();
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