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
    
    sigc::signal<void, bool> & linkStatus(); 
    bool getLinkStatus(); 
    
    sigc::signal<void, int> & mode(); 
    int getMode(); 
    void setMode(int); 
    
    sigc::signal<void, int, int> & gain(); 
    void setGain(int chan, int gain); 
    int getGain(int chan); 
    
    sigc::signal<void, int, bool> & hpfen(); 
    bool getHPFen(int chan); 
    void setHPFen(int chan, bool val); 
    
    sigc::signal<void, int, range_t> & range(); 
    range_t getRange(int chan); 
    // there is no setter for range
    
    sigc::signal<void, int> & chansel(); 
    int getChanSel(); 
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
    
    bool linkStatus_; 
    int mode_; 
    int gains_[CHANCNT]; 
    bool hpfens_[CHANCNT]; 
    range_t ranges_[CHANCNT]; 
    int chansel_; 
    
    friend class StateProxy; 
    void parseEvent(const sn::Event_t & event); 
    
  }; 

}

#endif 
