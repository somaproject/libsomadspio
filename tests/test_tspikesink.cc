#include <boost/test/auto_unit_test.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/fstream.hpp"   
#include <iostream>
#include <vector>
#include <list> 


#include <somadspio/dspcontrol.h>
#include <somadspio/mockdspboard.h>
#include <somadspio/logging.h>

#include <ext-dspboard/src/host/hw/acqserial.h>


BOOST_AUTO_TEST_SUITE(test_tspikesink); 

using namespace somadspio::mock; 

void stateProxyCallbackAdaptor(somadspio::StateProxy * sp, 
			       somanetwork::EventTX_t etx)
{
  sp->newEvent(etx.event); 
}

BOOST_AUTO_TEST_CASE(thresholdset_test)
{
  /*
    try setting the gain

  */ 

  //somadspio::init_logs(boost::logging::level::info); 

  somadspio::mock::MockDSPBoard dspboard(0, 8); 
  dspboard.acqserial->linkUpState_ = true;
  somadspio::StateProxy stateproxy(0, sigc::mem_fun(dspboard,
						    &MockDSPBoard::sendEvents), 0); 
  dspboard.setEventTXCallback(boost::bind(&stateProxyCallbackAdaptor, &stateproxy, _1)); 
  
   for (int i =0; i < 1000000; i++) {
     boost::array<int16_t, 10> samples; 
     stateproxy.setTime(0); 
     dspboard.addSamples(samples); 
     dspboard.runloop(); 
   }



   for (int i = 0; i < 3; i++) {
     stateproxy.tspikesink.setThold(i, i *1000 + 2000); 

     for (int j =0; j < 100000; j++) {
       boost::array<int16_t, 10> samples; 
       stateproxy.setTime(0); 
       dspboard.addSamples(samples); 
       dspboard.runloop(); 
     }


     BOOST_CHECK_EQUAL(stateproxy.tspikesink.getThold(i), i * 1000 + 2000);
   }

 }



BOOST_AUTO_TEST_SUITE_END(); 
