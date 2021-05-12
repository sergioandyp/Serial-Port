#include <iostream>
#include <string>
#include "SerialPort.h"

using namespace std;


int main() {

    try {
        
        SerialPort port("COM7");

        cout << "Recibo" << endl;

        port.startReading();

        string data;

        while (true) {

            while (!port.isNewLine()) {
                    port.runOperations();
            }
            
            data = port.getLine();

            cout << data << endl;
            
            //Sleep(rand()%5000);
        }

        port.disconnect();
    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

}