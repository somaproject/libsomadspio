#ifndef __SOMA_DSPIO_WAVESINK_H__
#define __SOMA_DSPIO_WAVESINK_H__

#include <sigc++/sigc++.h>
#include <somanetwork/network.h>
#include <somadspio/types.h>
#include <somadspio/eventcodec.h>

namespace somadspio {

    class WaveSink
    {
    public:
      sigc::signal<void, filterid_t> & filterID(); 
      void setFilterID(filterid_t filterID); 
      filterid_t getFilterID(); 
      
      sigc::signal<void, samprate_t > & sampratenum(); 
      void setSampRateNum(int chan, samprate_t); 
      samprate_t getSampRateNum(); 

    private:
      WaveSink(StateProxy & ); 
      friend class StateProxy; 
      bool newEvent(const sn::Event_t & );
      StateProxy & parent_; 
      
      samprate_t samprate_; 
      filterid_t filterid_; 
      void parseEvent(const sn::Event_t & event); 

    }; 
    
    


}

#endif // __SOMA_DSPIO_WAVESINK_H__

