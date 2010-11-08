#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

#include <boost/program_options.hpp>

// Набор настроек, которые пользователь указал в опциях командной строки
class Settings
{
public:
	static Settings* getInstance();
	
	// Парсит аргументы и проверяет их корректность
	void parse(int argc, char *argv[]);
	
	enum LanguageType { English = 0, Russian = 1 };
	
	bool help() const { return m_help; }
	bool interactive() const { return m_interactive; }
	bool statOnly() const { return m_statOnly; }
	bool verbose() const { return m_verbose; }
	LanguageType language() const { return m_language; }
	const std::string& inputFile() const { return m_inputFile; }
	const std::string& outputFile() const { return m_outputFile; }
	const std::string& statFile() const { return m_statFile; }
	
	const std::string& usage() const { return m_usage; }
private:
	Settings();
	
	bool m_help;
	bool m_interactive;
	bool m_statOnly;
	bool m_verbose;
	LanguageType m_language;
	std::string m_inputFile;
	std::string m_outputFile;
	std::string m_statFile;
	
	std::string m_usage;
	
	boost::program_options::options_description od;
	
	static Settings* self;
	
	std::string getOptStr(const boost::program_options::variables_map& vm, const std::string& optName);
	void check() const;
	bool checkFileExists(const std::string& fileName) const;
};

#endif // SETTINGS_H
