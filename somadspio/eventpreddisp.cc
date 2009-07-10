#include "eventpreddisp.h"


namespace somadspio {



  EventPredicateDispatch::EventPredicateDispatch( eventtxlist_sender_t   sender) :
    eventsender_(sender), 
    ispending_(false)
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
	std::cout << "predicate not yet true "
		  << queue_.size() << std::endl; 

      }
    } else { 
      if (!queue_.empty()) {
	// send the thing
	eventsender_(queue_.front().first); 
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
