
#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "lintowin.h"

#include <cstdio>

#define ELEM_LEN 31
#define VAL_LEN  71

#define DATADIR_LEN 100
#define DATAFILE_LEN 120

struct list_elements_t {
  char element[ELEM_LEN];
  char val[VAL_LEN];
  list_elements_t *next;
};

class Configuration {
  private:
    
    char *strConfigData; // Data to save first time config file.

    list_elements_t *myList;
    FILE *f;

    char myDataDir[DATADIR_LEN];
    char myDataFile[DATAFILE_LEN];

    bool createANewConfig();

    void insert(const char *attr, const char *val);

    bool parseFile();

    void deleteList();

  public:

    Configuration(const char *configFile, const char *configDir = 0);

    ~Configuration();

    bool readFile();

    void get(char *val, const char *attr);

    void setDefaultNewFileData(char *fileContent);

    inline bool fileExist() { return (f!=NULL); }

};


#endif // _CONFIGURATION_H_

