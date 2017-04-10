#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef _MY_LOGGER_H_
#define _MY_LOGGER_H_

#include <cstdio>

#define MY_BUFFER_SIZE 512u
#define STR_TMP_SIZE 15u

class MyLogger 
{
  private:
    unsigned short bufSize;
    unsigned short pos; // Position of the buffer
    FILE *f;
    char *buffer;
    char strTmp[STR_TMP_SIZE];


  public:

    MyLogger();
    MyLogger(unsigned short buffer_size);

    ~MyLogger();

    // A dummy logger for now, TODO improve
    MyLogger& operator<<(char c);
    MyLogger& operator<<(short s);
    MyLogger& operator<<(int i);
    MyLogger& operator<<(float f);
    MyLogger& operator<<(double d);

    MyLogger& operator<<(char *c);
    MyLogger& operator<<(const char *cc);

    MyLogger& operator<<(unsigned int ui);
    MyLogger& operator<<(unsigned long ul);

    void flush();

    void getTime();

    void saveToFile(const char *strFile);

};

#endif

