#include <somadspio/dspcontrol.h>
#include <somadspio/acqdatasource.h>


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
  }

  void StateProxy::setTime(uint64_t t)
  {
    preddisp_.setTime(t); 

  }

  void StateProxy::newEvent(const Event_t & event) {
    // FIXME: WARNING LINEAR TIME
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
    preddisp_.submit(el, pred); 
  }


  sn::EventTXList_t createList(const sn::EventTX_t & et) {
    sn::EventTXList_t el; 
    el.push_back(et); 
    return el; 

  }

}
