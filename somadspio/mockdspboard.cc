
#include "mockdspboard.h"



#include "hostdataout.h"
#include <iostream>
#include "dspboard/mainloops/rawmainloop.h"
#include "dspboard/mainloops/somamainloop.h"
#include "dspfixedconfig.h"
#include "dspboard/event.h"
#include "dspboard/echoproc.h"
#include "dspboard/systemtimer.h"
#include "dspboard/filterio.h"
#include "dspboard/eventdispatch.h"
#include "dspboard/systemtimer.h"
#include "dspboard/acqdatasource.h"
#include "dspboard/sinks/rawsink.h"
#include "dspboard/host/dspfixedconfig.h"
#include "dspboard/host/hw/acqserial.h" 

#include "dspboard/eventutil.h" 
#include "dspboard/mainloops/somamainloop.h"

namespace somadspio { namespace mock { 

void dspboard_run(MockDSPBoard & dspboard, int iters) {
  for (int i = 0; i < iters; i++) {
    dspboard.runloop(); 
  }

}

MockDSPBoard::MockDSPBoard(char dsrc, dsp::eventsource_t esrc):
  dsrc_(dsrc), 
  esrc_(esrc), 
  timer(new SystemTimer()), 
  dataout(new HostDataOut()), 
  config(new DSPFixedConfig(DSPA, dsrc_, esrc_)), 
  ed(new EventDispatch(config->getDSPPos())), 
  eventtx(new EventTX()), 
  acqserial(new AcqSerial(false)), 
  bm(new Benchmark()), 
  eep( new EventEchoProc(ed, eventtx, timer, bm, config->getEventDevice())), 
  mainloop(new SomaMainLoop())
  //  sp(dsrc_, sigc::mem_fun(*this, &MockDSPBoard::sendEvents)))
{
  timer->setTime(0); 
  mainloop->setup(ed, eventtx, acqserial, timer, eep, 
		  dataout, config); 
  acqserial->linkUpState_ = true; 
  
}

void MockDSPBoard::setEventTXCallback(sigc::slot<void, somanetwork::EventTX_t> eventcb)
{
  eventcb_ = eventcb; 
}

void MockDSPBoard::sendEvents(const somanetwork::EventTXList_t & etxl)
{
  /* 
     this corresponds to one eventcycle's worth of events

     We use the EventTX so that we can construct the appropriate input
     buffer.


  */ 
  for (somanetwork::EventTXList_t::const_iterator etx = etxl.begin(); 
       etx != etxl.end(); ++etx) { 
    // convert from network EventTX to dsp EventTX
    dsp::Event_t detx; 
    detx.cmd = etx->event.cmd; 
    detx.src = etx->event.src; 
    for(int i = 0; i < 5; i++) {
      detx.data[i]= etx->event.data[i]; 
    }
    
    // send the actual event to the mock DSP
    std::vector<bool> amask, bmask, cmask, dmask; 
    std::vector<dsp::Event_t> events; 
    for (int i = 0; i < 80; i++) {
      amask.push_back(false); 
      bmask.push_back(false); 
      cmask.push_back(false); 
      dmask.push_back(false); 
      events.push_back(dsp::Event_t()); 
    }
    amask[2] = true; 
    events[2] = detx; 
    uint16_t * buf = createEventBuffer(amask, bmask, cmask, dmask, events); 
    ed->parseECycleBuffer(buf); 
    delete[] buf;
  }
}

void MockDSPBoard::addSamples(const boost::array<int16_t, 10> & samples)
{

  std::vector<int16_t> buf; 
  for (int i = 0; i < samples.size(); i++) {
    buf.push_back(samples[i]); 
  }
  acqserial->appendSamples(buf); 

}

void MockDSPBoard::runloop()
{
  std::cout << "MockDSPBoard::runloop() " << std::endl; 
  mainloop->runloop(); 
  
  if (eventtx->eventBuffer_.size() > 0) {
    // convert 
    dsp::EventTX_t etx = eventtx->eventBuffer_.front(); 
    std::cout << "spot 1" << std::endl; 
    somanetwork::EventTX_t snetx; 

    for (int i = 0; i < somanetwork::ADDRBITS; i++) {
      int word = i >> 3; 
      snetx.destaddr[i] = etx.addr[word] >> (i % 8); 
    }
    std::cout << "spot 2" << std::endl; 

    snetx.event.cmd = etx.event.cmd; 
    snetx.event.src = etx.event.src; 
    for(int i =0; i < 5; i++) {
      snetx.event.data[i] = etx.event.data[i]; 
    }
    
    std::cout << "spot 3" << std::endl; 
    eventcb_(snetx); 
    eventtx->eventBuffer_.pop_front(); 
    
  }
  std::cout << "spot 4" << std::endl; 
  for (int i = 0; i < 10; i++) {
    ed->dispatchEvents(); 
  }
  std::cout << "spot 5" << std::endl; 
}
    
  
}
}
