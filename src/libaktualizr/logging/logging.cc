#include "logging.h"

#include "libaktualizr/config.h"

static spdlog::level::level_enum gLoggingThreshold;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

extern void logger_init_sink(bool use_colors = false);

int64_t get_curlopt_verbose() { return gLoggingThreshold <= spdlog::level::trace ? 1L : 0L; }

void logger_init(bool use_colors) {
  gLoggingThreshold = spdlog::level::trace;

  logger_init_sink(use_colors);

  spdlog::set_level(gLoggingThreshold);
}

void logger_set_threshold(const spdlog::level::level_enum threshold) {
  gLoggingThreshold = threshold;
  spdlog::set_level(gLoggingThreshold);
}

void logger_set_threshold(const LoggerConfig& lconfig) {
  int loglevel = lconfig.loglevel;
  if (loglevel < spdlog::level::trace) {
    LOG_WARNING << "Invalid log level: " << loglevel;
    loglevel = spdlog::level::trace;
  }
  if (spdlog::level::off < loglevel) {
    LOG_WARNING << "Invalid log level: " << loglevel;
    loglevel = spdlog::level::off;
  }
  logger_set_threshold(static_cast<spdlog::level::level_enum>(loglevel));
}

void logger_set_enable(bool enabled) {
  auto level = enabled ? gLoggingThreshold : spdlog::level::off;
  spdlog::set_level(level);
}

int loggerGetSeverity() { return static_cast<int>(gLoggingThreshold); }

// vim: set tabstop=2 shiftwidth=2 expandtab:
