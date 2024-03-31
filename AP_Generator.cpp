#include<bits/stdc++.h>
using namespace std;

struct Task{
    int id;
    int release_time;
    int execution;
};

Task generator(int n) {
    Task New_task;
        New_task.execution = (rand() % 10)+1;
        New_task.release_time = (rand() % (50*n));
    
    return New_task;
}

int main(){
    int n;
    cout<<"Enter the number of Aperiodic tasks that you want to generate: ";
    cin>>n;
    vector<Task> task(n);
    for(int i=0;i<n;i++){
        task[i] = generator(n);
    }


    ofstream outputFile("AP_tasks.csv");

    outputFile << "id,release_time,Execution\n";
    
    for (int i = 0; i < n; i++) {
        auto [id,release_time, execution] = task[i];
        outputFile << i+1 <<","<< release_time << "," << execution<< "\n";
    }

    outputFile.close();

    cout << "Data written to tasks.csv successfully." << endl;
    return 0;
}