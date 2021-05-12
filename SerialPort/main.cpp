#include <iostream>
#include <string>
#include "SerialPort.h"

using namespace std;

int readTest(string portName);
int writeTest(string portName);

int main() {

    string port;

    cin >> port;

    return readTest(port);
    //return writeTest(port);


}


int readTest(string portName) {
    
    try {

        SerialPort port(portName);

        cout << "Recibo" << endl;

        port.startReading();

        string data;

        while (true) {

            while (!port.isNewLine()) {
                port.runOperations();
            }

            data = port.getLine();

            cout << data << endl;

            //Sleep(rand() % 5000);
        }

        port.disconnect();
    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}

int writeTest(string portName) {
    try {

        SerialPort port(portName);

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