// Comment this line out to switch off all custom mod logging
#define CUSTOMLOGDEBUG "CustomMods.log"

// Custom mod logger
#ifdef CUSTOMLOGDEBUG
#define CUSTOMLOG(sFmt, ...) {  \
  CvString sMsg;  \
  CvString::format(sMsg, sFmt, __VA_ARGS__);  \
  LOGFILEMGR.GetLog(CUSTOMLOGDEBUG, FILogFile::kDontTimeStamp)->Msg(sMsg.c_str());  \
}
#else
#define	CUSTOMLOG(sFmt, ...) ((void)0)
#endif