#include "SerialPort.h"

#include <boost/bind/bind.hpp>

#define NEW_LINE_CHAR   '\n'

using namespace std;

SerialPort::SerialPort() : 
    io_service(), serial(io_service), error(), data(), readByte(), isReading(false), writeOK(false)
{
}

SerialPort::SerialPort(string port, unsigned int baudRate) : 
    io_service(), serial(io_service, port), readByte(), error(), data(), isReading(false), writeOK(false)
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

    // Configuraciones necesarias
    serial.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none), error);    
    serial.set_option(boost::asio::serial_port_base::character_size(8));
    serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    
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

    requestByteRead();
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
        requestByteRead();
    }
}


string SerialPort::getData() {

    string temp = data;

    data.clear();   // Borramos data para hacer lugar

    return temp;

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

void SerialPort::write(std::string w_data) {
    if (serial.is_open()) {
        writeOK = false;
        
        serial.async_write_some(boost::asio::buffer(w_data.c_str(), w_data.length()), 
            boost::bind(&SerialPort::writeCb, this, 
                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
}

bool SerialPort::writeSync(string w_data) {
    if (serial.is_open()) {
        serial.write_some(boost::asio::buffer(w_data.c_str(), w_data.length()), error);

        return error.failed();
    }
    return true;
}


bool SerialPort::writeDone() {
    return writeOK;
}

void SerialPort::writeCb(const boost::system::error_code& error, size_t size) {
        writeOK = true;
}
