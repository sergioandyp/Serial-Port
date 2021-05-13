#ifndef _BASIC_LCD_
#define _BASIC_LCD_ 

#include <string>

struct cursorPosition
{
	int row;
	int column;
};

class lcdError
{
public:
	lcdError(std::string name, std::string desc, unsigned long code);
	std::string getErrorName();
	std::string getErrorDescription();
	unsigned long getErrorCode();

private:
	std::string name;
	std::string description;
	unsigned long code;
};


class basicLCD
{
public:

	/*=====================================================
	* Name: basicLCD
	* Entra: -
	* Resulta: Constructor de la clase. Inicializa el LCD y deja
	* todo listo comenzar a utilizarlo.
	*
	* cadd =1 (cursor address) (ver NOTA 1)
	*=====================================================*/
	basicLCD();

	/*=====================================================
	* Name: ~basicLCD
	* Entra: -
	* Resulta: Destructor de la clase. Libera cualquier recurso
	* que se hubiera tomado de forma de evitar
	* "resources leak".
	*=====================================================*/
	virtual ~basicLCD();

	/*=====================================================
	* Name: lcdInitOk
	* Entra: -
	* Resulta: No genera ning�n cambio en el display.
	* Devuelve en su nombre �true� si el display se inicializ�
	* correctamente (el constructor no tuvo errores) o �false
	* en caso contrario.
	*=====================================================*/
	virtual bool lcdInitOk() = 0;

	/*=====================================================
	* Name: lcdGetError
	* Entra: -
	* Resulta: No genera ning�n cambio en el display.
	* Devuelve en su nombre un lcdError&
	*=====================================================*/
	virtual lcdError& lcdGetError() = 0;

	/*=====================================================
	* Name: lcdClear
	* Entra: -
	* Resulta: Borra el display y el cursor va a HOME
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	virtual bool lcdClear() = 0;

	/*=====================================================
	* Name: lcdClearToEOL
	* Entra: -
	* Resulta: Borra el display desde la posici�n actual
	* del cursor hasta el final de la l�nea.
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	virtual bool lcdClearToEOL() = 0;

	/*=====================================================
	* Name: operator<<()
	* Entra: Un car�cter
	* Resulta: Pone el car�cter en la posici�n actual
	* del cursor del display y avanza el cursor a la pr�xima
	* posici�n respetando el gap (si el car�cter no es imprimible
	* lo ignora)
	*
	* Devuelve en su nombre una referencia a un basicLCD que permite
	* encascar la funci�n:
	* basicLCD lcd;
	* lcd << �a� << �b� << �c�;
	*=====================================================*/
	virtual basicLCD& operator<<(const char c) = 0;

	/*=====================================================
	* Name: operator<<()
	* Entra: Una cadena de caracteres NULL terminated
	* Resulta: imprime la cadena de caracteres en la posici�n actual
	* del cursor y avanza el cursor al final de la cadena respetando
	* el gap (si alg�n car�cter no es imprimible lo ignora). Si recibe una
	* cadena de m�s de 32 caracteres, muestra los �ltimos 32 en el display.
	*
	* Devuelve en su nombre una referencia a un basicLCD que permite
	* encascar la funci�n:
	* basicLCD lcd;
	* lcd << �Hola� << � � << �Mundo�;
	*=====================================================*/
	virtual basicLCD& operator<<(const char* c) = 0;

	/*=====================================================
	* Name: lcdMoveCursorUp
	*
	* Entra: -
	* Resulta: Pasa el cursor a la primera l�nea del display sin
	* alterar la columna en la que estaba.
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	virtual bool lcdMoveCursorUp() = 0;

	/*=====================================================
	* Name: lcdMoveCursorDown
	*
	* Entra: -
	* Resulta: Pasa el cursor a la segunda l�nea del display sin
	* alterar la columna en la que estaba.
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	virtual bool lcdMoveCursorDown() = 0;

	/*=====================================================
	* Name: lcdMoveCursorRight
	*
	* Entra: -
	* Resulta: Avanza el cursor una posici�n
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	virtual bool lcdMoveCursorRight() = 0;

	/*=====================================================
	* Name: lcdMoveCursorLeft
	*
	* Entra: -
	* Resulta: Retrocede el cursor una posici�n
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	virtual bool lcdMoveCursorLeft() = 0;

	/*=====================================================
	* Name: lcdSetCursorPosition
	* Entra: Recibe una estructura tipo cursorPosition
	* Resulta: Posiciona el cursor en la posici�n dada
	* por row y column. row[0-1] col[0-15]. Ante un valor inv�lido
	* de row y/o column ignora la instrucci�n (no hace nada).
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	virtual bool lcdSetCursorPosition(const cursorPosition pos) = 0;

	/*=====================================================
	* Name: lcdGetCursorPosition
	* Entra: -
	* Resulta: Devuelve la posici�n actual del cursor.
	*
	*
	* Devuelve una estructura tipo cursorPosition
	*=====================================================*/
	virtual cursorPosition lcdGetCursorPosition() = 0;

};

#endif // _BASIC_LCD_