#ifndef ALPHABETPERMUTATION_H
#define ALPHABETPERMUTATION_H

#include <map>
#include <iostream>

#include <QString>

#include "Settings.h"
#include "Statistics.h"

class AlphabetPermutation
{
	friend std::ostream& operator<<(std::ostream &stream, const AlphabetPermutation& p);
public:
    AlphabetPermutation();
	
	void setPermutation(const QString& referenceAlphabet, const QString& permutation);
	
	QChar encrypt(QChar c) const;
	QChar decrypt(QChar c) const;
	
	void flipChars(QChar from, QChar to);
	void flipDecryptedChars(QChar c1, QChar c2);
	
	static void initAlphabet();
	static const QString& alphabet() { return m_alphabet; }
private:
	static QString m_alphabet;
	
	typedef std::map<QChar, QChar> CharMap;
	
	CharMap m_encrypt;
	CharMap m_decrypt;
		
	static QChar mapChar(const CharMap& map, QChar ch);
};

std::ostream& operator<<(std::ostream &stream, const AlphabetPermutation& p);

#endif // ALPHABETPERMUTATION_H
