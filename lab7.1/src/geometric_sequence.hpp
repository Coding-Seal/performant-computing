#pragma once

#include <vector>

namespace lab7_1 {

void compute_geometric_sequence_racy(std::vector<double> &a, double f,
                                     int num_threads);

void compute_geometric_sequence_parallel(std::vector<double> &a, double f,
                                         int num_threads);

} // namespace lab7_1
