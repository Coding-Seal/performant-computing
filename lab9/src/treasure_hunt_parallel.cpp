#include "treasure_hunt_parallel.hpp"

#include <cstddef>
#include <mpi.h>
#include <vector>

#include "island.hpp"
#include "search.hpp"
#include "zone.hpp"

namespace lab9 {

namespace {

constexpr int kZoneTag = 0;
constexpr int kResultTag = 1;

MPI_Datatype make_zone_datatype() {
  constexpr int count = 4;
  int block_lengths[count] = {1, 1, 1, 1};
  MPI_Aint displacements[count] = {
      offsetof(Zone, x_begin), offsetof(Zone, x_end), offsetof(Zone, y_begin),
      offsetof(Zone, y_end)};
  MPI_Datatype const types[count] = {MPI_UNSIGNED_LONG, MPI_UNSIGNED_LONG,
                                     MPI_UNSIGNED_LONG, MPI_UNSIGNED_LONG};
  MPI_Datatype zone_type;
  MPI_Type_create_struct(count, block_lengths, displacements, types,
                         &zone_type);
  MPI_Type_commit(&zone_type);
  return zone_type;
}

MPI_Datatype make_search_result_datatype() {
  constexpr int count = 4;
  int block_lengths[count] = {1, 1, 1, 1};
  MPI_Aint displacements[count] = {
      offsetof(SearchResult, found), offsetof(SearchResult, x),
      offsetof(SearchResult, y), offsetof(SearchResult, cells_searched)};
  MPI_Datatype const types[count] = {MPI_C_BOOL, MPI_UNSIGNED_LONG,
                                     MPI_UNSIGNED_LONG, MPI_UNSIGNED_LONG};
  MPI_Datatype result_type;
  MPI_Type_create_struct(count, block_lengths, displacements, types,
                         &result_type);
  MPI_Type_commit(&result_type);
  return result_type;
}

} // namespace

SearchResult treasure_hunt_parallel(const Island &island, MPI_Comm comm) {
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  if (size < 2) {
    return search_zone(island, Zone{.x_begin = 0,
                                    .x_end = island.width,
                                    .y_begin = 0,
                                    .y_end = island.height});
  }

  MPI_Datatype zone_type = make_zone_datatype();
  MPI_Datatype result_type = make_search_result_datatype();
  const auto num_workers = static_cast<std::size_t>(size - 1);

  SearchResult local_result{
      .found = false, .x = 0, .y = 0, .cells_searched = 0};

  if (rank == 0) {
    const std::vector<Zone> zones =
        partition_into_zones(island.width, island.height, num_workers);
    for (std::size_t worker = 0; worker < num_workers; ++worker) {
      MPI_Send(&zones[worker], 1, zone_type, static_cast<int>(worker + 1),
               kZoneTag, comm);
    }

    for (std::size_t worker = 0; worker < num_workers; ++worker) {
      SearchResult worker_result{};
      MPI_Status status;
      MPI_Recv(&worker_result, 1, result_type, static_cast<int>(worker + 1),
               kResultTag, comm, &status);
      local_result.cells_searched += worker_result.cells_searched;
      if (worker_result.found) {
        local_result.found = true;
        local_result.x = worker_result.x;
        local_result.y = worker_result.y;
      }
    }
  } else {
    Zone zone{};
    MPI_Status status;
    MPI_Recv(&zone, 1, zone_type, 0, kZoneTag, comm, &status);
    const SearchResult worker_result = search_zone(island, zone);
    MPI_Send(&worker_result, 1, result_type, 0, kResultTag, comm);
  }

  MPI_Type_free(&zone_type);
  MPI_Type_free(&result_type);
  return local_result;
}

} // namespace lab9
