/*
 *  RudeTweaker.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeTweaker
#define __H_RudeTweaker

#define NO_RUDETWEAKER


#ifndef NO_RUDETWEAKER

#include <list>


typedef enum {
	kBool,
	kInt,
	kFloat,
	
} eRudeTweakType;

class RudeTweak {
public:
	
	RudeTweak(const char *name, eRudeTweakType type, void *data);

	const char *m_name;
	eRudeTweakType m_type;
	void *m_data;
	
};

#define RUDE_TWEAK(name, type, var) \
	RudeTweak name##_tweak(#name , type, &(var));


typedef std::list<RudeTweak *> tTweakerList;

class RudeTweaker {
	
public:
	
	static RudeTweaker * GetInstance();
	
	void Init();
	
	void AddTweak(RudeTweak *tweak);
	
	int HttpHandler(void * cls,
										struct MHD_Connection * connection,
										const char * url,
										const char * method,
										const char * version,
										const char * upload_data,
										unsigned int * upload_data_size,
										void ** ptr);
	
private:
	
	tTweakerList m_tweaks;
	
};

#else

#define RUDE_TWEAK(name, type, var) \
	;

#endif

#endif
