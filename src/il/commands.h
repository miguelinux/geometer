/*! \file commands.h
 *  This is the header file where is defined the class StreamSocket
 */
#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef IL_COMMANDS_H_
#define IL_COMMANDS_H_

namespace il {

/// A class containing names for most robot microcontroller system commands
/**
   A class with an enum of the commands that can be sent to the robot, see the 
   robot operations manual for more detailed descriptions.
*/
  class Commands {
    public:
      enum Codes { 
        PULSE = 0, ///< none, keep alive command, so watchdog doesn't trigger
        OPEN = 1, ///< none, sent after connection to initiate connection
        CLOSE = 2, ///< none, sent to close the connection to the robot
        MOVE = 8, ///< int, translational move (mm)
        SIM_EXIT = 239,         ///< int, Exit the simulator. Argument is the exit code (use 0 for a "normal" exit).

        CAMERA = 1000, ///<int, 1 open camera, 0 close camera
        IMAGE = 1001, ///<int, get image from one camera. Argument is the number of camera
        STEREO_IMG = 1002, ///<none, get stereo image 
        IMG_PROP = 1003, ///<short,short, get the image properties (width,height)

        LASER = 1100, ///<note, get laser data

        MESSAGE = 0xFFFD ///<int, An message. Argument is the message size
        ERROR = 0xFFFE ///<int, Error message. Argument is the message size
        SERVER_EXIT = 0xFFFF ///<none, Stop server running
     };

  }; // class Commands
}; //namespace il

#endif

