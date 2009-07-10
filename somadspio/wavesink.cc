#include "wavesink.h"

namespace somadspio {

  WaveSink::WaveSink(StateProxy & sp) :
    parent_(sp)
  {

  }

  bool WaveSink::newEvent(const sn::Event_t & event)
  {

    switch(event.cmd) {
    case codec::WaveSink::RESPBCAST :
      parseEvent(event); 
      return true; 
    default:
      return false; 
    }
    
  }

  void WaveSink::parseEvent(const sn::Event_t & event)
  {


  }

}
