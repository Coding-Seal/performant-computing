#pragma once

#include <string>

namespace lab6 {

bool strings_equal_parallel(const std::string &a, const std::string &b,
                            int num_threads);

} // namespace lab6
