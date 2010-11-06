#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <QFile>

#include "Settings.h"
#include "Exceptions.h"

Settings* Settings::self = 0;

Settings::Settings() : od("Possible options")
{
	m_help = false;
	m_interactive = false;
	m_verbose = false;
	m_statOnly = false;
	m_language = Settings::English;
	
	od.add_options()
			("manual,m", "Manual mode -- make modifications to decrypted text")
			("verbose,v", "Verbose mode (additional messages are written to stderr)")
			("language,l", boost::program_options::value<std::string>(), "Language assumption: en for english, ru for russian. Default is en")
			("input-file,i", boost::program_options::value<std::string>(), "Input file name. If not specified, stdin is used")
			("output-file,o", boost::program_options::value<std::string>(), "Output file name. If not specified, stdout is used")
			("stat-file,s", boost::program_options::value<std::string>(), "XML Statistics file name. Default is en_stat.xml for english, ru_stat.xml for russian")
			("gather-stat,g", "Gather statistics for input, write it to stat-file and exit")
			("help,h", "Display help message and exit");
	
	m_usage = "DeCaesar -- substitution cipher decryption\n"
			  "Usage : decaesar options\n" + boost::lexical_cast<std::string>(od) +"\n"
			  "Author: Victor Shcherbinin (victorshch@gmail.com)\n";
}

Settings* Settings::getInstance() {
	if(self == 0) {
		self = new Settings();
	}
	
	return self;
}

void Settings::parse(int argc, char *argv[]) {
	boost::program_options::variables_map vm;
	boost::program_options::store(
			boost::program_options::parse_command_line(argc, argv, od),
			vm
			);
	boost::program_options::notify(vm);
	
	if(vm.count("help")) {
		m_help = true;
		return;
	}
	
	if(vm.count("verbose")) {
		m_verbose = true;
	}
	
	if(vm.count("manual")) {
		m_interactive = true;
	}
	
	if(vm.count("gather-stat")) {
		m_statOnly = true;
	}
	
	if(vm.count("language")) {
		std::string langStr = boost::algorithm::to_lower_copy(vm["language"].as<std::string>());
		if(langStr == "en") {
			m_language = Settings::English;
		} else if(langStr == "ru") {
			m_language = Settings::Russian;
		} else {
			throw UserError("Incorrect language specification : '" + vm["language"].as<std::string>()+"'", false);
		}
	} else {
		m_language = Settings::English;
	}
	
	m_inputFile = getOptStr(vm, "input-file");
	m_outputFile = getOptStr(vm, "output-file");
	
	m_statFile = getOptStr(vm, "stat-file");
	if(m_statFile.size() == 0) {
		switch(m_language) {
			case Settings::English: m_statFile = "en_stat.xml"; break;
			case Settings::Russian: m_statFile = "ru_stat.xml"; break;
		}
	}
	
	check();
}

std::string Settings::getOptStr(const boost::program_options::variables_map &vm, const std::string &optName) {
	if(vm.count(optName)) {
		return vm[optName].as<std::string>();
	}
	return "";
}

void Settings::check() const {
	if(inputFile().size() > 0 && !checkFileExists(inputFile())) {
		throw UserError("Input file '"+inputFile()+"' does not exist", 
						false);
	}

	if(this->interactive() && inputFile().size() == 0) {
		throw UserError("Can't use stdin as input file in manual mode -- specify input file", false);
	}
}

bool Settings::checkFileExists(const std::string &fileName) const {
	return QFile::exists(fileName.c_str());
}
