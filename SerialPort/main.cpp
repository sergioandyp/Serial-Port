#include <iostream>
#include <string>
#include "SerialPort.h"

using namespace std;

int readTest();
int writeTest();

int main() {

    //readTest();
    return writeTest();


}


int readTest() {
    
    try {

        SerialPort port("COM5");

        cout << "Recibo" << endl;

        port.startReading();

        string data;

        while (true) {

            while (!port.isNewLine()) {
                port.runOperations();
            }

            data = port.getLine();

            cout << data << endl;

            Sleep(rand() % 5000);
        }

        port.disconnect();
    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}

int writeTest() {
    try {

        SerialPort port("COM7");

        cout << "Envio" << endl;

        port.startReading();

        string data;


        while (data != "q") {

            cin >> data;

            port.write(data);

            while (!port.writeDone()) {
                port.runOperations();
            }
        }

        port.disconnect();
    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}