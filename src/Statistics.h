#ifndef STATISTICS_H
#define STATISTICS_H

#include <map>
#include <string>
#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/nvp.hpp>

#include <QString>

#include "Settings.h"
#include "QStringSerialization.h"

// Статистика по буквам, биграммам и триграммам. 
class Statistics
{
public:
	Statistics();

	void analyzeNextLetter(QChar letter);
	
	void analyzeText(const QString& text);
	
	void calculateStatistics();
	double statistics(const QString& str) const;
	double statistics(QChar c) const;
	
	void saveToFile(const std::string& fileName);
	void loadFromFile(const std::string& fileName);
private:
	
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive& archive, const unsigned int /*version*/) {
		archive & BOOST_SERIALIZATION_NVP(m_statistics);
	}
	
	QString m_buffer;
	std::vector<bool> m_isLetter;
	
	Settings* m_settings;
	
	typedef std::map<QString, int> CountMap;
	typedef std::map<QString, double> StatMap;
	
	std::vector<CountMap> m_occCount;
	std::vector<int> m_count;
	mutable StatMap m_statistics;
	
	mutable bool m_calculated;
	
	bool isLetter(QChar ch) const;
	
	void addToBuffer(QChar letter);
	void addToMap(const QString& str);
	
	void fillMapFromBuffer();

};

#endif // STATISTICS_H
