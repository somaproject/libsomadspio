#include <somadspio/tspikesink.h>
#include <somadspio/eventcodec.h>
#include <somadspio/dspcontrol.h>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace somadspio { 

  namespace tspike = codec::TSpikeSink; 

  using namespace boost::phoenix;

  TSpikeSink::TSpikeSink(StateProxy & sp) : 
    parent_(sp)
  {
    

  }
  
  bool TSpikeSink::newEvent(const sn::Event_t & event)
  {
    switch(event.cmd) {
    case tspike::RESPBCAST :
      parseEvent(event); 
      return true; 
    default:
      return false; 
    }
       
  }

  void TSpikeSink::parseEvent(const sn::Event_t & event)
  {

    tspike::PARAMETERS p = tspike::whichParam(event); 
    switch(p) {
    case tspike::THRESHOLD:
      {
	tspike::chanthold_t thold = tspike::changeThreshold(event); 
	if (tholds_[thold.first] != thold.second) {
	  tholds_[thold.first] = thold.second; 
	  tholdSignal_.emit(thold.first, thold.second); 
	}
      }
      break; 

    case tspike::FILTERID:
      {
	tspike::chanfiltid_t filtid = tspike::changeFilterID(event); 
	if (filterids_[filtid.first] != filtid.second) {
	  filterids_[filtid.first] = filtid.second; 
	  filterIDSignal_.emit(filtid.first, filtid.second); 
	}
      }
      break; 

    }
    
  }
  

  sigc::signal<void, int, int> & TSpikeSink::thold()
  {
    return tholdSignal_; 
  }

  void TSpikeSink::setThold(int chan, int thold)
  {
    tspike::chanthold_t ct; 
    ct.first = chan; 
    ct.second = thold; 
    sn::EventTX_t etx = tspike::changeThreshold(ct); 
    parent_.setETXDest(etx); 
    parent_.submit(createList(etx), ref(tholds_[chan]) == thold); 

  }

  int TSpikeSink::getThold(int chan)
  {
    return tholds_[chan];
  }
  
  sigc::signal<void, int, filterid_t> & TSpikeSink::filterID()
  {
    return filterIDSignal_; 
  }

  void TSpikeSink::setFilterID(int chan, filterid_t filterID)
  {
    tspike::chanfiltid_t cfid; 
    cfid.first = chan; 
    cfid.second = filterID; 
    sn::EventTX_t etx = tspike::changeFilterID(cfid); 
    parent_.setETXDest(etx); 
    parent_.submit(createList(etx), ref(filterids_[chan]) == filterID); 

  }

  filterid_t TSpikeSink::getFilterID(int chan)
  {
    return filterids_[chan];

  }

}
