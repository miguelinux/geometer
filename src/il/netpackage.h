/*! \file netpackage.h
 *  This is the header file where is defined the class StreamSocket
 */
#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef IL_NET_PACKAGE_H_
#define IL_NET_PACKAGE_H_

namespace il {

  struct NetPackage {
    int commnad;
    union {
      char  cParam[4];
      short sParam[2];
      int   iParam;
      float fParam;
    };
  };

}; // namespace il

#endif

