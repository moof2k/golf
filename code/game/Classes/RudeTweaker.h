/*
 *  RudeTweaker.h
 *  golf
 *
 *  Created by Robert Rose on 9/9/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RudeTweaker
#define __H_RudeTweaker

//#define NO_RUDETWEAKER


#ifndef NO_RUDETWEAKER

#include <map>


typedef enum {
	kBool,
	kInt,
	kFloat,
	
} eRudeTweakType;

class RudeTweak {
public:
	
	RudeTweak(const char *name, eRudeTweakType type, void *data);

	eRudeTweakType m_type;
	void *m_data;
	
};

#define RUDE_TWEAK(name, type, var) \
	RudeTweak name##_tweak(#name , type, &(var));


typedef std::map<const char *, RudeTweak *> tTweakerMap;

class RudeTweaker {
	
public:
	
	static RudeTweaker * GetInstance();
	
	void Init();
	
	void AddTweak(const char *name, RudeTweak *tweak)
	{
		m_tweaks[name] = tweak;
	}
	
	int RudeTweaker::HttpHandler(void * cls,
										struct MHD_Connection * connection,
										const char * url,
										const char * method,
										const char * version,
										const char * upload_data,
										unsigned int * upload_data_size,
										void ** ptr);
	
private:
	
	tTweakerMap m_tweaks;
	
};

#else

#define RUDE_TWEAK(name, type, var) \
	;

#endif

#endif
