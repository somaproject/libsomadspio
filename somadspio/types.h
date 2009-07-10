#ifndef __SOMA_DSPIO_TYPES__
#define __SOMA_DSPIO_TYPES__

#include <sigc++/sigc++.h>
#include <somanetwork/network.h>

namespace somadspio { 
  
  typedef std::pair<uint32_t, uint32_t> samprate_t;
  typedef uint32_t filterid_t; 
  typedef std::pair<int32_t, int32_t> range_t;
  
  // forward declarations
  class StateProxy; 
  class AcqDataSource; 
  class TSpikeSink; 
  class WaveSink; 

  namespace sn = somanetwork; 

  typedef sigc::slot<void, const sn::EventTXList_t &> eventtxlist_sender_t; 
  
}



#endif // 
