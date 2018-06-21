#include <stdlib.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <wiringPi.h>

using namespace std;

#define PIN 0
#define INTERVAL 100

void wait(int i) {
	// Reset LED to low
	digitalWrite(PIN, LOW);
	// Interval times i for scaling
	delay(INTERVAL * i);
}

void dot() {
	// Reset LED to low
	digitalWrite(PIN, LOW);
	digitalWrite(PIN, HIGH);
	// Interval times 1 for scaling
	delay(INTERVAL * 1);
	digitalWrite(PIN, LOW);
	// Wait 1 times the interval
	wait(1);
}


void dash() {
	// Reset LED to low
	digitalWrite(PIN, LOW);
	digitalWrite(PIN, HIGH);
	// Interval times 1 for scaling
	delay(INTERVAL * 3);
	digitalWrite(PIN, LOW);
	// Wait 1 times the interval
	wait(1);
}

void morse(char c) {
	switch (c)
	{
	case 'a':
		dot();
		dash();
		break;
	case 'b':
		dash();
		dot();
		dot();
		dot();
		break;
	case 'c':
		dash();
		dot();
		dash();
		dot();
		break;
	case 'd':
		dash();
		dot();
		dot();
		break;
	case 'e':
		dot();
		break;
	case 'f':
		dot();
		dot();
		dash();
		dot();
		break;
	case 'g':
		dash();
		dash();
		dot();
		break;
	case 'h':
		dot();
		dot();
		dot();
		dot();
		break;
	case 'i':
		dot();
		dot();
		break;
	case 'j':
		dot();
		dash();
		dash();
		dash();
		break;
	case 'k':
		dash();
		dot();
		dash();
		break;
	case 'l':
		dot();
		dash();
		dot();
		dot();
		break;
	case 'm':
		dash();
		dash();
		break;
	case 'n':
		dash();
		dot();
		break;
	case 'o':
		dash();
		dash();
		dash();
		break;
	case 'p':
		dot();
		dash();
		dash();
		dot();
		break;
	case 'q':
		dash();
		dash();
		dot();
		dash();
		break;
	case 'r':
		dot();
		dash();
		dot();
		break;
	case 's':
		dot();
		dot();
		dot();
		break;
	case 't':
		dash();
		break;
	case 'u':
		dot();
		dot();
		dash();
		break;
	case 'v':
		dot();
		dot();
		dot();
		dash();
		break;
	case 'w':
		dot();
		dash();
		dash();
		break;
	case 'x':
		dash();
		dot();
		dot();
		dash();
		break;
	case 'y':
		dash();
		dot();
		dash();
		dash();
		break;
	case 'z':
		dash();
		dash();
		dot();
		dot();
		break;
	case '0':
		dash();
		dash();
		dash();
		dash();
		dash();
		break;
	case '1':
		dot();
		dash();
		dash();
		dash();
		dash();
		break;
	case '2':
		dot();
		dot();
		dash();
		dash();
		dash();
		break;
	case '3':
		dot();
		dot();
		dot();
		dash();
		dash();
		break;
	case '4':
		dot();
		dot();
		dot();
		dot();
		dash();
		break;
	case '5':
		dot();
		dot();
		dot();
		dot();
		dot();
		break;
	case '6':
		dash();
		dot();
		dot();
		dot();
		dot();
		break;
	case '7':
		dash();
		dash();
		dot();
		dot();
		dot();
		break;
	case '8':
		dash();
		dash();
		dash();
		dot();
		dot();
		break;
	case '9':
		dash();
		dash();
		dash();
		dash();
		dot();
		break;
	case '.':
		dot();
		dash();
		dot();
		dash();
		dot();
		dash();
		break;
	case ',':
		dash();
		dash();
		dot();
		dot();
		dash();
		dash();
		break;
	case '?':
		dot();
		dot();
		dash();
		dash();
		dot();
		dot();
		break;
	case '\'':
		dot();
		dash();
		dash();
		dash();
		dash();
		dot();
		break;
	case '!':
		dash();
		dash();
		dash();
		dot();
		dot();
		dot();
		break;
	case '/':
		dash();
		dot();
		dot();
		dash();
		dot();
		break;
	case '(':
		dash();
		dot();
		dash();
		dash();
		dot();
		break;
	case ')':
		dash();
		dot();
		dash();
		dash();
		dot();
		dash();
		break;
	case '&':
		dot();
		dash();
		dot();
		dot();
		dot();
		break;
	case ':':
		dash();
		dash();
		dash();
		dot();
		dot();
		dot();
		break;
	case ';':
		dash();
		dot();
		dash();
		dot();
		dash();
		dot();
		break;
	case '=':
		dash();
		dot();
		dot();
		dot();
		dash();
		break;
	case '+':
		dot();
		dash();
		dot();
		dash();
		dot();
		break;
	case '-':
		dash();
		dot();
		dot();
		dot();
		dot();
		dash();
		break;
	case '"':
		dot();
		dash();
		dot();
		dot();
		dash();
		dot();
		break;
	case '@':
		dot();
		dash();
		dash();
		dot();
		dash();
		dot();
		break;
	case ' ':
		wait(4);
		break;
	default:
		break;
	}
}

int main() {
	// Setup wiring pi
	if (wiringPiSetup() < 0) {
		cout << "Unable to setup wiring pi.";
		return 1;
	}
	// Set the pin mode for the LED to output
	pinMode(PIN, OUTPUT);

	while (1) {
		// Wait for user input
		cout << "Please enter text:" << endl;
		// Read line for spacebars
		std::string line;
		std::getline(std::cin, line);
		// Transform user input to uppercase
		std::transform(line.begin(), line.end(), line.begin(), ::tolower);
		// Exit on exit
		if (line == "exit") {
			break;
		}
		// Iterate through the input for sending morse code
		for (int i = 0; i < line.length(); i++) {
			// Send morse of that character
			morse(line[i]);
			// Wait 3 pulses
			wait(3);
		}
	}
	// Return
	return 0;
}
