#include <cstdio>
#include <iostream>

#include <QTextCodec>

#include "IO.h"

QTextStream* IO::m_stderr = 0;

IO::IO(Settings *settings): m_inputFile(0), m_outputFile(0)
{
	m_standardInput = new QTextStream(stdin, QIODevice::ReadOnly);
	
	if(m_stderr == 0) {
		m_stderr = new QTextStream(stderr, QIODevice::WriteOnly);
	}
	
	if(settings->inputFile().size() > 0) {
		m_inputFile = new QFile(settings->inputFile().c_str());
		m_inputFile->open(QIODevice::ReadOnly | QIODevice::Text);
		m_input = new QTextStream(m_inputFile);
	} else {
		m_input = m_standardInput;
	}
	
	if(settings->outputFile().size() > 0) {
		m_outputFile = new QFile(settings->outputFile().c_str());
		m_outputFile->open(QIODevice::WriteOnly | QIODevice::Text);
		m_output = new QTextStream(m_outputFile);
	} else {
		m_output = new QTextStream(stdout, QIODevice::WriteOnly);
	}
	
	m_input->setCodec(QTextCodec::codecForName("UTF-8"));
	m_output->setCodec(m_input->codec());
}

IO::~IO() {
	delete m_input;
	delete m_output;
	delete m_standardInput;
	delete m_stderr;
	
	delete m_inputFile;
	delete m_outputFile;
}

void IO::writeVerboseMessage(const std::string &message) {
	if(Settings::getInstance()->verbose()) {
		writeToScreen(message);
	}
}

void IO::writeToScreen(const std::string &message) {
	(*m_stderr)<<message.c_str()<<'\n';
	m_stderr->flush();
}

void IO::writeToScreen(const QString &message) {
	(*m_stderr)<<message<<'\n';
	m_stderr->flush();
}

void IO::writeToScreen(const char *message) {
	writeToScreen(std::string(message));
}

void IO::inputChar(QChar &c) {
	QString str;
	standardInput()>>str;
	c = str[0];
}
