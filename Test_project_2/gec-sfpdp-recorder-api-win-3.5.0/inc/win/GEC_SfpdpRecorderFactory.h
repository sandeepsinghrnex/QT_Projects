#ifndef GEC_SFPDPRECORDERFACTORY_H_
#define GEC_SFPDPRECORDERFACTORY_H_

#include "GEC_ISfpdpRecorder.h"

class GEC_SfpdpRecorderFactory
{
public:
  /**
    Creates an instance of the GEC_ISfpdpRecorder interface 
    that enables a client to communicate with a server over TCP

    On Windows the client is required to call WSAStartup() before
    calling this method.

    @param address 
    String with IP address of the server

    @return
    Pointer to instance of GEC_ISfpdpRecorder
  */
  static GEC_ISfpdpRecorder* createTcpBasedInstance(std::string address);

  /**
    Destroys an instance of the GEC_ISfpdpRecorder created by @ref createTcpBasedInstance()

    @param instance
    Pointer to instance of GEC_ISfpdpRecorder
  */
  static void destroyTcpBasedInstance(GEC_ISfpdpRecorder* instance);

};

#endif GEC_SFPDPRECORDERFACTORY_H_
