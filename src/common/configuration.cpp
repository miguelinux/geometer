#include "configuration.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef WIN32 
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

Configuration::Configuration(const char *configFile,
    const char *configDir )
{

  char *appData;

#ifdef WIN32 
  char delim[2]={"\\"};
  size_t len;
  _dupenv_s( &appData, &len, "APPDATA" );
#else
  char delim[2]={"/"};
  appData = getenv("HOME");
#endif

  STRCPY(myDataDir,DATADIR_LEN,appData);

  if (configDir != 0) {
    STRNCAT(myDataDir,delim,DATADIR_LEN);
    STRNCAT(myDataDir,configDir,DATADIR_LEN);
  }

  STRCPY(myDataFile,DATAFILE_LEN,myDataDir);
  STRNCAT(myDataFile,delim,DATAFILE_LEN);
  STRNCAT(myDataFile,configFile,DATAFILE_LEN);

  //delete [] appData;

  myList= 0;
  strConfigData = 0;

  FOPEN(f, myDataFile,"r");
}

Configuration::~Configuration()
{
  deleteList();
  if (f!=NULL) fclose(f);
}

bool Configuration::createANewConfig()
{

  if (f == NULL) {
    return false;
  }

  if (strConfigData) {
    fprintf(f,"%s\n",strConfigData);
  } else {
    fprintf(f,"; No configuration file given.\n\n");
  }

  rewind(f); // goto begin of File

  parseFile();

  return true;
}


void Configuration::insert(const char *attr, const char *val)
{
    list_elements_t *tmp;
    if (myList == 0) {
      myList = new list_elements_t();
      STRCPY(myList->element, ELEM_LEN, attr);
      STRCPY(myList->val, VAL_LEN, val);
      myList->next=0;
    } else {
      tmp = new list_elements_t();
      STRCPY(tmp->element, ELEM_LEN, attr);
      STRCPY(tmp->val,VAL_LEN, val);
      tmp->next=myList->next;
      myList->next=tmp;
    }
}

bool Configuration::parseFile()
{
  char buffer[101];
  char *pBuff;
  char element[ELEM_LEN];
  char val[VAL_LEN];
  short len;

  if (f == NULL) {
    return false;
  }

  while(!feof(f)) {
    // TODO: improve this poor parser
    fgets(buffer,101,f);
    if (buffer[0]==';' || buffer[0]=='\n') continue;
    pBuff = strchr(buffer,'=');
    if (pBuff == 0) continue;
    STRCPY(val,VAL_LEN,pBuff+1);
    *pBuff='\0';
    STRCPY(element,ELEM_LEN,buffer);
    len = strlen(val);
    val[len-1]='\0'; // erasing the \n character
    insert(element,val);
  }
  fclose(f);
  f=0;
  /*
   * usar ctype.h
   * isalnum(int)
   *
    std::string comando;
  std::string valor;
  char com_val=1;
  for(unsigned short j=0;j<linea.size();j++){
    if(linea[j]==' ') continue;
    if(linea[j]=='='){ com_val=0; continue; }
    if(com_val) comando+=linea[j];
      else valor+=linea[j];
  }
  asign(comando,valor);
  */

  return true;
}

void Configuration::deleteList()
{
    list_elements_t *tmp;
    while (myList != 0) {
      tmp = myList;
      myList = myList->next;
      delete tmp;
    }
}

bool Configuration::readFile()
{

  if (f==NULL) { // if is Null then it doesn't exist (1)
    FOPEN(f, myDataFile,"w+");
    if (f==NULL) { //  if is Null maybe the direcotory doesn't exist (2)
#ifdef WIN32 
      CreateDirectoryA(myDataDir, NULL);
#else
      // Create a directory in linux
      mkdir(myDataDir, 0755);
#endif
      FOPEN(f, myDataFile,"w+");
      if (f==NULL) {
        // The file can not be created.
        return false;
      } else {
        // The file can be read.
        return createANewConfig();
      }
    } else { // if f == NULL (2)
      return createANewConfig();
    }
  } // if f == NULL (1)

  parseFile();

  return true;
}

void Configuration::get(char *val, const char *attr)
{
  list_elements_t *tmp;

  tmp = myList;
  val[0]='\0';

  while (tmp != 0) {
    if ( ! strcmp(tmp->element, attr) ) {
      STRCPY(val, VAL_LEN, tmp->val);
      break;
    }
    tmp = tmp->next;
  }
}

void Configuration::setDefaultNewFileData(char *fileContent)
{
  if (fileContent)
  {
    strConfigData = fileContent;
  } else {
    strConfigData = 0;
  }
}


