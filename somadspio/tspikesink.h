#ifndef __SOMA_DSPIO_TSPIKESINK_H__
#define __SOMA_DSPIO_TSPIKESINK_H__

#include <sigc++/sigc++.h>
#include <somanetwork/network.h>
#include <somadspio/types.h>
#include <somadspio/eventcodec.h>

namespace somadspio { 
  
  class TSpikeSink
  {
  public:
    static const int CHANN = 4; 
    
    sigc::signal<void, int, int> & thold(); 
    void setThold(int chan, int thold); 
    boost::optional<int> getThold(int chan); 
    
    sigc::signal<void, int, filterid_t> & filterID(); 
    void setFilterID(int chan, filterid_t filterID); 
    boost::optional<filterid_t> getFilterID(int chan); 
    
  private:
    TSpikeSink(StateProxy & ); 
    
    sigc::signal<void, int, int32_t> tholdSignal_; 
    sigc::signal<void, int, uint32_t> filterIDSignal_; 
    
    friend class StateProxy; 
    bool newEvent(const sn::Event_t & );
    StateProxy & parent_;
    boost::array<boost::optional<int32_t>, CHANN>  tholds_; 
    boost::array<boost::optional<filterid_t>, CHANN> filterids_; 
    void parseEvent(const sn::Event_t & event); 
    
  }; 
  

}

#endif 
