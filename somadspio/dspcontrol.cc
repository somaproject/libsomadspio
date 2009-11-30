#include <somadspio/dspcontrol.h>
#include <somadspio/acqdatasource.h>
#include <somadspio/logging.h>
#include <boost/lexical_cast.hpp>

namespace somadspio {
  using namespace somanetwork; 
  
  StateProxy::StateProxy(datasource_t dsrc, eventtxlist_sender_t etgt, uint64_t timeout) :
    dsrc_(dsrc), 
    src_(dsrc + 0x08),
    eventTX_(etgt), 
    preddisp_(etgt),
    acqdatasrc(*this), 
    tspikesink(*this), 
    wavesink(*this)
  {
    preddisp_.setTimeout(timeout); 
    DSPIOL_(info) << "StateProxy[" << (unsigned int) dsrc_ << "] : constructing for data source = "
		  << (unsigned int) dsrc_ << " with timeout = " << timeout; 

  }
  
  StateProxy::~StateProxy()
  {
    if(preddisp_.queueSize() > 0) {
      DSPIOL_(warning) << "StateProxy[" << (unsigned int) dsrc_ << "]: warning, predicate queue still has "
		       << preddisp_.queueSize() << " items"; 

      std::list<sn::EventTXList_t> out = preddisp_.getQueueEvents(); 
      for(std::list<sn::EventTXList_t>::iterator ei = out.begin(); 
	  ei != out.end(); ++ei)
	{
	  DSPIOL_(warning) << "StateProxy: " 
			   << *ei; 

	}
    }

  }

  void StateProxy::setTime(uint64_t t)
  {
    preddisp_.setTime(t); 
    DSPIOL_(debug) << "StateProxy[" << (unsigned int) dsrc_ << "]: setTime time = " 
	      << t; 

  }

  void StateProxy::newEvent(const Event_t & event) {
    // FIXME: WARNING LINEAR TIME
    using namespace somanetwork; 
    DSPIOL_(debug) << "StateProxy[" << (unsigned int) dsrc_ << "]: new event ";
    boost::lexical_cast<std::string>(event); 


    acqdatasrc.newEvent(event); 
    tspikesink.newEvent(event); 
    wavesink.newEvent(event); 
    preddisp_.checkPred(); 
    
  }
  
  void StateProxy::setETXDest(EventTX_t &  etx)
  {
    etx.destaddr[src_] = true; 
    etx.event.src = src_; 
  }

  void StateProxy::submit(const sn::EventTXList_t & el, boost::function<bool () > pred) 
  {
    using namespace somanetwork; 

    DSPIOL_(debug) << "StateProxy[" << (unsigned int) dsrc_ << "]: submitting event tx "; 
    //boost::lexical_cast<std::string>(el); 

    preddisp_.submit(el, pred); 
  }


  sn::EventTXList_t createList(const sn::EventTX_t & et) {
    sn::EventTXList_t el; 
    el.push_back(et); 
    return el; 

  }

}
