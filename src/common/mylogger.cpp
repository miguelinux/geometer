#include "mylogger.h"
#include "lintowin.h"
#include <cstring>
#include <ctime>

MyLogger::MyLogger()
{
  buffer = new char[MY_BUFFER_SIZE];
  buffer[0] = 0;
  bufSize = MY_BUFFER_SIZE;
  pos = 0u;
  f = 0;
}

MyLogger::MyLogger(unsigned short buffer_size)
{
  buffer = new char[buffer_size];
  buffer[0] = 0;
  bufSize = buffer_size;
  pos = 0u;
  f = 0;
}

MyLogger::~MyLogger()
{
  flush();
  if (f) {
    fclose(f);
    f=0;
  }
  delete [] buffer;
}

MyLogger& MyLogger::operator<<(char c)
{
  if (pos + 1u >= bufSize) {
    flush();
  }
  buffer[pos++]=c;
  buffer[pos]=0;
  return *this;
}

MyLogger& MyLogger::operator<<(short s)
{
  if (pos + STR_TMP_SIZE >= bufSize) {
    flush();
  }
  SPRINTF(strTmp,STR_TMP_SIZE,"%hd",s);  
  STRNCAT(&buffer[pos],strTmp,STR_TMP_SIZE);
  pos += strlen(strTmp);
  return *this;
}

MyLogger& MyLogger::operator<<(int i)
{
  if (pos + STR_TMP_SIZE >= bufSize) {
    flush();
  }
  SPRINTF(strTmp,STR_TMP_SIZE,"%d",i);  
  STRNCAT(&buffer[pos],strTmp,STR_TMP_SIZE);
  pos += strlen(strTmp);
  return *this;
}

MyLogger& MyLogger::operator<<(float f)
{
  if (pos + STR_TMP_SIZE >= bufSize) {
    flush();
  }
  SPRINTF(strTmp,STR_TMP_SIZE,"%f",f);  
  STRNCAT(&buffer[pos],strTmp,STR_TMP_SIZE);
  pos += strlen(strTmp);
  return *this;
}

MyLogger& MyLogger::operator<<(double d)
{
  if (pos + STR_TMP_SIZE >= bufSize) {
    flush();
  }
  SPRINTF(strTmp,STR_TMP_SIZE,"%f",d);  
  STRNCAT(&buffer[pos],strTmp,STR_TMP_SIZE);
  pos += strlen(strTmp);
  return *this;
}

MyLogger& MyLogger::operator<<(char *c)
{
  int size =strlen(c);
  if (pos + size >= bufSize) {
    flush();
  }
  STRNCAT(&buffer[pos],c,size+1); // Add Null termination
  pos += size;
  return *this;
}

MyLogger& MyLogger::operator<<(const char *cc)
{
  int size =strlen(cc);
  if (pos + size >= bufSize) {
    flush();
  }
  STRNCAT(&buffer[pos],cc,size+1); // Add Null termination
  pos += size;
  return *this;
}

MyLogger& MyLogger::operator<<(unsigned int ui)
{
  if (pos + STR_TMP_SIZE >= bufSize) {
    flush();
  }
  SPRINTF(strTmp,STR_TMP_SIZE,"%u",ui);  
  STRNCAT(&buffer[pos],strTmp,STR_TMP_SIZE);
  pos += strlen(strTmp);
  return *this;
}

MyLogger& MyLogger::operator<<(unsigned long ul)
{
  if (pos + STR_TMP_SIZE >= bufSize) {
    flush();
  }
  SPRINTF(strTmp,STR_TMP_SIZE,"%ld",ul);  
  STRNCAT(&buffer[pos],strTmp,STR_TMP_SIZE);
  pos += strlen(strTmp);
  return *this;
}

void MyLogger::flush()
{
  if (buffer[0] != 0) {
    printf(buffer);
    if (f) fprintf(f,buffer);
    buffer[0]=0;
    pos = 0u;
  }
}

void MyLogger::getTime()
{
  char strTime[23];
  time_t rawtime;

  time ( &rawtime );

#ifdef WIN32
  struct tm  timeinfo;
  localtime_s (&timeinfo, &rawtime );
  sprintf_s(strTime, 23, "[%4d.%2d.%2d %2d:%02d:%02d] ", 1900+timeinfo.tm_year,
        timeinfo.tm_mon+1, timeinfo.tm_mday, timeinfo.tm_hour,
        timeinfo.tm_min, timeinfo.tm_sec);
#else
  struct tm  *timeinfo;
  timeinfo = localtime ( &rawtime );
  sprintf(strTime, "[%4d.%2d.%2d %2d:%02d:%02d] ", 1900+timeinfo->tm_year,
        timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour,
        timeinfo->tm_min, timeinfo->tm_sec);
#endif

  if (pos + 23u >= bufSize) {
    flush();
  }
  STRNCAT(&buffer[pos],strTime,23);
  pos += 22u;
}

void MyLogger::saveToFile(const char *strFile)
{
  if (! strlen(strFile) ) return;
  if (f) {
    fclose(f);
  }
  FOPEN(f,strFile,"w");
  if (f==NULL) {
    f=0;
  }
}

