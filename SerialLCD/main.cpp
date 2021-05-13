#include <iostream>
#include <string>
#include "SerialLCD.h"

using namespace std;

int main() {

	string port;
	cin >> port;

	SerialLCD lcd(port);

	if (!lcd.lcdInitOk()) {
		cout << "Error al inicializar LCD" << endl;
		return 1;
	}

	string text;


	while (text != "q") {	// quit
		
		cin >> text;
	
		if (text == "u") {		// up
			lcd.lcdMoveCursorUp();
		}
		else if (text == "d") {		// down
			lcd.lcdMoveCursorDown();
		}
		else if (text == "l") {		// left
			lcd.lcdMoveCursorLeft();
		}
		else if (text == "r") {		// right
			lcd.lcdMoveCursorRight();
		}
		else {			// Cualquier otra cosa, imprimo al display
			lcd << text.c_str();
		}

	}


	return 0;
}