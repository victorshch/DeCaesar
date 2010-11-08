#ifndef QSTRINGSERIALIZATION_H
#define QSTRINGSERIALIZATION_H

#include <string>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_free.hpp>

#include <QByteArray>
#include <QString>

// Объявления, необходимые для того, чтобы сериализация QString работала через boost_serialization
namespace boost {
	namespace serialization {
		template<class Archive>
		void save(Archive& archive, const QString& str, unsigned int /*version*/) {
			QByteArray byteArray = str.toUtf8();
			const char* data = byteArray.data();
			
			std::string dataStr = std::string(data);
			
			archive << BOOST_SERIALIZATION_NVP(dataStr);
		}
		
		template<class Archive>
		void load(Archive& archive, QString& str, unsigned int /*version*/) {
			std::string dataStr;
			
			archive >> BOOST_SERIALIZATION_NVP(dataStr);
			
			str = QString::fromUtf8(dataStr.c_str());
		}
	}
}

BOOST_SERIALIZATION_SPLIT_FREE(QString);

#endif // QSTRINGSERIALIZATION_H
