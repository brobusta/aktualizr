#ifndef SOTA_CLIENT_TOOLS_LOGGING_H_
#define SOTA_CLIENT_TOOLS_LOGGING_H_

#include <spdlog/spdlog.h>
#include <cstdint>
#include <sstream>

struct LoggerConfig;

class streamable_logger_t {
  spdlog::level::level_enum log_level_;
  std::ostringstream log_stream_;

 public:
  explicit streamable_logger_t(spdlog::level::level_enum log_level) : log_level_(log_level) {}

  template <typename T>
  streamable_logger_t& operator<<(const T& value) {
    log_stream_ << value;
    return *this;
  }

  ~streamable_logger_t() { spdlog::log(log_level_, "{}", log_stream_.str()); }
};

#define LOG_S(log_level) streamable_logger_t(spdlog::level::log_level)

/** Log an unrecoverable error */
#define LOG_FATAL LOG_S(critical)

/** Log that something has definitely gone wrong */
#define LOG_ERROR LOG_S(err)

/** Warn about behaviour that is probably bad, but hasn't yet caused the system
 * to operate out of spec. */
#define LOG_WARNING LOG_S(warn)

/** Report a user-visible message about operation */
#define LOG_INFO LOG_S(info)

/** Report a message for developer debugging */
#define LOG_DEBUG LOG_S(debug)

/** Report very-verbose debugging information */
#define LOG_TRACE LOG_S(trace)

// Use like:
// curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, get_curlopt_verbose());
int64_t get_curlopt_verbose();

void logger_init(bool use_colors = false);

void logger_set_threshold(spdlog::level::level_enum threshold);

void logger_set_threshold(const LoggerConfig& lconfig);

void logger_set_enable(bool enabled);

int loggerGetSeverity();

#endif
