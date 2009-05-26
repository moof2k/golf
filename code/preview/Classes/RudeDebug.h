#ifndef _H_RudeDebug
#define _H_RudeDebug

#include "Rude.h"

#define kRudeDebugTextLen 5
#define kRudeDebugTextSize 80

class RudeDebug
{
public:
	static void Init();
	
	static void Write(const char *file, int line, char *format, ...);

	static void PurgeLog();

private:

	static char sLogfile[80];
	
};

#define RUDE_REPORT(...)  RudeDebug::Write(__FILE__, __LINE__, __VA_ARGS__)


#endif

