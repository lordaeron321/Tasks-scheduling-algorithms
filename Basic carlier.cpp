#include <iostream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

class Task {
public:
    int R;
    int P;
    int Q;
    int exe_moment;
    int index;

    Task(int r, int p, int q, int I) : R(r), P(p), Q(q), index(I) {
        exe_moment = 0;
    }
    friend ostream& operator<< (ostream& s, const Task& v);
};

vector<string> mystrtok(string str, char delim) {
    vector<string> tokens;
    string temp = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == delim) {
            tokens.push_back(temp);
            temp = "";
        }
        else
            temp += str[i];
    }
    tokens.push_back(temp);
    return tokens;
}

vector<Task> getdata(string path) {
    int i = 1;
    vector<Task> tasks;

    ifstream myfile;
    myfile.open(path);
    string myline;
    if (myfile.is_open()) {
        while (myfile) {
            getline(myfile, myline);
            vector<string> tokens = mystrtok(myline, ' ');
            int r = stoi(tokens[0]);
            int p = stoi(tokens[1]);
            int q = stoi(tokens[2]);
            Task current_task(r, p, q, i);
            tasks.push_back(current_task);
            i++;
        }
    }
    else {
        cout << "Couldn't open file\n";
    }

    return tasks;

}
vector<int>nodes;

bool cmpR(const Task& l, const Task& r)
{
    return l.R < r.R;
}
bool cmpQ(const Task& l, const Task& r)
{
    return l.Q < r.Q;
}
ostream& operator<< (ostream& s, const Task& v)
{
    return s << "R: " << v.R;
}

bool isEqual(const Task& task, int r, int p, int q, int index) {
    return task.R == r && task.P == p && task.Q == q && task.index == index;
}


bool compareTasksByR(const Task& a, const Task& b) {
    return a.R < b.R;
}


bool compareTasksByQ(const Task& a, const Task& b) {
    return a.Q > b.Q;
}
static int CmaxCount(vector<Task> tasks) {
    int time = 0, Cmax = 0;
    for (auto it : tasks)
    {

        if (it.R >= time) {
            time = it.R + it.P;
        }
        else {
            time += it.P;
        }
        Cmax = max(Cmax, time + it.Q);
  
    }
    return Cmax;
}
//schrage z przerwaniem zadan
int schragePmtn(vector<Task> tasks) {

    sort(tasks.begin(), tasks.end(), compareTasksByR);
    int time = 0;
    int Cmax = 0;
    Task l(0, 0, INFINITY, 0);
    vector<Task> N;
    vector<Task> D;
    N = tasks;

    while (!N.empty() || !D.empty()) {
        while (!N.empty() && N.front().R <= time) {
            D.push_back(N.front());
            sort(N.begin(), N.end(), compareTasksByR);
            Task t = N.front();
            N.erase(N.begin());

            if (t.Q > l.Q) {
                l.P = time - t.R;
                time = t.R;
            }
            if (l.P > 0)D.push_back(l);
        }

        if (D.empty()) {
            time = N.front().R;
            continue;
        }

        sort(D.begin(), D.end(), compareTasksByQ);
        Task t = D.front();
        D.erase(D.begin());

        time += t.P;
        Cmax = max(Cmax, time + t.Q);
    }

    return Cmax;
}
//schrage
int schrage(vector<Task>& tasks) {

   
    sort(tasks.begin(), tasks.end(), compareTasksByR);
    vector<Task> out_tasks;

    int time = 0;
    int Cmax = 0;
    vector<Task> N;
    vector<Task> D;
    N = tasks;

    while (!N.empty() || !D.empty()) {
        while (!N.empty() && N.front().R <= time) {
            D.push_back(N.front());
            sort(N.begin(), N.end(), compareTasksByR);
            Task t = N.front();
            N.erase(N.begin());

        }

        if (D.empty()) {
            time = N.front().R;
            continue;
        }

        sort(D.begin(), D.end(), compareTasksByQ);
        Task t = D.front();
        D.erase(D.begin());

        time += t.P;
        Cmax = max(Cmax, time + t.Q);
        t.exe_moment = time - t.P;
        out_tasks.push_back(t);

    }
    tasks = out_tasks;
    return Cmax;
}
int carlier(vector<Task> tasks) {
    
   

    static int ITERATION = 1;
    int critical_tasks_sum = 0;
    int Cmax = schrage(tasks);
    
    vector<Task> critical_tasks;
    critical_tasks.clear();

    int local_Cmax = 0;

    vector<Task> ActualCriticalPath;
    ActualCriticalPath.clear();

    

    for (int task_no = 0; task_no < tasks.size(); task_no++) {

        if (tasks[task_no].exe_moment - tasks[task_no].R == 0) {
            
            int i_sum = task_no;
            while ((local_Cmax != Cmax && i_sum < tasks.size())) {
                if (i_sum < tasks.size() - 1) {
                    if (tasks[i_sum].exe_moment + tasks[i_sum].P == tasks[i_sum + 1].exe_moment) {
                        local_Cmax = max(local_Cmax, tasks[i_sum].exe_moment + tasks[i_sum].P + tasks[i_sum].Q);
                        critical_tasks.push_back(tasks[i_sum]);

                    }
                    else {
                        critical_tasks.clear();
                        break;
                    }
                }
                else {
                    if (tasks[i_sum].exe_moment - tasks[i_sum - 1].P == tasks[i_sum - 1].exe_moment) {
                        local_Cmax = max(local_Cmax, tasks[i_sum].exe_moment + tasks[i_sum].P + tasks[i_sum].Q);
                        critical_tasks.push_back(tasks[i_sum]);
                    }
                }
                i_sum++;
            }
        }
    }
    

    if (local_Cmax == Cmax) {
        Task critical_task = critical_tasks[0];
        int critical_task_index_in_vector = 0;
        for (int Potentally_critical_task = 1; Potentally_critical_task < critical_tasks.size() - 1; Potentally_critical_task++) {
            if (critical_tasks[Potentally_critical_task].Q < critical_task.Q) {
                critical_task = critical_tasks[Potentally_critical_task];
                critical_task_index_in_vector = Potentally_critical_task;

            }
        }

        if (critical_task.Q >= critical_tasks.back().Q) {
            return Cmax;
        }
        
        else {
            for (int i = critical_task_index_in_vector + 1; i < critical_tasks.size(); i++) {
                if (critical_tasks[i].Q >= critical_task.Q)ActualCriticalPath.push_back(critical_tasks[i]);
                else return Cmax;
            }
         
        }

      
        //koniec wyznaczania sceizki krytycznej

     
        //wyznaczanie wariantow polozenia zadania interferencyjnego
        int sumP = 0;
        for (int j = 0; j < ActualCriticalPath.size(); j++) {
            sumP += ActualCriticalPath[j].P;
        }

        auto rK_it = min_element(ActualCriticalPath.begin(), ActualCriticalPath.end(), cmpR);
        auto qK_it = min_element(ActualCriticalPath.begin(), ActualCriticalPath.end(), cmpQ);

        Task rK = *rK_it, qK = *qK_it;


        int UB = Cmax;
        int LB = schragePmtn(tasks);
 
        if (Cmax <= LB || Cmax > UB)return Cmax;

        //H1
        vector<Task> left_node;
        int r1 = rK.R + sumP;
        Task replaced_interf_task_left_node(r1, critical_task.P, critical_task.Q, critical_task.index);
        left_node = ActualCriticalPath;
        left_node.push_back(replaced_interf_task_left_node);

        auto min_r_after_replacement_it = min_element(left_node.begin(), left_node.end(), cmpR);
        Task min_r_after_replacement_task = *min_r_after_replacement_it;
        int min_r_after_replacement = min_r_after_replacement_task.R;
        //H1

        //podmiana qK na zadanie b 
        qK = ActualCriticalPath.back();

        //H2
        vector<Task> right_node;
        int q1 = qK.Q + sumP;
        Task replaced_interf_task_right_node(critical_task.R, critical_task.P, q1, critical_task.index);

        right_node = ActualCriticalPath;
        right_node.push_back(replaced_interf_task_right_node);

        auto min_q_after_replacement_it = min_element(right_node.begin(), right_node.end(), cmpQ);
        Task min_q_after_replacement_task = *min_q_after_replacement_it;
        int min_q_after_replacement = min_q_after_replacement_task.Q;
        //H2

        
        int H1 = min_r_after_replacement + sumP + replaced_interf_task_left_node.P + replaced_interf_task_left_node.Q;
        int H2 = replaced_interf_task_right_node.R + sumP + replaced_interf_task_right_node.P + min_q_after_replacement;


   

        vector<Task>new_left_node = tasks;
        for (auto& ite : new_left_node) {
            if (ite.index == replaced_interf_task_left_node.index)ite.R = replaced_interf_task_left_node.R;
        }

        vector<Task>new_right_node = tasks;
        for (auto& iter : new_right_node) {
            if (iter.index == replaced_interf_task_right_node.index)iter.Q = replaced_interf_task_right_node.Q;
        }

        if( H1 < UB && H2 < UB ) {
      
            return min(carlier(new_left_node), carlier(new_right_node));
        }
        else if (H1 < UB) {
           
            return carlier(new_left_node);
        }

        else if (H2 < UB) {
        
            return carlier(new_right_node);
        }
       
        else {
            return Cmax;
        }

    }
    else {
     
        return Cmax;
    }

}

int main() {
   
    vector<Task> Data = getdata("Data.txt");

    int result = carlier(Data);
    
   
   
    cout << endl << "Cmax: " << result;

    return 0;
}
