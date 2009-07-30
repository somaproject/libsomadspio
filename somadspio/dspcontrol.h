#ifndef DSP_IOLIB_CONTROL
#define DSP_IOLIB_CONTROL

#include <sigc++/sigc++.h>
#include <somanetwork/network.h>
#include <somadspio/types.h>
#include <somadspio/acqdatasource.h>
#include <somadspio/tspikesink.h>
#include <somadspio/wavesink.h>
#include <somadspio/eventpreddisp.h> 

/*
  caching of DSPboard state, event parsing and signal dispatch
  goal: 
     events in, state changes and signals out
     
  How do we know which events to deliver to the state proxy? 
  At the moment we're just going to give it all of them from its source
  
  FIXME:: Fix all-event passing someday so we don't do extra work. 

*/ 


namespace somadspio {
  
  class StateProxy {
  public:
    StateProxy(sn::datasource_t dsrc,  eventtxlist_sender_t  etgt, 
	       uint64_t timeout); 
    
    void newEvent(const sn::Event_t & event); 
    void setTime(uint64_t t); 
    sn::datasource_t dsrc_;
    sn::eventsource_t src_; 

    
    void setETXDest(sn::EventTX_t &  etx); 
    void submit(const sn::EventTXList_t & el, boost::function<bool () > pred); 

  private:
    const eventtxlist_sender_t eventTX_; 
    EventPredicateDispatch preddisp_; 
  public:

    AcqDataSource acqdatasrc; 
    TSpikeSink tspikesink; 
    WaveSink wavesink; 

  }; 

  sn::EventTXList_t createList(const sn::EventTX_t & et); 


}


#endif 
