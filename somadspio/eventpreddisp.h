#ifndef __SOMA_DSPIO_EVENTPREDDISP_H__
#define __SOMA_DSPIO_EVENTPREDDISP_H__

#include <boost/function.hpp>
#include <list> 
#include <somadspio/types.h>


namespace somadspio {
  /*
    Event predicate dispatch takes in a list of events to send
    and a predicate, and only moves on to the -next- event in the 
    queue once the predicate evaluates to true. 

    We make use of some of the more functional boost:: components
    to have nice declaration of predicates in the code. 

    This lets us get around the problem of sending a bunch of commands
    that all take a while to execute, and never hearing back. 


   */

  class EventPredicateDispatch
  {
  public:
    
    EventPredicateDispatch( eventtxlist_sender_t   sender); 
    void clearQueue(); 
    size_t queueSize(); 
    bool checkPred(); 
    void submit(const sn::EventTXList_t & el, boost::function<bool () > pred); 

  private:
    
    const eventtxlist_sender_t  eventsender_; 
    typedef std::pair<sn::EventTXList_t, boost::function<bool () > > queuepred_t; 
    typedef std::list<queuepred_t> queue_t; 
    
    queue_t queue_; 
    bool ispending_; 
    
    
  }; 


}


#endif 
