#ifndef SOMADSPIO_LOGGING_H
#define SOMADSPIO_LOGGING_H

#include <boost/logging/format_fwd.hpp>
#include <boost/logging/format/named_write.hpp>

using namespace boost::logging::scenario::usage;
typedef use<
        //  the filter is always accurate (but slow)
        filter_::change::always_accurate, 
        //  filter does not use levels
        filter_::level::use_levels, 
        // the logger is initialized once, when only one thread is running
        logger_::change::set_once_when_one_thread, 
        // the logger favors speed (on a dedicated thread)
        logger_::favor::speed> somadspio_finder;




BOOST_DECLARE_LOG_FILTER(somadspio_log_level, somadspio_finder::filter ) // holds the application log level
BOOST_DECLARE_LOG(somadspio_l, somadspio_finder::logger)

namespace somadspio
{
#define DSPIOL_(lvl) BOOST_LOG_USE_LOG_IF_LEVEL(somadspio_l(), somadspio_log_level(), lvl )

  void init_logs( boost::logging::level::type level); 

}


#endif // SOMADSPIO_LOGGING_H
