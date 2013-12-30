#include <iostream>
#include <sstream>
//http://stackoverflow.com/questions/4366904/custom-stream-to-method-in-c/4372966#4372966
class CLogger : public std::ostream {
private:
	class CLogBuf : public std::stringbuf {
	private:
		// or whatever you need for your application
		std::string m_marker;
		std::ostream* m_file;
		bool m_auto_flush;
	public:
		CLogBuf(const std::string& marker, std::ostream& file, bool autoFlush=false) :
			m_marker(marker), m_file(&file), m_auto_flush(autoFlush) { }
		~CLogBuf() { pubsync(); }
		int sync()
		{
			if (m_auto_flush)
			{
				std::cout << m_marker << ": " << str() << std::flush;// str("");
				if (m_file)
					*m_file << m_marker << ": " << str() << std::flush;
				str("");
			}
			else
			{
				std::cout << m_marker << ": " << str();// str("");
				if (m_file)
					*m_file << m_marker << ": " << str();
				str("");
			}
			//if (m_auto_flush) std::cout << std::flush;
			//if (m_file)
			//{
			//	*m_file << m_marker << ": " << str(); str("");
			//	if (m_auto_flush) *m_file << std::flush;
			//}
			return !std::cout;
		}
	};

public:
	// Other constructors could specify filename, etc
	// just remember to pass whatever you need to CLogBuf
	CLogger(const std::string& marker, std::ostream& file, bool autoFlush = false) :
		std::ostream(new CLogBuf(marker, file,autoFlush)) {}
	~CLogger() { delete rdbuf(); }
};