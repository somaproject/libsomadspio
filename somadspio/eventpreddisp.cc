#include "eventpreddisp.h"


namespace somadspio {



  EventPredicateDispatch::EventPredicateDispatch( eventtxlist_sender_t   sender) :
    eventsender_(sender), 
    ispending_(false), 
    timeout_(0), 
    timenow_(0), 
    sendtime_(0)
  {

  }

  size_t EventPredicateDispatch::queueSize()
  {
    return queue_.size(); 
  }
  
  void EventPredicateDispatch::clearQueue()
  {
    queue_.clear(); 
    ispending_ = false; 
  }
  
  void EventPredicateDispatch::setTimeout(uint64_t timeout)
  {
    timeout_ = timeout; 

  }

  void EventPredicateDispatch::setTime(uint64_t t)
  {
    timenow_ = t; 
    if (timeout_ > 0) {
      checkPred(); 
      
    }
  }

  bool EventPredicateDispatch::checkPred()
  {
    if (ispending_) {
      if (queue_.front().second() ) {
	std::cout << "Predicate is true" << std::endl; 
	// predicate is true!
	queue_.pop_front(); 	
	ispending_ = false; 
	return true; 
      } else {
	// check timeout 
	if (timeout_> 0) {
	  if (timenow_ > (sendtime_ + timeout_)) { 
	    std::cerr << "timenow_ = " << timenow_; 
	    std::cerr << " sendtime_ = " << sendtime_; 
	    std::cerr << "TIMEOUT: Abandoning event tx" << std::endl; 
	    queue_.pop_front(); 
	    ispending_ = false; 
	    return true; 
	    
	  }
	}
      }
    } else { 
      if (!queue_.empty()) {
	// send the thing
	std::cout << "sending the event " << queue_.front().first << std::endl; 
	eventsender_(queue_.front().first); 
	sendtime_ = timenow_; 

	ispending_ = true; 
      }
    }
    
    return false; 
  }
  
  void EventPredicateDispatch::submit(const sn::EventTXList_t & el, 
				      boost::function<bool () > pred)
  {
    queue_.push_back(std::make_pair(el, pred)); 
    checkPred(); 

  }


}
