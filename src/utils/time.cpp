#include <chrono>

using time_point = std::chrono::_V2::steady_clock::time_point;

time_point time ( void ) {
  return std::chrono::steady_clock::now();
}

double elapsed(time_point t_start, time_point t_fin) {
  return std::chrono::duration_cast<std::chrono::duration<double>>(t_fin - t_start).count();
}