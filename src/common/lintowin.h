
#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef _LIN_TO_WIN_H_
#define _LIN_TO_WIN_H_

#ifdef WIN32

#define STRCPY(dest,len,src) strcpy_s(dest,len,src)
#define STRNCAT(dest,src,len) strcat_s(dest,len,src)

#define FOPEN(fd,filename,mode) fopen_s(&fd,filename,mode)

#define SPRINTF(dest,len,src,var) sprintf_s(dest,len,src,var)

#else

#define STRCPY(dest,len,src) strncpy(dest,src,len)
#define STRNCAT(dest,src,len) strncat(dest,src,len)

#define FOPEN(fd,filename,mode) fd=fopen(filename,mode)

#define SPRINTF(dest,len,src,var) sprintf(dest,src,var)

#endif

#endif

