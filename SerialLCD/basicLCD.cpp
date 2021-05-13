#include "basicLCD.h"

using namespace std;

lcdError::lcdError(string name, string desc, unsigned long code) 
			: name(name), description(desc), code(code) 
{
}

string lcdError::getErrorName() {
	return name;
}

string lcdError::getErrorDescription() {
	return description;
}

unsigned long lcdError::getErrorCode() {
	return code;
}

basicLCD::basicLCD() {

}

basicLCD::~basicLCD() {

}