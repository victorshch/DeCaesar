#include <fstream>
#include <stdexcept>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "Exceptions.h"
#include "Statistics.h"

#include "IO.h"

Statistics::Statistics() : m_calculated(false), m_buffer("   "), m_isLetter(3, false), 
m_occCount(3), m_count(3, 0), m_settings(Settings::getInstance())
{

}

void Statistics::analyzeNextLetter(QChar character) {
	m_calculated = false;
	
	QString str1;
	str1.resize(1);
	str1[0] = character;

	bool isLetter = this->isLetter(character);
	
	if(isLetter || m_isLetter[2] || m_isLetter[1]) {
		if(!isLetter) {
			character = QChar(' ');
		}
		
		addToBuffer(character.toLower());
		fillMapFromBuffer();
				
		if(!isLetter) {
			addToBuffer(QChar(' '));
			fillMapFromBuffer();
		}
	}
}

void Statistics::analyzeText(const QString &text) {
	for(int i = 0; i < text.size(); i++) {
		analyzeNextLetter(text[i]);
	}
	
	calculateStatistics();
}

double Statistics::statistics(const QString &str) const {
	StatMap::const_iterator it = m_statistics.find(str);
	return it != m_statistics.end() ? it->second : 0;
}

double Statistics::statistics(QChar c) const {
	QString str;
	str.resize(1);
	str[0] = c;
	return statistics(str);
}

void Statistics::saveToFile(const std::string &fileName) {
	calculateStatistics();
	std::ofstream stream(fileName.c_str());
	if(!stream.good()) {
		throw UserError("Couldn't open file " + fileName + " for writing");
	}
	
	boost::archive::xml_oarchive archive(stream);
	
	archive << boost::serialization::make_nvp("Statistics", *this);
}

void Statistics::loadFromFile(const std::string &fileName) {
	std::ifstream stream(fileName.c_str());
	if(!stream.good()) {
		throw UserError("Couldn't open file "+fileName+" for reading");
	}
	
	boost::archive::xml_iarchive archive(stream);
	
	archive >> boost::serialization::make_nvp("Statistics", *this);	
	m_calculated = true;
}

void Statistics::calculateStatistics() {
	if(m_calculated) {
		return;
	}
	
	if(m_isLetter[2]) {
		analyzeNextLetter(QChar(' '));
	}
	
	m_statistics.clear();
	
	for(int i = 0; i < 3; i++) {
		for(CountMap::const_iterator it = m_occCount[i].begin(); it != m_occCount[i].end(); ++it) {
			m_statistics[it->first] = ((double) it->second) / (double) m_count[i];
		}
	}
	
	m_calculated = true;
}

bool Statistics::isLetter(QChar ch) const {
	ch = ch.toLower();
	switch(m_settings->language()) {
		case Settings::Russian : return (ch.unicode() >= 1072u && ch.unicode() <= 1103u) 
					|| (ch.unicode() == 1105u);
			
		case Settings::English : return (ch.unicode() >= QChar('a').unicode() && ch.unicode() <= QChar('z').unicode());
			
		default: throw std::logic_error("Unexpected LanguageType value");
	}
}

void Statistics::addToBuffer(QChar letter) {
	m_buffer[0] = m_buffer[1];
	m_buffer[1] = m_buffer[2];
	m_buffer[2] = letter;
	
	m_isLetter[0] = m_isLetter[1];
	m_isLetter[1] = m_isLetter[2];
	m_isLetter[2] = isLetter(letter);
}

void Statistics::addToMap(const QString &str) {
	if(str.size() > 3 || str.size() == 0) {
		return;
	}
	
	CountMap& map = m_occCount[str.size() - 1];
	
	CountMap::iterator it = map.find(str);
	if(it != map.end()) {
		map[str] = (it->second) + 1;
	} else {
		map[str] = 1;
	}

}

void Statistics::fillMapFromBuffer() {
	if(m_isLetter[2]) {
		addToMap(m_buffer.right(1));
		++m_count[0];
	}
	if(m_isLetter[2] || m_isLetter[1]) {
		addToMap(m_buffer.right(2));
		++m_count[1];
	}
	if(m_isLetter[2] || m_isLetter[1] || m_isLetter[0]) {
		addToMap(m_buffer);
		++m_count[2];
	}
}
