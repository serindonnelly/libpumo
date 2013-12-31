#pragma once
#include <iostream>
#include <sstream>

class CLogger : public std::ostream {
private:
    class CLogBuf : public std::stringbuf {
    private:
        // or whatever you need for your application
        std::string m_marker;
		std::ostream* m_file;
    public:
        CLogBuf(const std::string& marker, std::ostream& file) :
			m_marker(marker), m_file(&file) { }
        ~CLogBuf() {  pubsync(); }
        int sync() { (*m_file) << m_marker << ": " << str(); str("");  return !(*m_file); }
    };

public:
    // Other constructors could specify filename, etc
    // just remember to pass whatever you need to CLogBuf
    CLogger(const std::string& marker,std::ostream& file) : std::ostream(new CLogBuf(marker,file)) {}
    ~CLogger() { delete rdbuf(); }
};

static CLogger* err;
static CLogger* warn;
static CLogger* info;