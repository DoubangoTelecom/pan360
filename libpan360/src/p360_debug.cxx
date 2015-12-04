#include "pan360/p360_debug.h"

const void* P360DebugMgr::s_pcArgData = NULL;
P360DebugFuncPtr P360DebugMgr::s_pfInfo = NULL;
P360DebugFuncPtr P360DebugMgr::s_pfWarn = NULL;
P360DebugFuncPtr P360DebugMgr::s_pfError = NULL;
P360DebugFuncPtr P360DebugMgr::s_pfFatal = NULL;
#if defined(DEBUG) || defined(_DEBUG)
P360_DEBUG_LEVEL P360DebugMgr::s_eLevel = P360_DEBUG_LEVEL_INFO;
#else
P360_DEBUG_LEVEL P360DebugMgr::s_eLevel = P360_DEBUG_LEVEL_WARN;
#endif

P360DebugMgr::P360DebugMgr()
{

}

P360DebugMgr::~P360DebugMgr()
{

}


void P360DebugMgr::setArgData(const void* pcArgData)
{
    P360DebugMgr::s_pcArgData = pcArgData;
}

const void* P360DebugMgr::getArgData()
{
    return P360DebugMgr::s_pcArgData;
}

void P360DebugMgr::setInfoFuncPtr(P360DebugFuncPtr funPtr)
{
    P360DebugMgr::s_pfInfo = funPtr;
}

P360DebugFuncPtr P360DebugMgr::getInfoFuncPtr()
{
    return P360DebugMgr::s_pfInfo;
}

void P360DebugMgr::setWarnFuncPtr(P360DebugFuncPtr funPtr)
{
    P360DebugMgr::s_pfWarn = funPtr;
}

P360DebugFuncPtr P360DebugMgr::getWarnFuncPtr()
{
    return P360DebugMgr::s_pfWarn;
}

void P360DebugMgr::setErrorFuncPtr(P360DebugFuncPtr funPtr)
{
    P360DebugMgr::s_pfError = funPtr;
}

P360DebugFuncPtr P360DebugMgr::getErrorFuncPtr()
{
    return P360DebugMgr::s_pfError;
}

void P360DebugMgr::setFatalFuncPtr(P360DebugFuncPtr funPtr)
{
    P360DebugMgr::s_pfFatal = funPtr;
}

P360DebugFuncPtr P360DebugMgr::getFatalFuncPtr()
{
    return P360DebugMgr::s_pfFatal;
}

P360_DEBUG_LEVEL P360DebugMgr::getLevel()
{
    return P360DebugMgr::s_eLevel;
}

void P360DebugMgr::setLevel(P360_DEBUG_LEVEL eLevel)
{
    P360DebugMgr::s_eLevel = eLevel;
}
