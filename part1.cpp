#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;
// Doubly linked list

class Node {
public:
	int data;       // data of the process
    int PID;        // id of the process
    int priority;   // priority of process
    string state;   // node's state: ready/running
	Node *pNext;    // next pointer
    Node *pPre;     // previous pointer
    Node *tail;     // tail pointer, only the tail pointer of the head node has value
};
 
class List_ProcessQueue{
private:
    Node *head;
    string state;
public:
    List_ProcessQueue(){
        head = new Node;
        head -> data = 0;
        head -> PID = 0;
        head -> priority = 0;
        head -> pNext = NULL;
        head -> pPre = NULL;
        head -> tail = NULL;
    }
    List_ProcessQueue(string state){
        this->state = state;
        head = new Node;
        head -> data = 0;
        head -> PID = 0;
        head -> priority = 0;
        head -> pNext = NULL;
        head -> pPre = NULL;
        head -> tail = NULL;
    }
    ~List_ProcessQueue(){
        delete head;
    }
    void CreateLinkList(int n);				//Create a LinkList
	void InsertNode(int position, int PID, int d, int prio);	//Insert a node in a specific position
	void TraverseLinkList();				//Print the whole list
	bool IsEmpty();							//See if the list is empty
	int GetLength();						//Get the length of the list
	void DeleteNode(int position);			//Delete a node at the specific position
    void DeleteNode_PID(int PID);           //Delete a node with the PID
    void loadTxtFile();                     //load data of txtfile into the list
	void DeleteLinkList();					//delete the whole LinkList
};

void List_ProcessQueue::CreateLinkList(int n){
    if (n < 1){
        cout << "The number you input is not valid." << endl;
        exit(EXIT_FAILURE);
    }
    else {
        Node *pnew, *ptmp; //create two new node
        ptmp = head;
        int i = n;
        while (n-- > 0){
            pnew = new Node;
            pnew->data = i-n;
            if(this->state == "ready"){
                pnew->PID = i-n;
            }else{
                pnew->PID = i-n+20;
            }
            pnew->priority = rand()%5;
            // new node will be added after ptmp
            ptmp->pNext = pnew;
            // there is no node after the new added node
            pnew->pNext = NULL;
            // set the node status the same as the queue property
            pnew->state = this->state;
            // the pre pointer of new node points to ptmp
            pnew->pPre = ptmp;
            // update the ptmp node
            ptmp = pnew;
            // update the tail information of head node
            head->tail = pnew;
        }
    }
}

void List_ProcessQueue::InsertNode(int position, int PID, int data, int priority){
    // Exception handling
    if (position < 1 || position > GetLength()+1){
        cout << "The position does not exist." << endl;
        return ;
    }
    else {
        Node *pnew, *ptmp;
        pnew = new Node;
        ptmp = head;
        // Whether the cin PID already exists in the queue or not
        while (ptmp->PID!=PID){
            ptmp = ptmp->pNext;
            // error handling
            if(ptmp->PID == PID){
                cout<<"The PID already exsits, fail to insert"<<endl; 
                return ;
            }
            // Can't find the PID in the queue, the node can be inserted successfully
            if(ptmp->pNext == NULL){break;}
        }
        ptmp = head;
        pnew->data = data;
        pnew->PID = PID;
        pnew->priority = priority;
        pnew->state = this->state;
        // the head node is actually in the queue, so it's position>1, not 0
        while (position-- > 1){
            ptmp = ptmp->pNext;
        }
        // insertion node isn't the last note of the queue, re-link the nodes before and after ptmp
        if (ptmp->pNext != NULL){       
            ptmp->pNext->pPre = pnew;   
        }
        // establish the link between two nodes
        pnew->pNext = ptmp->pNext;
        ptmp->pNext = pnew;
        pnew->pPre = ptmp; 
        // update the ptmp node
        ptmp = pnew;
        //if the next node after pnew is NULL, then it is the tail
        if (ptmp->pNext == NULL){
            head->tail = ptmp;
        }
    }
}

// Print the whole list
void List_ProcessQueue::TraverseLinkList(){
    Node *ptmp = head->pNext;
    // cout << "Here is the Queue:"<<endl;
    if(this->state == "ready"){
        cout << "Ready Queue:" << endl;
    }else if(this->state == "waiting"){
        cout << "Waiting Queue:" << endl;
    }
    cout << "---------------------------------------------------------------" << endl;
    while (ptmp!=NULL){
        cout << left << "PID: " << setw(10) << ptmp->PID << left 
            <<  "Data: " << setw(10) << ptmp->data << left << setw(10) 
            <<  "Priority: "<< setw(10) << ptmp->priority << "State: " 
            << ptmp->state << endl;
        ptmp = ptmp->pNext;
    }
    cout << "---------------------------------------------------------------" << endl;
}

int List_ProcessQueue::GetLength(){
    int n = 0;
    Node *ptmp = head->pNext;
    while (ptmp != NULL){
        n++;
        ptmp = ptmp->pNext;
    }
    return n;
}

bool List_ProcessQueue::IsEmpty(){
    if (head->pNext==NULL){
        return true;
    }
    return false;
}

// Delete the specific PID node
void List_ProcessQueue::DeleteNode_PID(int PID){
    Node *ptmp = head->pNext;
    while (ptmp->PID!=PID){
        ptmp = ptmp->pNext;
        // error handling
        if(ptmp == NULL){cout<<"The PID does not exsit"<<endl; return ;}
    }
    Node *pdelete;
    pdelete = ptmp;
    if(pdelete->pNext == NULL){         //pdelete is the last node of the queue
        pdelete->pPre->pNext = NULL;
        head->tail = pdelete->pPre;
    }
    else{
        ptmp->pPre->pNext = pdelete->pNext;
    }
    delete pdelete;
    pdelete = NULL;
}

void List_ProcessQueue::DeleteNode(int position){
    if (position < 1 || position > GetLength()+1){
        cout << "The position does not exsit, please enter again:" << endl << endl;
        //quit this deleteNode function but not quit the main function
        return;     
    }
    else{
        Node *pdelete, *ptmp;
        ptmp = head;
        while (position-- > 1){           
            ptmp = ptmp->pNext;
        }
        pdelete = ptmp->pNext; 
        // if pdelete isn't the last node of the queue, re-link the node before and after pdelete         
        if (pdelete->pNext != NULL){        
            pdelete->pNext->pPre = ptmp;
        }else{
            head->tail = pdelete->pPre; 
        }
        ptmp->pNext = pdelete->pNext;
        delete pdelete;
        pdelete = NULL;                 
    }

}
void List_ProcessQueue::DeleteLinkList(){
    Node *pdelete, *ptmp;
    pdelete = head->pNext;
    while (pdelete != NULL){
        ptmp = pdelete->pNext;
        head->pNext = ptmp;
        if (ptmp!=NULL){
            ptmp->pPre = head;
        }
        delete pdelete;
        pdelete = ptmp;
    }
}

// split a string based on a character
void Split(const string& src, const string& separator, vector<string>& dest){	
    string str = src;	
    string substring;	
    string::size_type start = 0, index;	
    dest.clear();	
    index = str.find_first_of(separator,start);	
    do	{		
        if (index != string::npos)  {    			
            substring = str.substr(start,index-start );			
            dest.push_back(substring);			
            start =index+separator.size();			
            index = str.find(separator,start);			
            if (start == string::npos) break;		
            }	
        }
    while(index != string::npos); 	
    //the last part	
    substring = str.substr(start);	
    dest.push_back(substring);

}

// load process information into queues
void List_ProcessQueue::loadTxtFile(){    
    char buffer[256];
    string tmpstring[256];
    vector<string> Data;
    string string_pcb[4];
    string b[3];	
    int flag=0;
    int b_count;
    int i=0;    
    fstream outFile;    
    outFile.open("/Users/ouyang/desktop/part1_sample.txt",ios::in);       
    while(!outFile.eof())    {        
        // getline(char *,int,char) if it meets the '\n' or 256 character, then quit
        outFile.getline(buffer,256,'\n');
        tmpstring[i]=buffer;
        Split(tmpstring[i],",",Data);
        for (int c=0;c<Data.size();c++)	{	
            string_pcb[c] = Data[c].c_str();	
        }
        // stoi() convert string to int
        if(this->state == string_pcb[3]){
            InsertNode(GetLength()+1, stoi(string_pcb[0]), stoi(string_pcb[1]), stoi(string_pcb[2]));
        }   
        i++;
    }    
    outFile.close();
}

int flag = 0;

bool is_int(string x){
    try{
        stoi(x);
        return true;
    }catch(exception &e){
        cout << "The parameter should be a number!" << endl << endl;
        return false;
    }
}

// main function
int main() {
    void error_handle(int position, int PID, int data, int priority);
    cout << "Two queues have already been created(ready queue & waiting queue)." << endl;
    // creating two queues
	List_ProcessQueue dl_ready("ready");
    List_ProcessQueue dl_waiting("waiting");
    // need to manipulate two queues and the PCBs of them
    // pointer of the ready queue
    List_ProcessQueue *dl = new List_ProcessQueue();
    
	int position = 0, pid = 0, value = 0, n = 0, priority = 0;
    // for input error handling
    string posi, pidd, valuee, priorityy;
	// bool flag = false;
    int cinflag = 0;
	cout << "Enter the number of processes need to be created for the first time for each queue：";
	cin >> n;
    cout << endl;
	dl_ready.CreateLinkList(n);
	dl_ready.TraverseLinkList();
    dl_waiting.CreateLinkList(n);
    dl_waiting.TraverseLinkList();

    int choice;
    while(flag == 0){
        // select a queue to operate
        string queueChoice;
        cout << "Select a queue to operate (1 for first queue, 2 for second queue): " ;
        cin >> queueChoice;
        if (is_int(queueChoice)){
            if(stoi(queueChoice) == 1){
                *dl = dl_ready;
            }else if(stoi(queueChoice) == 2){
                *dl = dl_waiting;
            }else{
                cout << "There is no such queue!" << endl;
                break;
            }
            dl->TraverseLinkList();
        }

        cout << "1)Insert a process" << endl;
        cout << "2)Delete a process" << endl;
        cout << "3)Delete a process based on PID" << endl;
        cout << "4)Print the queue" << endl;
        cout << "5)Check linklist is empty or not"<<endl;
        cout << "6)Insert processes from .txt:" << endl;
        cout << "7)Exit" << endl << endl;
        cout << "Please enter the function number: ";
        cin >> choice;
        switch (choice){
            case 1:
                cout << "Please enter the position, PID, data and priority of the node inserted: " << endl;
                cout << "Please enter the position: ";
	            cin >> posi;
                cout << "Please enter the pid: ";
                cin >> pidd;
                cout << "Please enter the value: ";
                cin >> valuee;
                cout << "Please enter the priority: ";
                cin >> priorityy;
                if((is_int(posi))&&(is_int(pidd))&&(is_int(valuee))&&(is_int(priorityy))){
                    position = stoi(posi);
                    pid = stoi(pidd);
                    value = stoi(valuee);
                    priority = stoi(priorityy);
                    dl->InsertNode(position, pid, value, priority); //when value is miss-input as a String, error!
                    dl->TraverseLinkList();
                }
                break;
            case 2:
                cout << "Please enter the position of the node deleted：";
	            cin >> posi;
                if(is_int(posi)){
                    position = stoi(posi);
                    dl->DeleteNode(position);
                }
                dl->TraverseLinkList();
                break;

            case 3:
                cout << "Please enter the PID you would like to delete: " ;
                cin >> pidd ;
                if(is_int(pidd)){
                    pid = stoi(pidd);
                    dl->DeleteNode_PID(pid);
                    dl->TraverseLinkList();
                }
                break;

            case 4:
                dl->TraverseLinkList();
                break;
            case 5:
                flag = dl->IsEmpty();
                if (flag){
                    cout<<"The linklist is empty"<<endl;
                }else{
                    cout<<"The linklist is not empty"<<endl;
                }
                break;
            
            case 6:
                dl->loadTxtFile();
                dl->TraverseLinkList();
                break;
            case 7:
                dl->DeleteLinkList();
	            flag = dl->IsEmpty();
                delete dl;
	            if (flag)
		            cout << "Successfully delete linklist！" << endl;
	            else
		            cout << "Failed to delete linklist！" << endl;
                return 0;
        }
    }
}

