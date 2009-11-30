#include <somadspio/tspikesink.h>
#include <somadspio/eventcodec.h>
#include <somadspio/dspcontrol.h>
#include <somadspio/logging.h>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace somadspio {

  namespace tspike = codec::TSpikeSink; 

  using namespace boost::phoenix;
  namespace p = boost::phoenix; 

  TSpikeSink::TSpikeSink(StateProxy & sp) : 
    parent_(sp)
  {
    DSPIOL_(info) << "TSpikeSink[" << (unsigned int)parent_.dsrc_ << "]: sending initialization query set"; 
    
    for (int i = 0; i < 4; i++) {
      sn::EventTX_t etx = tspike::queryThreshold(i); 
      parent_.setETXDest(etx); 
      parent_.submit(createList(etx), val(true)); 
    }
      
    for (int i = 0; i < 4; i++) {
      sn::EventTX_t etx = tspike::queryFilterID(i); 
      parent_.setETXDest(etx); 
      parent_.submit(createList(etx), p::ref(filterids_[i])); 
    }
      
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
	int chan = thold.first; 
	int val = thold.second; 

	DSPIOL_(info) << "TSpikeSink[" << (unsigned int)parent_.dsrc_ << "]: received updated threshold for chan "
		      << chan << ", value=" << val; 
	
	if (tholds_[chan] != val) {
	  tholds_[chan] = val; 
	  tholdSignal_.emit(chan, val); 
	}
      }
      break; 

    case tspike::FILTERID:
      {
	tspike::chanfiltid_t filtid = tspike::changeFilterID(event); 
	DSPIOL_(info) << "TSpikeSink[" << (unsigned int)parent_.dsrc_ << "]: received updated filterid for chan "
		 << filtid.first << ", value=" << filtid.second; 

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

    DSPIOL_(info) << "TSpikeSink[" << (unsigned int)parent_.dsrc_ << "]: setting threshold for chan "
	     << chan << ", value=" << thold; 

    parent_.submit(createList(etx), p::ref(tholds_[chan]) == thold); 

  }

  boost::optional<int> TSpikeSink::getThold(int chan)
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

    DSPIOL_(info) << "TSpikeSink[" << (unsigned int)parent_.dsrc_ << "]: setting FilterID for chan "
	     << chan << ", value=" << filterID ;

    parent_.submit(createList(etx), p::ref(filterids_[chan]) == filterID); 

  }

  boost::optional<filterid_t> TSpikeSink::getFilterID(int chan)
  {
    return filterids_[chan];

  }

}
