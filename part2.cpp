#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;
# define MAXSIZE 10

int numbers_process=0;
struct process{
    int process_id;         // process id
    int arrival_time;       // process arrival time
    int burst_time;         // process burst time
    int r_burst_time;	    // a copy of burst time
    int priority;           // process priority
    int begin_time;         // when process begin
    int over_time;          // when process finish
    int waiting_time;       // process wating time
    int turnaround_time;    // process turnaround time: completion time - arrival time
    int is_completed;		// if the process is completed, it will be set to 1

}pcb[MAXSIZE];


// compare the process according to the arrvial time (from early to later)
bool CmpByArrvialTime(process p1, process p2){
    return p1.arrival_time<p2.arrival_time;
}

void get_BeginAndOverTime(){
    for (int i=0; i<MAXSIZE; i++){
        if (i == 0){
            pcb[i].begin_time = pcb[i].arrival_time;
        }
        else{
            pcb[i].begin_time = pcb[i-1].over_time;
        }
        pcb[i].over_time = pcb[i].begin_time + pcb[i].burst_time;
    }
}

void get_WaitingAndTurnaroundTime(){
    for (int i=0; i<MAXSIZE; i++){
        pcb[i].waiting_time = pcb[i].begin_time - pcb[i].arrival_time;
        pcb[i].turnaround_time = pcb[i].over_time - pcb[i].arrival_time;
    }
}

void FCFS(){
    sort(pcb, pcb+numbers_process, CmpByArrvialTime);
    get_BeginAndOverTime();
    get_WaitingAndTurnaroundTime();
    
}

// The smaller number, the higher priority
bool CmpByPriority(process p1, process p2){
    return p1.priority<p2.priority;
}

void nonPreem_PrioritySchedule(){
    // tmp_pcb is used to do temporary processing job
    process tmp_pcb[10];    
    int tmp_i;
    int wholeBurstTime;
    // index of pcbCopy, the number of items in the tmp_cpy
    int copy_j = 0;
    sort(pcb, pcb+numbers_process, CmpByArrvialTime);

    for(int q=1; q<numbers_process; q++){
        for(int w=0; w<q; w++){
            // wholeBurstTime += pcbCopy[w].burst_time;
            wholeBurstTime += pcb[w].burst_time;
        }
        // put all the processes who arrive before the wholeBurstTime into the tmp_pcb queue
        for(int e=q; e<numbers_process; e++){
            if(pcb[e].arrival_time <= wholeBurstTime){
                tmp_pcb[copy_j] = pcb[e];
                copy_j++;
            }
        }
        // sort the tmp_pcb based on their priority, then tmp_pcb[0] is the next process to execute
        sort(tmp_pcb, tmp_pcb+copy_j, CmpByPriority);

        // do exchanging operation inside the pcb queue
        for(int r=q; r<numbers_process; r++){
            if(pcb[r].process_id == tmp_pcb[0].process_id){
                process tmp_pro_swap;
                tmp_pro_swap = pcb[r];
                pcb[r] = pcb[q];
                pcb[q] = tmp_pro_swap;
            }
        }
        // reset the counter
        copy_j = 0;
        // clear the tmporary tmp_pcb
        memset(tmp_pcb, 0, sizeof(tmp_pcb));
    }
    get_BeginAndOverTime();
    get_WaitingAndTurnaroundTime();
}

int queue[20];		// ready queue
int head = -1;
int tail = -1;

// when a process is qualified, it will be moved to the end of the queue
void enter_queue(int position_i){
	tail++;
	queue[tail] = position_i;
	if(head == -1){
		head = 0;
	}
}

// when a process (the head of a queue) is executed, it is removed out of the queue
int quit_queue(){
	int tmp;
	tmp = queue[head];
	if(head==tail){
        // all processes have been executed, the ready queue is empty
		head = -1;
		tail = -1;
	}
	else{
        // one process executes, the head pointer increase
		head++;
	}
	return tmp;
}

// if a process is in the ready queue, it return 1
int Is_InQueue(int position_i){
	int p;
	for(p=head;p<=tail;p++){
		if(queue[p]==position_i){
			return 1;
		}
	}
	return 0;
}

void pcbClear(){
    for (int i = 0; i < MAXSIZE; i++)
    {
        pcb[i].process_id = 0; 
        pcb[i].arrival_time = 0;
        pcb[i].burst_time = 0;
        pcb[i].r_burst_time = 0;    
        pcb[i].priority = 0;        
        pcb[i].begin_time = 0;      
        pcb[i].over_time = 0;       
        pcb[i].waiting_time = 0;    
        pcb[i].turnaround_time = 0; 
        pcb[i].is_completed = 0;    
    }
}

void RoundRobin(){
    memset(queue, 0, 20);
    head = -1;
    tail = -1;
    int i,j;                        // the index of pcb[]
	int timeline = 0;
	int sum_bursttime = 0;
	int time_quantum = 2;
	float average_waitingtime;
	string executionSequence;
	// sort the pcb based on its arrival time
	sort(pcb, pcb+numbers_process, CmpByArrvialTime);
	for(int count=0;count<numbers_process;count++){
		sum_bursttime += pcb[count].burst_time;
	}
	// enter the first process into the queue
	// because the pcb[] has been sorted, pcb[0].arrival_time is 0
	enter_queue(0);
    
	cout << "The execution order is: ";
	for(timeline=pcb[0].arrival_time;timeline<sum_bursttime;){
        // i is the index of the pcb that will be executed now
		i = quit_queue();
		if(pcb[i].r_burst_time <= time_quantum){
			// if the rest of burst time of pcb[i] is less than or equal to quantum
			// it can be executed only once before finishing the whole process
			timeline += pcb[i].r_burst_time;
			pcb[i].r_burst_time = 0;
			pcb[i].is_completed = 1;
			pcb[i].waiting_time = timeline - pcb[i].burst_time - pcb[i].arrival_time;
			pcb[i].turnaround_time = timeline - pcb[i].arrival_time;
			executionSequence += to_string(pcb[i].process_id) + " ";
			// while pcb[i] is executing, some processes have already arrived
			for(j=0;j<numbers_process;j++){
				// if pcb[j] has arrived after the execution of pcb[i]
				// and pcb[j] hasn't completed and pcb[j] is now in the queue
				// pcb[j] needs to be put into ready queue
				if(pcb[j].arrival_time<=timeline && pcb[j].is_completed!=1 && Is_InQueue(j)!=1){
					enter_queue(j);
				}

			}

		}
		else if(pcb[i].r_burst_time > time_quantum){
			timeline += time_quantum;
			pcb[i].r_burst_time = pcb[i].r_burst_time - time_quantum;
			pcb[i].is_completed = 0;
			executionSequence += to_string(pcb[i].process_id) + " ";
			// while pcb[i] is executing, some processes have already arrived
			for(j=0;j<numbers_process;j++){
				if(pcb[j].arrival_time<=timeline && pcb[j].is_completed!=1 && Is_InQueue(j)!=1 && i!=j){
					enter_queue(j);
				}
			}
			// in the end, put the pcb[i] which was just executed back into
			// the end of the queue
			enter_queue(i);
		}
        
	}
    cout << executionSequence << endl;
}


// split a string based on a specific symbol
void Split(const string& src, const string& separator, vector<string>& dest){	
    string str = src;	
    string substring;	
    // size_type is a variable type to make sure the variable I use to support large
    // enough size of any string
    string::size_type start = 0, index;	
    dest.clear();	
    // search the string for the first occurence of any character of separator
    index = str.find_first_of(separator,start);	
    do	{		
        // npos is a static member constant value with the greatest possible value for an element of type size_t.
        if (index != string::npos)  {    	
            // get the string before the first separator		
            substring = str.substr(start,index-start );	
            // push the substring into the <string>dest		
            dest.push_back(substring);
            // re-assign the value of start--the start index to search the string next time		
            start =index+separator.size();
            // find() is to search the string for the first string which match the whole "separator" string.
            // when the variable's type is size_t, when find() find nothing, it will return npos			
            index = str.find(separator,start);		
            if (start == string::npos) break;		
            }	
        }
    while(index != string::npos); 	
    // the last part	
    substring = str.substr(start);	
    dest.push_back(substring);
}

void loadTxtFile(int file_select=0){
    pcbClear();
    char buffer[256];
    string tmpstring[256];
    vector<string> Data;
    string string_pcb[128];
    string b[3];	
    int flag=0;
    int b_count;
    int line_count=0;
    fstream outFile;
    if (file_select == 0){
        outFile.open("/Users/ouyang/desktop/schedule_sample.txt",ios::in);
    }else{
        outFile.open("/Users/ouyang/desktop/schedule_sample2.txt",ios::in);
    }
    while(!outFile.eof())    { 
        // getline(char *,int,char) if it meets the '\n' or 256 character, then quit       
        outFile.getline(buffer,256,'\n');
        tmpstring[line_count]=buffer;
        Split(tmpstring[line_count],",",Data);
        for (int c=0;c<Data.size();c++)	{	
            string_pcb[c] = Data[c].c_str();	
        }
        // stoi() convert string to int
        pcb[line_count].process_id = stoi(string_pcb[0]);
        pcb[line_count].arrival_time = stoi(string_pcb[1]);
        pcb[line_count].burst_time = stoi(string_pcb[2]);
        pcb[line_count].r_burst_time = stoi(string_pcb[2]);
        pcb[line_count].priority = stoi(string_pcb[3]);

        line_count++;
        // cout << string_pcb[0] <<endl;
    }    
    outFile.close();
    // one line = one process, get the numbers of processes
    numbers_process = line_count;
    cout << endl << "Numbers of process is " << numbers_process << endl;
    
}

void print_ScheduleList(){
    int average_waitingtime = 0;
    int average_turnaroundtime =0;
    int inter_space = 17;
    cout << "------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Process_id" << "\t" << "Arrival_time" << "\t" << "Burst_time" << "\t" << "Priority"<< "\t" 
        << "Begin_time" << "\t" << "Over_time" << "\t" << "Waiting_time" 
        << "\t" <<"Trunaround time" << endl;

    for (int i=0; i<numbers_process; i++){
        cout << left << setw(inter_space) << pcb[i].process_id << left << setw(inter_space) << pcb[i].arrival_time <<
            left << setw(inter_space) << pcb[i].burst_time << left << setw(inter_space) << pcb[i].priority << left 
            << setw(inter_space) << pcb[i].begin_time << left << setw(inter_space) << pcb[i].over_time << left 
            << setw(inter_space) << pcb[i].waiting_time << left << setw(inter_space) << pcb[i].turnaround_time << endl;
        average_waitingtime += pcb[i].waiting_time;
        average_turnaroundtime += pcb[i].turnaround_time;
    }
    cout << "------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Average waiting time is: " << float(average_waitingtime)/float(numbers_process) << endl;
    cout << "Average turnaround time is: " << float(average_turnaroundtime)/float(numbers_process) << endl;
}

void print_RR(){
    int average_waitingtime = 0;
    int average_turnaroundtime =0;
    int inter_space = 17;
    cout << "----------------------------------------------------------------------------------------------------" << endl;
    cout << "Process_id" << "\t" << "Arrival_time" << "\t" << "Burst_time" << "\t"
        << "Priority" << "\t" << "Waiting_time" << "\t" << "Trunaround time" << endl;
    for (int i=0; i<numbers_process; i++){
        cout << left << setw(inter_space) << pcb[i].process_id << left << setw(inter_space) << pcb[i].arrival_time <<
            left << setw(inter_space) << pcb[i].burst_time << left << setw(inter_space) << pcb[i].priority << left << 
            setw(inter_space) << pcb[i].waiting_time << left << setw(inter_space) << pcb[i].turnaround_time << endl;
        average_waitingtime += pcb[i].waiting_time;
        average_turnaroundtime += pcb[i].turnaround_time;
    }
    cout << "----------------------------------------------------------------------------------------------------" << endl;
    cout << "Average waiting time is: " << float(average_waitingtime)/float(numbers_process) << endl;
    cout << "Average turnaround time is: " << float(average_turnaroundtime)/float(numbers_process) << endl;
}

int main(){
    loadTxtFile();
    nonPreem_PrioritySchedule();
    cout << "The non-Preemptive Priority scheduling list is as below: (execution sequence)" << endl;
    print_ScheduleList();
    

    loadTxtFile();
    FCFS();
    cout << "The FCFS scheduling list is as below: (execution sequence)" << endl; 
    print_ScheduleList();

    loadTxtFile(0);
    cout << "The RR scheduling list is as below:" << endl; 
    RoundRobin();
    print_RR();

    loadTxtFile(1);
    cout << "The RR scheduling (large set of processes) list is as below:" << endl; 
    RoundRobin();
    print_RR();

}