#include <boost/test/auto_unit_test.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/fstream.hpp"   
#include <iostream>
#include <vector>
#include <list> 


#include <somadspio/dspcontrol.h>
#include <somadspio/mockdspboard.h>
#include <ext-dspboard/src/host/hw/acqserial.h>


BOOST_AUTO_TEST_SUITE(test_acqdatasource); 

using namespace somadspio::mock; 

BOOST_AUTO_TEST_CASE(gainset_test)
{
  /*
    try setting the gain

  */ 

  somadspio::mock::MockDSPBoard dspboard(0, 8); 
  dspboard.acqserial->linkUpState_ = true;
  somadspio::StateProxy stateproxy(0, sigc::mem_fun(dspboard,
						    &MockDSPBoard::sendEvents), 0); 
  dspboard.setEventCallback(sigc::mem_fun(stateproxy,
					  &somadspio::StateProxy::newEvent)); 
  
  dspboard_run(dspboard, 10000); 
  
  // check if the link status update was correctly... uh, updated
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getLinkStatus(), true); 
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getMode(), 0); 
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getGain(0), 0); 
  
  int gains[] = {0, 100, 200, 500, 1000, 2000, 5000, 10000}; 
  // now try setting the gain 
  for (int i = 0; i < 7; i++) {
    stateproxy.acqdatasrc.setGain(0, gains[i]); 
    dspboard_run(dspboard, 10000); 
    BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getGain(0), gains[i]); 
    BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getRange(0).first, 
		      AcqState::RANGEMIN[i]); 
    BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getRange(0).second, 
		      AcqState::RANGEMAX[i]); 
  }
  
}

BOOST_AUTO_TEST_CASE(gainset_test_fast)
{
  /*
    try setting the gain rapidly, and seeing if we get a result

  */ 

  somadspio::mock::MockDSPBoard dspboard(0, 8); 
  dspboard.acqserial->linkUpState_ = true;
  somadspio::StateProxy stateproxy(0, sigc::mem_fun(dspboard,
						    &MockDSPBoard::sendEvents), 0); 
  dspboard.setEventCallback(sigc::mem_fun(stateproxy,
					  &somadspio::StateProxy::newEvent)); 
  
  dspboard_run(dspboard, 10000); 
  
  // check if the link status update was correctly... uh, updated
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getLinkStatus(), true); 
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getMode(), 0); 
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getGain(0), 0); 
  
  int gains[] = {0, 100, 200, 500, 1000, 2000, 5000, 10000}; 
  // now try setting the gain 
  for (int i = 0; i < 5; i++) {
    stateproxy.acqdatasrc.setGain(i, gains[i]); 
    dspboard_run(dspboard, 10); 
  } 

  dspboard_run(dspboard, 100000); 
  for (int i = 0; i < 5; i++) {
    BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getGain(i), gains[i]); 
    BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getRange(i).first, 
		      AcqState::RANGEMIN[i]); 
    BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getRange(i).second, 
		      AcqState::RANGEMAX[i]); 
  }
  
}

BOOST_AUTO_TEST_CASE(hpfset_test)
{
  /*
    try setting the hpf

  */ 

  somadspio::mock::MockDSPBoard dspboard(0, 8); 
  dspboard.acqserial->linkUpState_ = true;
  somadspio::StateProxy stateproxy(0, sigc::mem_fun(dspboard,
						    &MockDSPBoard::sendEvents), 0); 
  dspboard.setEventCallback(sigc::mem_fun(stateproxy,
					  &somadspio::StateProxy::newEvent)); 
  
  dspboard_run(dspboard, 10000); 
  
  // check if the link status update was correctly... uh, updated
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getLinkStatus(), true); 
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getMode(), 0); 
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getHPFen(0), false); 
  
  bool hpfs[] = {true, false, true, false, true}; 
  // now try setting the gain 
  for (int i = 0; i < 5; i++) {
    stateproxy.acqdatasrc.setHPFen(0, hpfs[i]); 
    dspboard_run(dspboard, 10000); 
    BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getHPFen(0), hpfs[i]); 
  }
  
}

BOOST_AUTO_TEST_CASE(inputsel_test)
{
  /*
    try setting the input selection

  */ 

  somadspio::mock::MockDSPBoard dspboard(0, 8); 
  dspboard.acqserial->linkUpState_ = true;
  somadspio::StateProxy stateproxy(0, sigc::mem_fun(dspboard,
						    &MockDSPBoard::sendEvents), 0); 
  dspboard.setEventCallback(sigc::mem_fun(stateproxy,
					  &somadspio::StateProxy::newEvent)); 
  
  dspboard_run(dspboard, 10000); 
  
  // check if the link status update was correctly... uh, updated
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getLinkStatus(), true); 
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getMode(), 0); 
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getGain(0), 0); 
  BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getChanSel(), 0); 

  
  
  // now try setting the gain 
  for (int i = 0; i < 5; i++) {
    stateproxy.acqdatasrc.setChanSel(i); 
    dspboard_run(dspboard, 10000); 
    BOOST_CHECK_EQUAL(stateproxy.acqdatasrc.getChanSel(), i); 
  }
  
}



BOOST_AUTO_TEST_SUITE_END(); 
