#include "somadspio/logging.h"
#include <boost/logging/format.hpp>
#include <boost/logging/format/formatter/tags.hpp>
#include <boost/logging/format/named_write.hpp>
#include <boost/logging/format/formatter/tags.hpp>
#include <boost/logging/format/formatter/high_precision_time.hpp>

  //using namespace boost::logging;
namespace bl = boost::logging; 



BOOST_DEFINE_LOG_FILTER(somadspio_log_level, somadspio_finder::filter ) // holds the application log level
BOOST_DEFINE_LOG(somadspio_l, somadspio_finder::logger); 

namespace somadspio {
    
  void init_logs( boost::logging::level::type level)  {
    somadspio_log_level()->set_enabled(level); 

    somadspio_l()->writer().add_formatter(bl::formatter::high_precision_time("$mm.$ss:$micro ")); 
    somadspio_l()->writer().add_formatter(bl::formatter::append_newline()); 
    somadspio_l()->writer().add_destination( bl::destination::cout() );
    somadspio_l()->mark_as_initialized();

  }

}


