#include "tinbes.h"

#define TABLE_SIZE 25
#define RUNNING 'r'
#define PAUSED 'p'
string processName[TABLE_SIZE];
int id[TABLE_SIZE];
char state[TABLE_SIZE];
long addr[TABLE_SIZE];
int noOfProcesses = 0;
int idCounter = 0;

void initialise(){ //initialise lists so that all the places are empty (-1)
  for(int i = 0; i < 25; i++){
    id[i] = -1;
    addr[i] = -1;
    state[i] = PAUSED;
  }
}

int findProcess(int i) {
  for (int j = 0; j < noOfProcesses; j++) {
    if(id[j] == i){
      return j;
    }

  }
  return -1;
}

int newProcess(string name, long address) {
  for(int i = 0; i < noOfProcesses + 1; i++){ //loop through array
    // Check if process name and/or address isn't in use yet.
    if(name == processName[i] || address == addr[i]){
      cout << "Address or name already in use";
      return -1;
    }
    else if(addr[i] == -1) { //if there is an empty spot
      id[i] = idCounter;
      processName[i] = name;
      addr[i] = address;
      noOfProcesses++;
      return idCounter++;
    }
  }
}

void removeProcess(int i) {
  int j = findProcess(i);
  if(j == -1){
    cout << "Couldn't find process." << endl;
    return;
  }
  cout << "Process " << i << " has been succesfully removed." <<  endl;
  id[j] = -1;
  state[j] = PAUSED;
  addr[j] = -1;
  processName[j] = " ";
  //Move all processes after j one step backwards
  for(int index = j; index < noOfProcesses; index++){
    id[index] = id[index + 1];
    state[index] = state[index + 1];
    addr[index] = addr[index + 1];
    processName[index] = processName[index + 1];
  }
  noOfProcesses--;
}

void executeProcesses() {
  long newAddr;
  for (int i = 0; i < noOfProcesses; i++) {
    if (state[i] == RUNNING) {
      newAddr = execute(addr[i]);
      addr[i] = newAddr;
      if (newAddr == 0) {
        cout << "Process \"" << processName[i] << "\" has terminated." << endl;
        removeProcess(id[i]);
      } else {
        addr[i] = newAddr;
      }
    }
  }
}

void listProcesses() {
  for (int i = 0; i < noOfProcesses; i++) {
    cout << id[i] << ' ' << processName[i] << ' ' << state[i] << endl;
  }
}

void suspendProcess(int i) {
  int j = findProcess(i);
  if (j == -1) {
    cout << "Process does not exist." << endl;
    return;
  }
  if (state[j] == PAUSED) {
    cout << "Process already paused." << endl;
    return;
  }
  else {
    state[j] = PAUSED;
    cout << "Process " << id[j] << " has been paused" << endl;
  }
}

void resumeProcess(int i) {
  int j = findProcess(i);
  if (j == -1) {
    cout << "Process does not exist." << endl;
    return;
  }
  if (state[j] == RUNNING) {
    cout << "Process already running." << endl;
  }
  else {
    state[j] = RUNNING;
    cout << "Process " << id[j] << " has been resumed" << endl;
  }
}

void killProcess(int i) {
  int j = findProcess(i);
  if (j == -1) {
    cout << "Process does not exist." << endl;
  } else {
    suspendProcess(i); //suspend process
    removeProcess(i); //remove process
  }
}

int main() {
  unblockCin();
  initialise();
  string command;
  cout << "Please enter your command  (type HELP for help)" << endl;
  while (true) {
    if (charactersAvailable()) {
      command = readLine();
      if (command == "RUN") {
		cout << "Please enter process name" << endl;
		string name = readLine();
		cout << "Please enter process address" << endl;
        long address = stoi(readLine());
		cout << "Your process id is: ";
        cout << newProcess(name, address) << endl;
      }
      if (command == "LIST") {
        listProcesses();
      }
      if (command == "SUSPEND") {
		cout << "Please enter process id" << endl;
        int i = stoi(readLine());
        suspendProcess(i);
      }
      if (command == "RESUME") {
		cout << "Please enter process id" << endl;
        int i = stoi(readLine());
        resumeProcess(i);
      }
      if (command == "KILL") {
		cout << "Please enter process id" << endl;
        int i = stoi(readLine());
        killProcess(i);
      }
	  if (command == "HELP") {
        cout << "--------------" << endl;
		cout << "RUN - start new process" << endl;
		cout << "LIST - list all processes" << endl;
		cout << "SUSPEND - suspend a process" << endl;
		cout << "RESUME - resume a process" << endl;
		cout << "KILL - kill a process" << endl;
		cout << "--------------" << endl;
      }
    }
    else {
      executeProcesses();
    }
  }
}
