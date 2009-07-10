#include <somadspio/dspcontrol.h>
#include <somadspio/acqdatasource.h>


namespace somadspio {
  using namespace somanetwork; 
  
  StateProxy::StateProxy(datasource_t dsrc, eventtxlist_sender_t etgt) :
    dsrc_(dsrc), 
    src_(dsrc + 0x08),
    acqdatasrc(*this), 
    tspikesink(*this), 
    wavesink(*this), 
    eventTX_(etgt), 
    preddisp_(etgt)
  {
    std::cout << "Init done" << std::endl; 
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
