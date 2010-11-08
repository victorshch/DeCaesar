#ifndef ANALYZER_H
#define ANALYZER_H

#include <QString>

#include "Settings.h"
#include "Statistics.h"
#include "AlphabetPermutation.h"
#include "MutationStrategy.h"

// Анализатор -- подбирает перестановку алфавита, минимизирующую целевую функцию
class Analyzer
{
public:
    Analyzer(const Statistics& referenceStatistics, const Statistics& textStatistics);
	
	void run();
	
	const AlphabetPermutation& result() const { return m_key; }
	
private:
	static const int maxIterationCount;
	
	const Statistics* m_referenceStatistics;
	const Statistics* m_textStatistics;
	
	AlphabetPermutation m_key;
	MutationStrategy m_strategy;
	
	double factor;
	
	// Целевая функция
	double objectiveFunction(const AlphabetPermutation& permutation) const;
};

#endif // ANALYZER_H
