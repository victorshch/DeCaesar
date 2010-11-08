#ifndef MUTATIONSTRATEGY_H
#define MUTATIONSTRATEGY_H

#include <QString>

#include "Statistics.h"
#include "AlphabetPermutation.h"

// Стратегия изменения перестановки алфавита при минимизации целевой функции
class MutationStrategy
{
public:
    MutationStrategy();
	
	void setMutationCandidates(const QString& mutationCandidates);
	
	bool canMutate() const;
	void mutate(AlphabetPermutation& permutation);
	
	void reset();
private:
	QString m_mutationCandidates;
	
	int a;
	int b;
};

#endif // MUTATIONSTRATEGY_H
