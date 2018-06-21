#include "tinbes.h"
#include <math.h>

#define TABLE_SIZE 25

long addr[TABLE_SIZE];
long length[TABLE_SIZE];
int noOfBlocks = 0;

void sort() {
  bool sorted = false;
  long tmp;
  while (!sorted) {
    sorted = true;
    for (int i = 0; i < noOfBlocks - 1; i++) {
      if (addr[i] > addr[i + 1]) {
        //Switch addr[i] and addr[i + 1] around
        long tempaddr = addr[i];
        long templength = length[i];
        addr[i] = addr[i + 1];
        length[i] = length[i + 1];
        addr[i + 1] = tempaddr;
        length[i + 1] = templength;
        sorted = false;
      }
    }
  }
}

long findFreeSpace(long size) {
  sort();
  // If there is not a block allocated yet
  if(noOfBlocks == 0){
    return 0;
  }
  // If there is space between 0 and the first address
  else if(addr[0] >= size){
    return 0;
  }
      //check if there is space between 2 used addresses
  for (int i = 0; i < noOfBlocks - 1; i++) {
    if(addr[i + 1] - (addr[i] + length[i]) >= size){
      return addr[i] + length[i];
    }
  }
  //if there is space left after the last address used
  if (getMem() - (addr[noOfBlocks - 1] +  length[noOfBlocks - 1]) >= size) return addr[noOfBlocks - 1] + length[noOfBlocks - 1];
  return -1;
}

void allocate(long size) {
  int f = findFreeSpace(size);
  if (f == -1) {
    cout << "Not enough memory." << endl;
  }
  else {
      addr[noOfBlocks] = f;
      length[noOfBlocks] = size;
      noOfBlocks++;
      cout << f << endl;
  }
}

void deallocate(long a) {
  for (int i = 0; i < noOfBlocks - 1; i++) {
     if (addr[i] == a) {
      //move every allocated memory one place back in array, deallocting a
        for(int j = i; j <  noOfBlocks; j++){
          addr[j] = addr[j+1];
          length[j] = length[j+1];
        }
        noOfBlocks--;
        cout << "Address " << a << " deallocated" << endl;
        return;
    }
  }
  cout << "Address not allocated." << endl;
}

long freeMem() {
  long total = getMem();
  for (int i = 0; i < noOfBlocks; i++) {
    //subtract the length of all allocated memory
    total = total - length[i];
  }
  return total;
}

void storeChar(long a, string data){
  storeByte(data[0], a);
}

void storeInt(long a, string data) {
  int i = stoi(data);
  storeByte((i & 0xFF00) >> 8, a);
  storeByte(i & 0xFF, a + 1);
}

void storeFloat(long a, string data) {
  float number = stof(data);
  unsigned char sign = 0;
  if (number < 0) {
    number = -number;
    sign = 128;
  }
  long integerPart = number;
  number -= integerPart;
  long fractionPart = 0;
  for (int i = 22; i >= 0; i--) {
    number *= 2;
    if (number >= 1) {
      fractionPart += (1l << i);
      number -= 1;
    }
  }
  unsigned char exponent = 127;
  while (integerPart != 1) {
    if (integerPart > 1) {
      fractionPart = ((integerPart & 1) << 22) + (fractionPart >> 1);
      integerPart = integerPart >> 1;
      exponent++;
    } else {
      integerPart = fractionPart >> 22;
      fractionPart = (fractionPart & 0x3FFFFF) << 1;
      number *= 2;
      if (number > 1) {
        fractionPart++;
        number -= 1;
      }
      exponent--;
    }
  }
  unsigned char b = sign + (exponent >> 1);
  storeByte(b, a);
  b = fractionPart >> 16;
  if (exponent & 1) b += 128;
  storeByte(b, a + 1);
  b = (fractionPart & 0xFF00) >> 8;
  storeByte(b, a + 2);
  b = fractionPart & 0xFF;
  storeByte(b, a + 3);
}

void storeString(long a, string data) {
  int i;
  for (i = 0; i < data.length(); i++) {
    if (data[i] == '\\' && i + 1 < data.length()) {
      a--;
      if (data[++i] == 'n') {
        storeByte('\n', a + i);
        continue;
      }
    }
    storeByte(data[i], a + i);
  }
  storeByte(0, a + i);
}

void store(long a, string type, string data) {
  long size;
  if (type == "CHAR") size = 1;
  if (type == "INT") size = 2;
  if (type == "FLOAT") size = 4;
  if (type == "STRING") size = 1;

  bool fits = false;
  for (int i = 0; i < noOfBlocks; i++) {
    if (a >= addr[i] && a + size <= addr[i] + length[i]) {
      fits = true;
      break;
    }
  }
  if (!fits) {
    cout << "Variable does not fit in allocated memory." << endl;
    return;
  }
  if (type == "CHAR") {
    storeChar(a, data);
  }
  if (type == "INT") {
    storeInt(a, data);
  }
  if (type == "FLOAT") {
    storeFloat(a, data);
  }
  if (type == "STRING") {
    storeString(a, data);
  }
}

string retrieveChar(long a) {
  string str(1, static_cast<char>(recallByte(a)));
  return str;
}

string retrieveInt(long a) {
    int b1 = recallByte(a);
    int b2 = recallByte(a + 1);
    int i;
    i = ((b1 << 8) | b2);
    return to_string(i);
}

string retrieveFloat(long a) {
  float number = float(recallByte(a));

  return to_string(number);
}

string retrieveString(long a) {
  string ret;
  char c;
  while ((c = (char)recallByte(a++)) != '\0') {
    ret = ret + c;

  }
  return ret;
}

string retrieve(long a, string type) {
  long size;
  if (type == "CHAR" || type == "STRING") size = 1;
  if (type == "INT") size = 2;
  if (type == "FLOAT") size = 4;
  bool addressOK = false;
  for (int i = 0; i < noOfBlocks; i++) {
    if (a >= addr[i] && a + size <= addr[i] + length[i]) {
      addressOK = true;
      break;
    }
  }
  if (!addressOK) {
    return string("Address not in allocated memory.");
  }
  if (type == "CHAR") {
    return retrieveChar(a);
  }
  if (type == "INT") {
    return retrieveInt(a);
  }
  if (type == "FLOAT") {
    return retrieveFloat(a);
  }
  if (type == "STRING") {
    return retrieveString(a);
  }
  return string();
}

int main() {
  string command;
  cout << "Please enter your command  (type HELP for help)" << endl;
  while (true) {
    command = readLine();
    if (command == "ALLOC") {
	  cout << "Please enter the size" << endl;
      long size = stoi(readLine());
	  cout << "Memory id: ";
      allocate(size);
    }
    if (command == "DEALLOC") {
	  cout << "Please enter the address" << endl;
      long address = stoi(readLine());
      deallocate(address);
    }
    if (command == "FREEMEM") {
      cout << freeMem() << endl;
    }
    if (command == "SET") {
	  cout << "Please enter the memory id" << endl;
      long i = stoi(readLine());
	  cout << "Please enter the data type" << endl;
	  cout << "CHAR | INT | FLOAT | STRING" << endl;
      string type = readLine();
	  cout << "Please enter the data" << endl;
      string data = readLine();
      store(i, type, data);
	  cout << "All done!" << endl;
    }
    if (command == "GET") {
	  cout << "Please enter the memory id" << endl;
      long address = stoi(readLine());
	  cout << "Please enter the data type" << endl;
	  cout << "CHAR | INT | FLOAT | STRING" << endl;
      string type = readLine();
      cout << retrieve(address, type) << endl;
    }
	if (command == "HELP") {
        cout << "--------------" << endl;
		cout << "ALLOC" << endl;
		cout << "DEALLOC" << endl;
		cout << "FREEMEM" << endl;
		cout << "SET" << endl;
		cout << "GET" << endl;
		cout << "--------------" << endl;
      }
  }
}
