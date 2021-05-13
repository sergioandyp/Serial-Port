#include "SerialLCD.h"

//#define DEBUG

#define MAX_ROW	(LCD_ROW_COUNT-1)
#define MAX_COL	(LCD_COL_COUNT-1)

#define TOTAL_CHARS	(LCD_COL_COUNT*LCD_ROW_COUNT)

// Se considera caracter imprimible
#define IN_RANGE(c)		((c) >= 32 && (c) <= 126)

using namespace std;

SerialLCD::SerialLCD(string portName, unsigned int baudRate) :
	cursor{ 0, 0 }, error("", "", E_OK), text(' ', TOTAL_CHARS), 	// Construyo text lleno de espacios 
	port()
{

	if (!port.initPort(portName, baudRate)) {				// inicializacion OK
		print();
	}
	else {
		setError(E_INIT);
	}


}

SerialLCD::~SerialLCD() {

	// Antes de destruir todo, desconectamos
	port.disconnect();

}

bool SerialLCD::lcdInitOk() {
	return error.getErrorCode() != E_INIT;
}

lcdError& SerialLCD::lcdGetError() {
	return error;
}

bool SerialLCD::lcdClear() {

	text = string(' ', TOTAL_CHARS);

	cursor = { 0, 0 };
	print();
	return true;
}

bool SerialLCD::lcdClearToEOL() {

	size_t len = getLengthToEOL();

	text.replace(cursor.row * LCD_COL_COUNT + cursor.column, len, len, ' ');

	print();

	return true;
}

basicLCD& SerialLCD::operator<<(const char c) {
	if (IN_RANGE(c)) {
		text[cursor.row * LCD_COL_COUNT + cursor.column] = c;
		lcdMoveCursorRight();
		print();
	}
	return *this;
}

basicLCD& SerialLCD::operator<<(const char* c) {

	string s(c);

	for (int i = 0; i < s.size(); i++) {	// Borro caracteres invalidos
		if (!IN_RANGE(s[i])) {
			s.erase(i, 1);
			i--;
		}
	}

	int len = getLengthToEnd();
	if (s.size() > len) {
		s = s.substr(s.size() - len, len);
	}
	text.replace(cursor.row * LCD_COL_COUNT + cursor.column, s.size(), s);	// Reemplazo
	moveCursorOffset(s.size());
	print();
	return *this;
}

bool SerialLCD::lcdMoveCursorDown() {

	bool ret = moveCursorOffset(LCD_COL_COUNT);		// Le sumo al cursor la cantidad de columnas
	print();
	return ret;
}

bool SerialLCD::lcdMoveCursorUp() {

	bool ret = moveCursorOffset(-1 * LCD_COL_COUNT);
	print();
	return ret;

}

bool SerialLCD::lcdMoveCursorRight() {

	bool ret = moveCursorOffset(+1);
	print();
	return ret;
}

bool SerialLCD::lcdMoveCursorLeft() {

	bool ret = moveCursorOffset(-1);
	print();
	return ret;

}

bool SerialLCD::lcdSetCursorPosition(const cursorPosition pos) {
	if (pos.column < 0 || pos.column > MAX_COL
		|| pos.row < 0 || pos.row > MAX_ROW) {		// error si pos fuera de rango
		setError(E_POS);
		return false;
	}
	cursor = pos;
	print();
	return true;
}

cursorPosition SerialLCD::lcdGetCursorPosition() {
	return cursor;
}


// Private Methods

void SerialLCD::print() {

	// Enviamos el string al port

	port.write(text);	
	// Escritura "sincronica"
	while (!port.writeDone()) {		// Peligroso, puede colgar el programa aca
		port.runOperations();
	}


	// DEBUG:
#ifdef DEBUG
	cout << "\"" << text << "\"" << endl;
	cout << "\"" << text.substr(0, 16) << "\"" << endl;
	cout << "\"" << text.substr(16, 16) << "\"" << endl << endl;
#endif

}

int SerialLCD::getLengthToEOL() {
	return LCD_COL_COUNT - cursor.column;
}

int SerialLCD::getLengthToEnd() {
	return getLengthToEOL() + LCD_COL_COUNT * (MAX_ROW - cursor.row);
}


bool SerialLCD::moveCursorOffset(int offset) {
	cursor.column += offset;

	if (cursor.column > MAX_COL) {								// Se pasa de los limites de columna
		cursor.row += (int)(cursor.column / LCD_COL_COUNT);		// Sumo diferencia de filas
		cursor.column = cursor.column % LCD_COL_COUNT;			// Sumo resto de columnas
	}
	else if (cursor.column < 0) {
		cursor.row += (int)((cursor.column + 1) / LCD_COL_COUNT) - 1;		// Resto diferencia de filas
		cursor.column += LCD_COL_COUNT * (1 - (int)((cursor.column + 1) / LCD_COL_COUNT));
	}

	int newRow = min(cursor.row, MAX_ROW);
	newRow = max(0, newRow);

	if (cursor.row != newRow) {		// Se supero un limite maximo/minimo
		setError(E_POS);
		if (newRow == MAX_ROW) {
			cursor = { MAX_ROW, MAX_COL };
		}
		else {
			cursor = { 0, 0 };
		}
		return false;
	}

	return true;
}

void SerialLCD::setError(errorCode error) {
	switch (error)
	{
	case E_OK:
		this->error = lcdError("No error", "All is OK", E_OK);
		break;
	case E_INIT:
		this->error = lcdError("Initialization error", "Could not load resources", E_INIT);
		break;
	case E_POS:
		this->error = lcdError("Incorrect position",
			"Cursor reached a limit or position provided was incorrect",
			E_POS);;
		break;
	default:
		break;
	}
}