#include "tinbes.h"

#define TABLE_SIZE 25

string name[TABLE_SIZE];
long addr[TABLE_SIZE];
long size[TABLE_SIZE];
int noOfFiles = 0;

void sort() {
  bool sorted = false;
  long tmp;
  string s;
  while (!sorted) {
    sorted = true;
    for (int i = 0; i < noOfFiles - 1; i++) {
      if (addr[i] > addr[i + 1]) {
        //switch them around
        string tempname = name[i];
        long tempaddr = addr[i];
        long tempsize = size[i];
        name[i] = name[i+1];
        addr[i] = addr[i+1];
        size[i] = size[i+1];
        name[i+1] = tempname;
        addr[i+1] = tempaddr;
        size[i+1] = tempsize;
        sorted = false;
      }
    }
  }
}

long findFreeSpace(long fileSize) {
  sort();
  // Check if there is not a block allocated yet
  if(noOfFiles == 0){
    return 0;
  }
  
  // Check if there is space between 0 and the first address
  else if(addr[0] >= fileSize){
    return 0;
  }
  
  //check if there is space between 2 used addresses
  for (int i = 0; i < noOfFiles - 1; i++) {
    if(addr[i + 1] - (addr[i] + size[i]) >= fileSize){
      return addr[i] + size[i];
    }
  }
  
  // Check if there is space left after the last address used
  if (getMem() - (addr[noOfFiles - 1] +  size[noOfFiles - 1]) >= fileSize) return addr[noOfFiles - 1] + size[noOfFiles - 1];
	return -1;
}

int findFile(string fileName) {
  for (int i = 0; i < noOfFiles; i++) {
    if (name[i] == fileName) return i;
  }
  return -1;
}

void store(long fileSize, string fileName, char *data) {
  int f = findFreeSpace(fileSize);
  if (f == -1) {
    cout << "Not enough space." << endl;
  }
  else if (findFile(fileName) != -1) {
    cout << "File already exists." << endl;
  }
  else {
    // Store file at address (F)
    name[noOfFiles] = fileName;
    addr[noOfFiles] = f;
    size[noOfFiles] = fileSize;
    cout << f <<endl;
    noOfFiles++;

    for (int i = 0; i < fileSize; i++) {
      writeByte(data[i], f + i);
    }
  }
}

string retrieve(string fileName) {
  int f = findFile(fileName);
  if (f == -1) {
    cout << "File not found." << endl;
    return string();
  }
  string ret;
  for (long a = addr[f]; a < addr[f] + size[f]; a++) {
    ret += readByte(a);
  }
  return ret;
}

void erase(string fileName) {
  int f = findFile(fileName);
  if (f == -1) {
    cout << "File not found." << endl;
    return;
  }
  for (int i = f; i < noOfFiles; i++){
    name[i] = name[i+1];
    addr[i] = addr[i+1];
    size[i] = size[i+1];
  }
  noOfFiles--;
}

void copy(string fileName1, string fileName2) {
  string contents = retrieve(fileName1);
  long fileSize = contents.length();
  if (fileSize > 0) {
    char *data = (char *)malloc(fileSize);
    contents.copy(data, fileSize);
    store(fileSize, fileName2, data);
    free(data);
  }
}

void rename(string fileName1, string fileName2) {
  int f = findFile(fileName1);
  if (f == -1) {
    cout << "File not found." << endl;
  }
  else if (findFile(fileName2) != -1) {
    cout << "File already exists." << endl;
  }
  else {
    name[f] = fileName2;
    return;
  }
}

void files() {
  for (int i = 0; i < noOfFiles; i++) {
    cout << name[i] << '\t' << size[i] << endl;
  }
}

long freeSpace() {
  sort();
  long largest = 0, length = 0;
  for (int i = 0; i < noOfFiles; i++) {
    length = addr[i+1] - (addr[i] + size[i]);
    //if the new length of address space is bigger than the previous largest freespace, replace it
    if (length > largest) largest = length;
  }
  //Maximum length is the maximum memory - the start
  length = getSize() - (addr[noOfFiles -1] + size[noOfFiles - 1]);
  //If the maximum length is bigger than the previously found largest freespace
  if (length > largest) largest = length;
  return largest;
}

void pack() {
  sort();
  //Check if first address is not null
  if(addr[0] != 0){
    addr[0] = 0;
  }
  //Go through every file
  for (int i = 0; i < noOfFiles; i++) {
    //Move every startaddress to the previous one + size
    addr[i+1] = addr[i] + size[i];
  }
}

int main() {
  string command;
  cout << "Please enter your command  (type HELP for help)" << endl;
  while (true) {
    command = readLine();
    if (command == "STORE") {
	  cout << "Please enter file name" << endl;
      string fileName = readLine();
	  cout << "Please enter file size" << endl;
      long fileSize = stoi(readLine());
      char* data = (char *)malloc(fileSize);
      cin.read(data, fileSize);
      store(fileSize, fileName, data);
      free(data);
	  cout << "All done!" << endl;
    }
    if (command == "RETRIEVE") {
	  cout << "Please enter file name" << endl;
      string fileName = readLine();
      string data = retrieve(fileName);
      if (data.length() > 0) cout << data << endl;
    }
    if (command == "ERASE") {
	  cout << "Please enter file name" << endl;
      string fileName = readLine();
      erase(fileName);
    }
    if (command == "COPY") {
	  cout << "Please enter source file name" << endl;
      string fileName1 = readLine();
	  cout << "Please enter copy file name" << endl;
      string fileName2 = readLine();
      copy(fileName1, fileName2);
    }
    if (command == "RENAME") {
	  cout << "Please enter current file name" << endl;
      string fileName1 = readLine();
	  cout << "Please enter new file name" << endl;
      string fileName2 = readLine();
      rename(fileName1, fileName2);
    }
    if (command == "FILES") {
      files();
    }
    if (command == "FREESPACE") {
      cout << freeSpace() << endl;
    }
    if (command == "PACK") {
      pack();
    }
	if (command == "HELP") {
        cout << "--------------" << endl;
		cout << "STORE" << endl;
		cout << "RETRIEVE" << endl;
		cout << "ERASE" << endl;
		cout << "COPY" << endl;
		cout << "RENAME" << endl;
		cout << "FILES" << endl;
		cout << "FREESPACE" << endl;
		cout << "PACK" << endl;
		cout << "--------------" << endl;
      }
  }
}
