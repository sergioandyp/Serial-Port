#include <iostream>
#include <string>
#include "SerialPort.h"

using namespace std;

int readTest(string portName);
int writeTest(string portName);

int main() {


    cout << "Nombre del puerto: ";

    string port;
    cin >> port;

    //return readTest(port);
    return writeTest(port);


}


int readTest(string portName) {
    
    try {

        SerialPort port(portName);

        port.startReading();

        string data;

        while (true) {

            while (!port.isNewLine()) {
                port.runOperations();
            }

            data = port.getLine();

            cout << data << endl;
        }

        port.disconnect();
    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

int writeTest(string portName) {
    try {

        SerialPort port(portName);

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

    return 0;
}