#include "MutationStrategy.h"

MutationStrategy::MutationStrategy() 
	: a(0), b(1)
{ }

void MutationStrategy::setMutationCandidates(const QString& mutationCandidates) {
	m_mutationCandidates = mutationCandidates;
}

bool MutationStrategy::canMutate() const {
	return b < m_mutationCandidates.length();
}

void MutationStrategy::mutate(AlphabetPermutation &permutation) {
	if(!canMutate()) {
		return;
	}
	
	QChar c1 = m_mutationCandidates[a];
	QChar c2 = m_mutationCandidates[a + b];

	permutation.flipChars(c1, c2);	
	
	++a;
	if((a + b) >= m_mutationCandidates.length()) {
		a = 0;
		++b;
	}	
}

void MutationStrategy::reset() {
	a = 0;
	b = 1;
}
