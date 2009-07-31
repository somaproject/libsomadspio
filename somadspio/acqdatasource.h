#ifndef __SOMA_DSPIO_ACQDATASOURCE_H__
#define __SOMA_DSPIO_ACQDATASOURCE_H__

#include <sigc++/sigc++.h>
#include <somanetwork/network.h>
#include <somadspio/types.h>
#include <somadspio/eventcodec.h>

namespace somadspio {

  namespace sn = somanetwork; 

  // basically a proxy for remote state
  class AcqDataSource
  {
  public:
    
    sigc::signal<void, bool > & linkStatus(); 
    boost::optional<bool> getLinkStatus(); 
    
    sigc::signal<void, int> & mode(); 
    boost::optional<int> getMode(); 
    void setMode(int); 
    
    sigc::signal<void, int, int> & gain(); 
    void setGain(int chan, int gain); 
    boost::optional<int> getGain(int chan); 
    
    sigc::signal<void, int, bool> & hpfen(); 
    boost::optional<bool> getHPFen(int chan); 
    void setHPFen(int chan, bool val); 
    
    sigc::signal<void, int, range_t> & range(); 
    boost::optional<range_t> getRange(int chan); 
    // there is no setter for range
    
    sigc::signal<void, int> & chansel(); 
    boost::optional<int> getChanSel(); 
    void setChanSel(int); 
    
    bool newEvent(const sn::Event_t & );
    
  private:
    AcqDataSource(StateProxy & parent); 
    
    StateProxy & parent_; 
    
    static const int CHANCNT = 5; 
    sigc::signal<void, bool> linkStatusSignal_; 
    sigc::signal<void, int> modeSignal_; 
    sigc::signal<void, int, int> gainSignal_; 
    sigc::signal<void, int, bool> hpfenSignal_; 
    sigc::signal<void, int>  chanselSignal_;
    sigc::signal<void, int, range_t> rangeSignal_; 
    
    boost::optional<bool> linkStatus_; 
    boost::optional<int> mode_; 
    boost::array<boost::optional<int>, CHANCNT> gains_; 
    boost::array<boost::optional<bool>, CHANCNT> hpfens_; 
    boost::array<boost::optional<range_t>, CHANCNT> ranges_; 
    boost::optional<int> chansel_; 
    
    friend class StateProxy; 
    void parseEvent(const sn::Event_t & event); 
    
  }; 

}

#endif 
