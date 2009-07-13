#include <boost/test/auto_unit_test.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/fstream.hpp"   
#include <iostream>
#include <vector>
#include <list> 
#include <sigc++/sigc++.h>
#include <somadspio/types.h>
#include <somadspio/dspcontrol.h>

#include <somadspio/eventpreddisp.h>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/home/phoenix/function/function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

BOOST_AUTO_TEST_SUITE(test_eventpred); 

using namespace somanetwork; 
using namespace somadspio; 

//std::list<EventTXList_t> tgtlist; 
void sender_to_list(const EventTXList_t & l, 
		    std::list<EventTXList_t> * tgtlist ) {
  std::cout << "sender_to_list called" << std::endl; 
  tgtlist->push_back(l); 

}

// BOOST_AUTO_TEST_CASE(simple)
// {
//   std::list<EventTXList_t> * etxl_list = new std::list<EventTXList_t>(); 
//   EventPredicateDispatch epd(sigc::bind(sigc::ptr_fun(sender_to_list),etxl_list)); 
  
//   BOOST_CHECK_EQUAL(epd.queueSize(), 0); 
  
// }

// BOOST_AUTO_TEST_CASE(dispatch)
// {
//   using namespace boost::phoenix;

//   std::list<EventTXList_t> * etxl_list = new std::list<EventTXList_t>(); 
//   EventPredicateDispatch epd(sigc::bind(sigc::ptr_fun(sender_to_list),
// 					etxl_list)); 
  
//   BOOST_CHECK_EQUAL(epd.queueSize(), 0); 
  
//   EventTX_t etx; 
//   etx.event.cmd = 0x10; 
//   int queuepos = 0; 
  
//   epd.submit(createList(etx), ref(queuepos) == 5); 
//   epd.submit(createList(etx), ref(queuepos) == 10); 
//   BOOST_CHECK_EQUAL(epd.queueSize(), 2); 

//   for (int i = 0; i < 15; i++) {
//     queuepos = i; 
//     epd.checkPred(); 
//   }
  
//   BOOST_CHECK_EQUAL(epd.queueSize(), 0); 
//   BOOST_CHECK_EQUAL(etxl_list->size(), 2); 

// }

BOOST_AUTO_TEST_CASE(timeout)
{
  /*  test timeout 
   */
  
  
  using namespace boost::phoenix;

  std::list<EventTXList_t> * etxl_list = new std::list<EventTXList_t>(); 
  EventPredicateDispatch epd(sigc::bind(sigc::ptr_fun(sender_to_list),
					etxl_list)); 
  
  BOOST_CHECK_EQUAL(epd.queueSize(), 0); 
  
  EventTX_t etx; 
  etx.event.cmd = 0x10; 
  int queuepos = 0; 
  
  epd.setTimeout(10); 
  
  epd.submit(createList(etx), ref(queuepos) == 5); 
  epd.submit(createList(etx), ref(queuepos) == 0); // this one should time out
  epd.submit(createList(etx), ref(queuepos) == 20); 

  BOOST_CHECK_EQUAL(epd.queueSize(), 3); 

  for (int i = 0; i < 30; i++) {
    epd.setTime(i); 
    queuepos = i; 
    epd.checkPred(); 
  }
  
  BOOST_CHECK_EQUAL(epd.queueSize(), 0); 
  BOOST_CHECK_EQUAL(etxl_list->size(), 3); 



}


BOOST_AUTO_TEST_SUITE_END(); 

