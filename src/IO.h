#ifndef IO_H
#define IO_H

#include <string>

#include <boost/lexical_cast.hpp>

#include <QFile>
#include <QTextStream>

#include "Settings.h"

// Класс для работы с вводом/выводом
class IO
{
public:
    IO(Settings* settings);
	~IO();
	
	QTextStream& input() { return *m_input; }
	QTextStream& output() { return *m_output; }
	QTextStream& standardInput() { return *m_standardInput; }

	static void writeVerboseMessage(const std::string& message);
	static void writeToScreen(const std::string& message);
	static void writeToScreen(const QString& message);
	static void writeToScreen(const char* message);
	
	void inputChar(QChar& c);
private:
	QTextStream* m_input;
	QTextStream* m_output;
	QTextStream* m_standardInput;
	static QTextStream* m_stderr;
	
	QFile* m_inputFile;
	QFile* m_outputFile;
};

template<class T>
inline std::string str(const T& t) {
	return boost::lexical_cast<std::string>(t);
}

#endif // IO_H
