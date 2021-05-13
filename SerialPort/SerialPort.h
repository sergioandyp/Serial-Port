#ifndef _SERIAL_H_
#define _SERIAL_H

#include <boost/asio.hpp>
#include <string>
#include <sstream>

#define DEFAULT_BAUD_RATE	115200


// Clase SerialPort para facilitar el uso de puerto serie en base a la libreria asio
class SerialPort {
public:

	SerialPort();
	
	// Crea el objeto e inicia una comunicacion con el puerto y baud_rate especificado
	SerialPort(std::string port, unsigned int baudRate = DEFAULT_BAUD_RATE);

	~SerialPort();

	// Inicia una comunicación en el puerto y baud_rate especificado
	// Devuelve 1 si ocurrio un error
	bool initPort(std::string port, unsigned int baudRate = DEFAULT_BAUD_RATE);
	
	// Se desconecta del puerto si estaba conectado
	// Devuelve 1 si ocurrio un error o no estaba conectado
	bool disconnect();

	// Se conigura el baudRate especificado, para eso debe haber una conexion iniciada
	// Devuelve 1 si hubo error
	bool setBaudRate(unsigned int baudRate);

	// Comienza a leer del puerto serie
	void startReading();
	
	// Deja de leer del puerto serie
	void stopReading();

	// Esta funcion se debe llamar continuamente para realizar las
	// acciones de lectura/escritura asincrónicas
	void runOperations();

	// Devuelve si hay contenido para leer
	bool isNewData();

	// Devuelve si el contenido no leido tiene un fin de linea
	bool isNewLine();

	// Devuelve toda el contenido recibido que no ha sido leido
	std::string getData();

	// Devuelve el contenido recivido hasta el primer salto de linea
	// Si no hay salto de linea, devuelve string vacio y el contenido no leido
	// se mantiene en el buffer
	std::string getLine();

	// Escribe al puerto de forma asincronica
	// Cuando writeDone devuelva true, la escritura se completo correctamente
	void write(std::string w_data);

	// Escribe al puerto de forma sincronica.
	// Devuelve true si hubo error
	bool writeSync(std::string w_data);
	
	// Devuelve true si la escritura se completo correctamente
	bool writeDone();

private:
	boost::asio::io_service io_service;
	boost::asio::serial_port serial;
	boost::system::error_code error;
	
	char readByte;
	std::string data;

	bool isReading;

	bool writeOK;

	bool setDefaults();

	void requestByteRead();
	void readByteCb(const boost::system::error_code& error, std::size_t size);
	
	void writeCb(const boost::system::error_code& error, std::size_t size);


};

#endif