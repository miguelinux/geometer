/*! \file qt3qt4macros.h
 *   This is the header file where is defined the macros used to
 *   define type of variables from Qt4 and Qt3
 */
#ifndef _QT3_QT4_MACROS_H
#define _QT3_QT4_MACROS_H

/*! \def QTX_MENU
 *  Defines a QMenu in Qt4, and a QPopupMenu in Qt3
 */
/*! \def QTX_LATIN1
 *  Defines the function from QString to transform it to a char string.
 */
#if QT_VERSION >= 0x040000
  #define QTX_MENU QMenu
  #define QTX_LATIN1 toLatin1().constData()
#else
  #define QTX_MENU QPopupMenu
  #define QTX_LATIN1 latin1()
#endif /*  QT_VERSION >= 0x040000 */

#endif /* _QT3_QT4_MACROS_H */

