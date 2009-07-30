#ifndef DSP_IOLIB_MOCKDSPBOARD_H
#define DSP_IOLIB_MOCKDSPBOARD_H


#include <sigc++/sigc++.h>
#include <vector>
#include <list>
#include <somanetwork/eventtx.h>
#include "dspboard/event.h"

namespace dspboard { 
  // forward declarations
  class HostDataOut; 
  class DSPFixedConfig; 
  class EventTX; 
  class AcqSerial; 
  class Benchmark; 
  class EventDispatch;
  class SystemTimer; 
  class EventEchoProc; 
  class SomaMainLoop; 
  namespace dsp { 

  }

}

namespace somadspio { namespace mock { 
    using namespace dspboard; 
    
    class MockDSPBoard; 
    void dspboard_run(MockDSPBoard & dspboard, int iters); 
    
    class MockDSPBoard
    {
    public:
      MockDSPBoard(char dsrc, dsp::eventsource_t esrc); 

      // primary public interface
      void setEventTXCallback(sigc::slot<void, somanetwork::EventTX_t> eventcb);
      void setDataTXCallback(sigc::slot<void, unsigned char *> cb); 
 
      // this callback is what's used to send _out_ events

      void addSamples(const boost::array<int16_t, 10> & samples);

      // send events -to- the dspboard -- not clear why we have
      // a separate EventTX_t and regular Event_t list version. 
      void sendEvents(const somanetwork::EventTXList_t & etxl); 
  
      double getSignalScale(int chan); 

      char dsrc_; 
      dsp::eventsource_t esrc_; 
      SystemTimer * timer; 
      HostDataOut * dataout; 
      DSPFixedConfig * config; 
      
      EventDispatch * ed; 
      EventTX * eventtx; 
      
      AcqSerial * acqserial; 
      
      Benchmark * bm;
      
      EventEchoProc * eep; 
      SomaMainLoop  * mainloop; 
      
      void runloop(); 
      
      std::list<dsp::EventTX_t> events; 
      sigc::slot<void, somanetwork::EventTX_t> eventcb_; 
      sigc::slot<void, unsigned char*> databuffercb_; 


      uint16_t * pEventBuffer_; 
    }; 
    
  }
}
#endif // MOCKDSPBOARD_H
