#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

void logger_init_sink(bool use_colors = false) { spdlog::set_default_logger(spdlog::stdout_color_mt("aktualizr")); }
