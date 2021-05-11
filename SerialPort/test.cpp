#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/basic_serial_port.hpp>
#include <boost/bind/bind.hpp>

#include <iostream>
#include <string>

using namespace std;

void read_cb(char c);

int main() {
    try {
        boost::asio::io_service io_service;

        boost::asio::serial_port port(io_service, "COM7");
        //boost::asio::serial_port port(io_service);
        //port.open("COM7");

        port.set_option(boost::asio::serial_port::baud_rate(9600));
        
        // Esto reinicia el Arduino
        // software parece que tambien anda
        port.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::hardware));

        char c;

        //cout << "Espero..." << endl;

        //Sleep(5000);

        //port.async_read_some(boost::asio::buffer(c), boost::bind(read_cb));

        cout << "Recibo" << endl;

        //boost::asio::read(port, boost::asio::buffer(&c, 1));

        string result;

        int count = 0;
        bool stop = false;
        while (!stop) {
            boost::asio::read(port, boost::asio::buffer(&c, 1));
            //port.read_some(boost::asio::buffer(&c, 1));
            //cout << "leo " << c << endl;
            switch (c)
            {
            case '\r':
                break;
            case '\n':
                cout << result << endl;
                result = "";
                //count++;
                if (count >= 10)
                    stop = true;
                break;
            default:
                result += c;
            }
        }


        cout << "resultado: " << result << endl;

        port.close();
    }
    catch (boost::system::system_error& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}

void read_cb(char c) {
    cout << "OK" << endl;

}
//
// 
// 
// 
// 
//#include <iostream>
//#include "SerialPort.h"
//
//using namespace std;
//using namespace boost;
//
//int main(int argc, char* argv[])
//{
//    try {
//
//        SimpleSerial serial("COM7", 9600);
//
//        //serial.writeString("Hello world\n");
//
//        cout << "Espero..." << endl;
//
//        Sleep(5000);
//
//        cout << "Recibo" << endl;
//
//        cout << serial.readLine() << endl;
//
//    }
//    catch (boost::system::system_error& e)
//    {
//        cout << "Error: " << e.what() << endl;
//        return 1;
//    }
//}