/*! \file qtaction.h
 *   This is the header file where is defined the class QtAction
 */
#ifndef _QT_ACTION_H
#define _QT_ACTION_H

#include <qaction.h>

//!  QtAction class.
/*!
 *   This will help to reduce the code using only one method called
 *   with parameters in it.
 */
class QtAction : public QAction
{

  Q_OBJECT
	
  private:
	
   //! Pixel buffer member.
   /*! This is where the pixel will be stored in raw format.
    *  \sa setParameter().
    */
   unsigned int parameter;

  public:

   //! The constructor of the QtAction class.
   /*!
    *  Here is where the values of variables are initialized.
    *  \param text to be displayed
    *  \param parent widget that has this action
    *  \param shortcut is the key value of the shortcut
    */
   QtAction(QString text, QObject *parent, int shortcut = 0);
	
   //! The destructor of the QtAction class.
   /*!
    *  Clear all the used variables.
    */
   ~QtAction();

   //! Set the paramter of the Action
   /*!
    *  Asign the parameter of the action to send it with
    *  the signal someAction().
    *  \sa someAction(), anyAction().
    */
   void setParameter(unsigned int param);

  public slots:

   //! Slot to capture the default action.
   /*! With this slot one can atach the default
    *  action (triggered Qt4, activated Qt3) and
    *  then emit a signal you want, in this case
    *  someAction() is emited.
    *  \sa someAction()
    */
   void anyAction();

  signals:

  //! Signal emited with parameter.
  /*! This signal is emited with special parameter
   *  before seted. The parameter is asigned by
   *  setParameter().
   *  \sa sparamete, retParameter(), someAction().
   */
  void someAction(unsigned int param);

};

#endif /* _QT_ACTION_H */

