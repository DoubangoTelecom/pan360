/* Copyright (C) 2016 Doubango Telecom <https://www.doubango.org>
*
* This file is part of Open Source Panorama 360 (a.k.a pan360) project.
* Source code hosted at https://github.com/DoubangoTelecom/pan360
* Website hosted at http://pan360.org
*
* PAN360 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* PAN360 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with PAN360.
*/
#if !defined(_PAN360_DEBUG_H_)
#define _PAN360_DEBUG_H_

#include "pan360/p360_config.h"
#include "pan360/p360_common.h"

typedef int(*P360DebugFuncPtr)(const void* arg, const char* fmt, ...);

class P360_API P360DebugMgr
{
private:
	P360DebugMgr();
public:
	virtual ~P360DebugMgr();

public:
	static void setArgData(const void*);
	static const void* getArgData();
	static void setInfoFuncPtr(P360DebugFuncPtr);
	static P360DebugFuncPtr getInfoFuncPtr();
	static void setWarnFuncPtr(P360DebugFuncPtr);
	static P360DebugFuncPtr getWarnFuncPtr();
	static void setErrorFuncPtr(P360DebugFuncPtr);
	static P360DebugFuncPtr getErrorFuncPtr();
	static void setFatalFuncPtr(P360DebugFuncPtr);
	static P360DebugFuncPtr getFatalFuncPtr();
	static P360_DEBUG_LEVEL getLevel();
	static void setLevel(P360_DEBUG_LEVEL);

private:
	static const void* s_pcArgData;
	static P360DebugFuncPtr s_pfInfo;
	static P360DebugFuncPtr s_pfWarn;
	static P360DebugFuncPtr s_pfError;
	static P360DebugFuncPtr s_pfFatal;
	static P360_DEBUG_LEVEL s_eLevel;
};

/* INFO */
#define P360_DEBUG_INFO(FMT, ...)		\
	if (P360DebugMgr::getLevel() >= P360_DEBUG_LEVEL_INFO) { \
		if (P360DebugMgr::getInfoFuncPtr()) \
			P360DebugMgr::getInfoFuncPtr()(P360DebugMgr::getArgData(), "*[PAN360 INFO]: " FMT "\n", ##__VA_ARGS__); \
		else \
			fprintf(stderr, "*[PAN360 INFO]: " FMT "\n", ##__VA_ARGS__); \
		}


/* WARN */
#define P360_DEBUG_WARN(FMT, ...)		\
	if (P360DebugMgr::getLevel() >= P360_DEBUG_LEVEL_WARN) { \
		if (P360DebugMgr::getWarnFuncPtr()) \
			P360DebugMgr::getWarnFuncPtr()(P360DebugMgr::getArgData(), "**[PAN360 WARN]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
		else \
			fprintf(stderr, "**[PAN360 WARN]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
	}

/* ERROR */
#define P360_DEBUG_ERROR(FMT, ...) 		\
	if (P360DebugMgr::getLevel() >= P360_DEBUG_LEVEL_ERROR) { \
		if (P360DebugMgr::getErrorFuncPtr()) \
			P360DebugMgr::getErrorFuncPtr()(P360DebugMgr::getArgData(), "***[PAN360 ERROR]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
		else \
			fprintf(stderr, "***[PAN360 ERROR]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
	}


/* FATAL */
#define P360_DEBUG_FATAL(FMT, ...) 		\
	if (P360DebugMgr::getLevel() >= P360_DEBUG_LEVEL_FATAL) { \
		if (P360DebugMgr::getFatalFuncPtr()) \
			P360DebugMgr::getFatalFuncPtr()(P360DebugMgr::getArgData(), "****[PAN360 FATAL]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
		else \
			fprintf(stderr, "****[PAN360 FATAL]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__); \
	}


#define P360_DEBUG_INFO_EX(MODULE, FMT, ...) P360_DEBUG_INFO("[" MODULE "] " FMT, ##__VA_ARGS__)
#define P360_DEBUG_WARN_EX(MODULE, FMT, ...) P360_DEBUG_WARN("[" MODULE "] " FMT, ##__VA_ARGS__)
#define P360_DEBUG_ERROR_EX(MODULE, FMT, ...) P360_DEBUG_ERROR("[" MODULE "] " FMT, ##__VA_ARGS__)
#define P360_DEBUG_FATAL_EX(MODULE, FMT, ...) P360_DEBUG_FATAL("[" MODULE "] " FMT, ##__VA_ARGS__)

#endif /* _PAN360_DEBUG_H_ */
