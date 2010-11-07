#include <cmath>

#include <boost/lexical_cast.hpp>

#include "Analyzer.h"
#include "IO.h"

const int Analyzer::maxIterationCount = 2000;

struct CompareByStatistics {
	CompareByStatistics(const Statistics* statistics) : m_statistics(statistics) {}
	bool operator()(QChar left, QChar right) const {
		return m_statistics->statistics(left) < m_statistics->statistics(right);
	}
private:
	const Statistics* m_statistics;
	
};

Analyzer::Analyzer(const Statistics &referenceStatistics, const Statistics &textStatistics)
	:m_referenceStatistics(&referenceStatistics), m_textStatistics(&textStatistics)
{
	QString referenceAlphabet = AlphabetPermutation::alphabet();
	QString sortedAlphabet = referenceAlphabet;
	
	std::sort(referenceAlphabet.begin(), referenceAlphabet.end(), CompareByStatistics(&referenceStatistics));
	std::sort(sortedAlphabet.begin(), sortedAlphabet.end(), CompareByStatistics(&textStatistics));
	
	IO::writeVerboseMessage("Sorted reference alphabet : \t" + referenceAlphabet.toStdString());
	IO::writeVerboseMessage("Sorted text alphabet : \t" + sortedAlphabet.toStdString());
	
	m_key.setPermutation(referenceAlphabet, sortedAlphabet);
	
	m_strategy.setMutationCandidates(sortedAlphabet);
	
	switch(Settings::getInstance()->language()) {
		case Settings::English: factor = 10.0; break;
		case Settings::Russian: factor = 3.0; break;
		default: factor = 6.0; break;
	}
}

void Analyzer::run() {
	double minObjVal = objectiveFunction(m_key);
	IO::writeVerboseMessage("Initial objective value : " + str(minObjVal));
	
	m_strategy.reset();
	
	for(int i = 1; i <= maxIterationCount; i++) {
		IO::writeVerboseMessage("Iteration : " + str(i));
		
		double newObjVal;
		
		//IO::writeAuxMessage("Trying to decrease objective function...");
		
		AlphabetPermutation temp;
		
		do {
			if(!m_strategy.canMutate()) {
				IO::writeVerboseMessage("Cannon mutate further, finishing minimization");
				return;
			}
			
			temp = m_key;
			
			m_strategy.mutate(temp);
			
			//IO::writeAuxMessage("Mutated : " + str(temp));
			
			newObjVal = objectiveFunction(temp);
			
			//IO::writeAuxMessage("Current objective value : " + str(newObjVal));
			
		} while(newObjVal >= minObjVal);
		
		IO::writeVerboseMessage("Minimal objective value : " + str(newObjVal));
		
		minObjVal = newObjVal;	
		
		m_key = temp;
		
		m_strategy.reset();
	}
	
	IO::writeVerboseMessage("Maximum number of iterations reached, finishing minimization");
}

// Целевая функция -- взвешенная сумма отклонений по биграммам и триграммам
double Analyzer::objectiveFunction(const AlphabetPermutation &permutation) const {
	double resultBigramm = 0;
	double resultTrigramm = 0;
	
	QString str2;
	str2.resize(2);
	QString str3;
	str3.resize(3);
	
	QString encryptedStr2;
	encryptedStr2.resize(2);
	QString encryptedStr3;
	encryptedStr3.resize(3);
	
	const QString& alphabet = " " + AlphabetPermutation::alphabet();
	int alphabetCount = alphabet.length();
	
	for(int i = 0; i < alphabetCount; i++) {
		for(int j = 0; j < alphabetCount; j++) {
			str2[0] = alphabet[i];
			str2[1] = alphabet[j];
			
			encryptedStr2[0] = permutation.encrypt(str2[0]);
			encryptedStr2[1] = permutation.encrypt(str2[1]);
			
			resultBigramm += std::abs(m_referenceStatistics->statistics(str2) 
							   - m_textStatistics->statistics(encryptedStr2));
			
			for(int k = 0; k < alphabetCount; k++) {
				str3[0] = alphabet[i];
				str3[1] = alphabet[j];
 				str3[2] = alphabet[k];
				
				encryptedStr3[0] = encryptedStr2[0];
				encryptedStr3[1] = encryptedStr2[1];
				encryptedStr3[2] = permutation.encrypt(str3[2]);
				
				resultTrigramm += std::abs(m_referenceStatistics->statistics(str3) 
								   - m_textStatistics->statistics(encryptedStr3));				
			}			
		}
	}
	
	return resultBigramm + factor * resultTrigramm;
}
