#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class UserError : public std::runtime_error {
public:
	UserError(const std::string& what, bool showHelp = false) : std::runtime_error(what), m_showHelp(showHelp) 
	{}
	
	bool showHelp() const { return m_showHelp; }
	
private:
	bool m_showHelp;
};

#endif // EXCEPTIONS_H
