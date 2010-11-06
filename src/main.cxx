#include <cstdio>
#include <iostream>

#include <boost/program_options.hpp>

#include <QString>
#include <QTextStream>
#include <QTextCodec>

#include "Settings.h"
#include "Exceptions.h"
#include "IO.h"
#include "Statistics.h"
#include "AlphabetPermutation.h"
#include "Analyzer.h"
#include "Exceptions.h"

void readStatistics(IO& io, Statistics& statistics) {
	if(io.input().status() != QTextStream::Ok) {
		throw UserError("Couldn't open input file for reading");
	}
	
	while(io.input().status() == QTextStream::Ok) {
		QChar c;
		io.input() >> c;
		statistics.analyzeNextLetter(c);
	}
	statistics.calculateStatistics();
}

void decrypt(const AlphabetPermutation& key, const QString& encryptedText, QString& decryptedText) {
	decryptedText.clear();
	decryptedText.reserve(encryptedText.length());
	for(int i = 0; i < encryptedText.length(); i++) {
		decryptedText.push_back(key.decrypt(encryptedText[i]));
	}	
}

void performInteractiveAction(IO& io, AlphabetPermutation& key, const QString& encryptedText, QString& decryptedText) {
	IO::writeToScreen("");
	IO::writeToScreen("Decrypted text : ");
	IO::writeToScreen(decryptedText);
	while(true) {
		IO::writeToScreen("");
		IO::writeToScreen("Enter x to save and exit, f to flip two characters in text");
		QChar c;
		io.inputChar(c);
		if(c.toLower() == QChar('x')) {
			return;
		} else if(c.toLower() == QChar('f')) {
			QChar first, second;
			IO::writeToScreen("Enter first character:");
			io.inputChar(first);
			IO::writeToScreen("Enter second character:");
			io.inputChar(second);
			
			IO::writeToScreen(QString("You selected to flip letters %1 and %2").arg(first, second));
			
			key.flipDecryptedChars(first, second);
			
			decrypt(key, encryptedText, decryptedText);
			
			IO::writeToScreen("");
			IO::writeToScreen("Decrypted text : ");
			IO::writeToScreen(decryptedText);			
		} else {
			IO::writeToScreen("Incorrect command");
			continue;
		}
	}
}

int main(int argc, char *argv[])
{
	Settings* settings = Settings::getInstance();
	try {
		settings->parse(argc, argv);
		
		if(settings->help()) {
			std::cout<<settings->usage();
			return 0;
		}
		
		QTextCodec::setCodecForCStrings(
				QTextCodec::codecForName("UTF-8")
				);		
		
		IO io(settings);
		
		Statistics refStat;
		
		if(settings->statOnly()) {
			
			readStatistics(io, refStat);
			
			refStat.saveToFile(settings->statFile());
			
			return 0;
		}				
		
		IO::writeVerboseMessage("Loading reference statistics...");
		
		refStat.loadFromFile(settings->statFile());
		
		Statistics textStat;
		
		IO::writeVerboseMessage("Loading intput file...");
		
		QString encryptedText = io.input().readAll();
		
		IO::writeVerboseMessage("Building input file statistics...");
		
		textStat.analyzeText(encryptedText);
		
		Analyzer analyzer(refStat, textStat);
		
		IO::writeVerboseMessage("Running frequency difference minimization...");
		
		analyzer.run();
		
		IO::writeVerboseMessage("Decrypting...");
		
		QString decryptedText;
		decryptedText.reserve(encryptedText.size());
		
		AlphabetPermutation key = analyzer.result();
		
		decrypt(key, encryptedText, decryptedText);
		
		if(settings->interactive()) {
			performInteractiveAction(io, key, encryptedText, decryptedText);
		}
		
		io.output()<<decryptedText;
		
		IO::writeVerboseMessage("Decrypted text written to output.");
	} catch(UserError e) {
		std::cerr<<"Error : "<<e.what()<<std::endl;
		if(e.showHelp()) {
			std::cerr<<settings->usage();
		}
		return -1;
	} catch(std::runtime_error e) {
		std::cerr<<"Runtime exception : "<<e.what()<<std::endl;
		return -2;
	} catch(std::exception e) {
		std::cerr<<"Exception : "<<e.what()<<std::endl;
		return -2;
	} catch(...) {
		std::cerr<<"Unknown fatal exception"<<std::endl;
		return -2;
	}

    return 0;
}

