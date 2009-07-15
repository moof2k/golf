/*
 *  RudeTweaker.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeTweaker.h"
#include "Rude.h"

#ifndef NO_RUDETWEAKER

#include "microhttpd.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


RudeTweak::RudeTweak(const char *name, eRudeTweakType type, void *data)
: m_name(name)
, m_type(type)
, m_data(data)
{
	RudeTweaker::GetInstance()->AddTweak(this);
}

RudeTweaker * RudeTweaker::GetInstance()
{
	static RudeTweaker *s_instance = 0;
	if(s_instance == 0)
		s_instance = new RudeTweaker();
	return s_instance;
}

void RudeTweaker::AddTweak(RudeTweak *tweak)
{
	bool inserted = false;
	
	tTweakerList::iterator end = m_tweaks.end();
	
	for(tTweakerList::iterator i = m_tweaks.begin(); i != end; ++i)
	{
		RudeTweak *it = *i;
		int result = strcmp(it->m_name, tweak->m_name);
		
		if(result > 0)
		{
			m_tweaks.insert(i, tweak);
			return;
		}
	}
	
	if(!inserted)
	{
		m_tweaks.push_back(tweak);
	}
}

static int HttpHandlerGbl(void * cls,
					struct MHD_Connection * connection,
					const char * url,
					const char * method,
                    const char * version,
					const char * upload_data,
					unsigned int * upload_data_size,
                    void ** ptr)
{
	return RudeTweaker::GetInstance()->HttpHandler(cls,
									connection,
									url,
									method,
									version,
									upload_data,
									upload_data_size,
									ptr);
}

char text[10 * 1024] = "";

int RudeTweaker::HttpHandler(void * cls,
							 struct MHD_Connection * connection,
							 const char * url,
							 const char * method,
							 const char * version,
							 const char * upload_data,
							 unsigned int * upload_data_size,
							 void ** ptr)
{
	static int dummy;
	struct MHD_Response * response;
	int ret;
	
	if (&dummy != *ptr) 
    {
		/* The first time only the headers are valid,
         do not respond in the first round... */
		*ptr = &dummy;
		return MHD_YES;
    }
	*ptr = NULL; /* clear context pointer */
	
	if(url)
		printf("URL: %s\n", url);
	if(upload_data)
		printf("Data: %s\n", upload_data);
	
	strcpy(text, "<html><head><title>Rude Tweaker</title></head>\r\n");
	strcat(text, "\n");
	strcat(text, "\n");
	
	for(tTweakerList::iterator i = m_tweaks.begin(); i != m_tweaks.end(); ++i)
	{
		RudeTweak *it = *i;
		
		const int kItemLen = 512;
		char item[kItemLen];
		char value[kItemLen];
		
		const char *invalue = MHD_lookup_connection_value (connection, MHD_GET_ARGUMENT_KIND, it->m_name);
		if(invalue)
			printf("Value: %s\n", invalue);
		
		switch(it->m_type)
		{
			case kBool:
				if(invalue)
				{
					int parsedvalue = 0;
					int results = sscanf(invalue, "%d", &parsedvalue);
					if(results == 1)
						*((bool *) it->m_data) = parsedvalue;
				}
				
				snprintf(value, kItemLen, "%d", *((bool *) it->m_data));
				break;
			case kFloat:
				if(invalue)
					sscanf(invalue, "%f", ((float *) it->m_data));
				
				snprintf(value, kItemLen, "%f", *((float *) it->m_data));
				break;
		}
		
		snprintf(item, kItemLen, "<form method=\"get\">%s: <input type=\"text\" name=\"%s\" value=\"%s\"></form><br>\n", it->m_name, it->m_name, value);
		
		
		
		strcat(text, item);
	}
	
	strcat(text, "<br><a href=\"/\">Refresh</a>\n");
	
	int textlen = strlen(text);
	RUDE_ASSERT(textlen < sizeof(text), "Tweaker exceeded text buffer bounds (%d / %d)", textlen, sizeof(text));
	
	//printf(text);
	
	response = MHD_create_response_from_data(strlen(text),
											 text,
											 MHD_NO,
											 MHD_NO);
	ret = MHD_queue_response(connection,
							 MHD_HTTP_OK,
							 response);
	MHD_destroy_response(response);
	return ret;
}

void RudeTweaker::Init()
{
	struct MHD_Daemon * d;
	d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
						 8080,
						 NULL,
						 NULL,
						 &HttpHandlerGbl,
						 0,
						MHD_OPTION_END);
	//if (d == NULL)
	//	return 1;
	//sleep(atoi(argv[2]));
	//MHD_stop_daemon(d);
	//return 0;
}


#endif // NO_RUDETWEAKER


