#include <boost/foreach.hpp>

#include <algorithm>
#include <iterator>
#include <utility>

#include "AlphabetPermutation.h"
#include "IO.h"

std::ostream& operator<<(std::ostream &stream, const AlphabetPermutation& perm) {
	BOOST_FOREACH(AlphabetPermutation::CharMap::value_type p, perm.m_encrypt) {
		if(p.first == QChar(' ')) {
			continue;
		}
		stream<<"("<<p.first.toAscii()<<"->"<<p.second.toAscii()<<"),";
	}
	stream<<";\n";
	BOOST_FOREACH(AlphabetPermutation::CharMap::value_type p, perm.m_decrypt) {
		if(p.first == QChar(' ')) {
			continue;
		}
		stream<<"("<<p.first.toAscii()<<"->"<<p.second.toAscii()<<");";
	}
	
	
	return stream;
}

QString AlphabetPermutation::m_alphabet = "";

AlphabetPermutation::AlphabetPermutation()
{
	initAlphabet();
}

void AlphabetPermutation::setPermutation(const QString& referenceAlphabet, const QString& permutation) {
	// TODO: check alphabets correctness
	
	m_encrypt.insert(std::make_pair(QChar(' '), QChar(' ')));
	m_decrypt.insert(std::make_pair(QChar(' '), QChar(' ')));	
	
	for(int i = 0; i < referenceAlphabet.length(); i++) {
		m_encrypt.insert(std::make_pair(referenceAlphabet[i], permutation[i]));
		m_decrypt.insert(std::make_pair(permutation[i], referenceAlphabet[i]));
	}		
}

void AlphabetPermutation::flipChars(QChar c1, QChar c2) {
	if(c1 == c2) {
		return;
	}
	
	QChar decryptC1 = decrypt(c1);
	QChar decryptC2 = decrypt(c2);
	
	m_encrypt[decryptC1] = c2;
	m_encrypt[decryptC2] = c1;
	
	m_decrypt[c1] = decryptC2;
	m_decrypt[c2] = decryptC1;
}

void AlphabetPermutation::flipDecryptedChars(QChar c1, QChar c2) {
	flipChars(encrypt(c1), encrypt(c2));
}

QChar AlphabetPermutation::encrypt(QChar c) const {
	return mapChar(m_encrypt, c);
}

QChar AlphabetPermutation::decrypt(QChar c) const {
	return mapChar(m_decrypt, c);
}

void AlphabetPermutation::initAlphabet() {
	if(m_alphabet.length() > 0) {
		return;
	}
	
	Settings::LanguageType language = Settings::getInstance()->language();
	
	if(language == Settings::English) {
		m_alphabet = "abcdefghijklmnopqrstuvwxyz";
	} else {
		m_alphabet.reserve(40);
		for(uint i = 1072u; i <= 1103u; i++) {
			m_alphabet.push_back(QChar(i));
		}
		//¸
		m_alphabet.push_back(QChar(1105u));
		
		IO::writeVerboseMessage(
				"Alphabet: " + m_alphabet.toStdString());
		//m_alphabet = "àáâãäå¸æçèéêëìíîïðñòóôõö÷øùúûüýþÿ";
	}
}

QChar AlphabetPermutation::mapChar(const CharMap &map, QChar ch) {
	if(!ch.isLetter()) {
		return ch;
	} else {
		bool isUpper = !(ch.isLower());
		CharMap::const_iterator it = map.find(isUpper ? ch.toLower() : ch);
		if(it == map.end()) {
			//throw std::runtime_error("Failed to map char " + str(ch.toAscii()));
			return ch;
		}
		const QChar& result = it->second;
		return isUpper ? result.toUpper() : result;
	}
}
