#ifndef DSP_IOLIB_MOCKDSPBOARD_H
#define DSP_IOLIB_MOCKDSPBOARD_H

#include <filterio.h>
#include <eventdispatch.h>
#include <systemtimer.h>
#include <acqdatasource.h>
#include <sinks/rawsink.h>
#include <hostdataout.h>
#include <vector>
#include <iostream>
#include <mainloops/rawmainloop.h>
#include <mainloops/somamainloop.h>
#include <dspfixedconfig.h>
#include <event.h>
#include <somanetwork/eventtx.h>
#include <sigc++/sigc++.h>
#include <echoproc.h>
#include <systemtimer.h>


  
class SomaMainLoop; 

namespace somadspio { namespace mock { 
  

class MockDSPBoard; 
void dspboard_run(MockDSPBoard & dspboard, int iters); 

class MockDSPBoard
{
public:
  MockDSPBoard(char dsrc, dsp::eventsource_t esrc); 

  void setEventCallback(sigc::slot<void, somanetwork::Event_t> eventcb); 
  void addSamples(std::vector<int16_t> samples);
  
  char dsrc_; 
  dsp::eventsource_t esrc_; 
  SystemTimer timer; 
  HostDataOut dataout; 
  DSPFixedConfig config; 

  EventDispatch ed; 
  EventTX eventtx; 
  
  AcqSerial acqserial; 

  Benchmark bm;
 
  EventEchoProc eep; 
  SomaMainLoop  * mainloop; 

  void runloop(); 
  void sendEvents(const somanetwork::EventTXList_t & etxl); 

  std::list<dsp::EventTX_t> events; 
  sigc::slot<void, somanetwork::Event_t> eventcb_; 

}; 

  }
}
#endif // MOCKDSPBOARD_H
