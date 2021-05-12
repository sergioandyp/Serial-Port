#include "SerialPort.h"

#include <boost/bind/bind.hpp>

#define NEW_LINE_CHAR   '\n'

using namespace std;

SerialPort::SerialPort() : io_service(), serial(io_service), readByte(), isReading(false) //, isData(false)
{
}

SerialPort::SerialPort(string port, unsigned int baudRate) : 
    io_service(), serial(io_service, port), readByte(), isReading(false) //, isData(false) 
{
    setBaudRate(baudRate);
    setDefaults();
}

SerialPort::~SerialPort() {
    serial.close();
}

bool SerialPort::initPort(string port, unsigned int baudRate) {

    if (serial.is_open()) return 1;     // El puerto se encuentra abierto, no hago nada

    serial.open(port, error);

    return error.failed() || setBaudRate(baudRate) || setDefaults();    // Si alguna devuelve 1 hubo error
}

bool SerialPort::setBaudRate(unsigned int baudRate) {
    
    if (!serial.is_open()) return 1;    // Debe haber una conexion abierta para poder configurarla

    serial.set_option(boost::asio::serial_port::baud_rate(baudRate), error);
    return error.failed();
}

bool SerialPort::setDefaults() {

    if (!serial.is_open()) return 1;    // Debe haber una conexion abierta para poder configurarla

    // Esto reinicia el Arduino
    // software parece que tambien anda
    serial.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::hardware), error);
    
    /// TODO: VER ESTO:
    //port_->set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    //port_->set_option(boost::asio::serial_port_base::character_size(8));
    //port_->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    //port_->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    //port_->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
    
    
    
    return error.failed();
}

bool SerialPort::disconnect() {

    if (!serial.is_open()) return 1;    // Debe haber una conexion abierta para poder cerrarla

    serial.close(error);
    return error.failed();
}

void SerialPort::startReading() {

    if (!serial.is_open()) return;    // Debe haber una conexion abierta para poder leer
    
    isReading = true;

    //isData = false;

    requestByteRead();
    //requestBuffRead();
}

void SerialPort::stopReading() {
    isReading = false;
}

void SerialPort::requestByteRead() {

    serial.async_read_some(boost::asio::buffer(&readByte, 1),       // Empezamos a leer de forma asincrónica
        boost::bind(&SerialPort::readByteCb, this, 
            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

}

void SerialPort::readByteCb(const boost::system::error_code& error, std::size_t size) {

    if (serial.is_open() && isReading) {
        if (size > 0) data += readByte;     // Agregamos el nuevo byte al string
        //isData = true;
        requestByteRead();
    }
}

//
//void SerialPort::requestBuffRead() {
//
//    //serial.async_read_some(buff,       // Empezamos a leer de forma asincrónica
//    //    boost::bind(&SerialPort::readBufferCb, this,
//    //        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
//
//
//    // Esta mal, streambuf no se usa asi
//
//
//    //async_read_some(serial, buff,       // Empezamos a leer de forma asincrónica
//    //    boost::bind(&SerialPort::readBufferCb, this,
//    //        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
//
//}
//
//
//void SerialPort::readBufferCb(const boost::system::error_code& error, std::size_t size) {
//
//    if (serial.is_open() && isReading) {
//        //isData = true;
//        requestBuffRead();
//    }
//}

string SerialPort::getData() {

    string temp = data;

    data.clear();   // Borramos data para hacer lugar

    return temp;

    //istream is(&buff);

    //getline(is, data, {});  // Guardo toda lo recibido en data

    //isData = false;

    //return data;
}

string SerialPort::getLine() {

    size_t pos = data.find_first_of(NEW_LINE_CHAR);

    string temp;
    
    if (pos != data.npos) { // Solo devolvemos si encontramos un salto de linea
        temp = data.substr(0, pos);  // Guardamos el valor a devolver (sin el /n)
        data.erase(0, pos+1); // Borramos data para hacer lugar (incluyedo /n)
    }

    return temp;

}

void SerialPort::runOperations() {
    io_service.run_one();
}

bool SerialPort::isNewData() {
    return !data.empty();   // Si data no esta vacio, hay contenido para leer
}

bool SerialPort::isNewLine() {
    return data.find_first_of(NEW_LINE_CHAR) != data.npos;   // Si data no esta vacio, hay contenido para leer
}