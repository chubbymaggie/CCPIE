#include <iomanip>

#include <boost/log/expressions.hpp>

#include <boost/log/support/date_time.hpp>

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "pie/Log.h"

using namespace boost::log;

namespace expr = expressions;

BOOST_LOG_GLOBAL_LOGGER_INIT(pie_logger, pie_logger_t) {
  pie_logger_t logger;
  add_common_attributes();

  /* Only show warnings or worse on console */
  add_console_log()->set_filter(
      expr::attr<trivial::severity_level>("Severity") >= trivial::warning);

  /* But log everything to a file */
  add_file_log(keywords::file_name = "pie.%Y%m%d.%H%M.log",
               keywords::auto_flush = true)
      ->set_formatter(expr::stream
                      << expr::format_date_time<boost::posix_time::ptime>(
                             "TimeStamp", "%Y-%m-%d  %H:%M:%S.%f")
                      << " |"
                      << std::setw(7)
                      << expr::attr<trivial::severity_level>("Severity")
                      << " > "
                      << expr::message);

  return logger;
}
