#include "treasure_hunt_parallel_packed.hpp"

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

int zone_pack_size(MPI_Comm comm) {
  int size_per_field = 0;
  MPI_Pack_size(1, MPI_UNSIGNED_LONG, comm, &size_per_field);
  return size_per_field * 4;
}

int result_pack_size(MPI_Comm comm) {
  int bool_size = 0;
  int ulong_size = 0;
  MPI_Pack_size(1, MPI_C_BOOL, comm, &bool_size);
  MPI_Pack_size(1, MPI_UNSIGNED_LONG, comm, &ulong_size);
  return bool_size + (ulong_size * 3);
}

void pack_zone(const Zone &zone, std::vector<char> &buffer, int &position,
               MPI_Comm comm) {
  const int capacity = static_cast<int>(buffer.size());
  MPI_Pack(&zone.x_begin, 1, MPI_UNSIGNED_LONG, buffer.data(), capacity,
           &position, comm);
  MPI_Pack(&zone.x_end, 1, MPI_UNSIGNED_LONG, buffer.data(), capacity,
           &position, comm);
  MPI_Pack(&zone.y_begin, 1, MPI_UNSIGNED_LONG, buffer.data(), capacity,
           &position, comm);
  MPI_Pack(&zone.y_end, 1, MPI_UNSIGNED_LONG, buffer.data(), capacity,
           &position, comm);
}

Zone unpack_zone(std::vector<char> &buffer, int &position, MPI_Comm comm) {
  const int capacity = static_cast<int>(buffer.size());
  Zone zone{};
  MPI_Unpack(buffer.data(), capacity, &position, &zone.x_begin, 1,
             MPI_UNSIGNED_LONG, comm);
  MPI_Unpack(buffer.data(), capacity, &position, &zone.x_end, 1,
             MPI_UNSIGNED_LONG, comm);
  MPI_Unpack(buffer.data(), capacity, &position, &zone.y_begin, 1,
             MPI_UNSIGNED_LONG, comm);
  MPI_Unpack(buffer.data(), capacity, &position, &zone.y_end, 1,
             MPI_UNSIGNED_LONG, comm);
  return zone;
}

void pack_result(const SearchResult &result, std::vector<char> &buffer,
                 int &position, MPI_Comm comm) {
  const int capacity = static_cast<int>(buffer.size());
  MPI_Pack(&result.found, 1, MPI_C_BOOL, buffer.data(), capacity, &position,
           comm);
  MPI_Pack(&result.x, 1, MPI_UNSIGNED_LONG, buffer.data(), capacity, &position,
           comm);
  MPI_Pack(&result.y, 1, MPI_UNSIGNED_LONG, buffer.data(), capacity, &position,
           comm);
  MPI_Pack(&result.cells_searched, 1, MPI_UNSIGNED_LONG, buffer.data(),
           capacity, &position, comm);
}

SearchResult unpack_result(std::vector<char> &buffer, int &position,
                           MPI_Comm comm) {
  const int capacity = static_cast<int>(buffer.size());
  SearchResult result{};
  MPI_Unpack(buffer.data(), capacity, &position, &result.found, 1, MPI_C_BOOL,
             comm);
  MPI_Unpack(buffer.data(), capacity, &position, &result.x, 1,
             MPI_UNSIGNED_LONG, comm);
  MPI_Unpack(buffer.data(), capacity, &position, &result.y, 1,
             MPI_UNSIGNED_LONG, comm);
  MPI_Unpack(buffer.data(), capacity, &position, &result.cells_searched, 1,
             MPI_UNSIGNED_LONG, comm);
  return result;
}

} // namespace

SearchResult treasure_hunt_parallel_packed(const Island &island,
                                           MPI_Comm comm) {
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

  const auto num_workers = static_cast<std::size_t>(size - 1);
  const int zone_buffer_size = zone_pack_size(comm);
  const int result_buffer_size = result_pack_size(comm);

  SearchResult local_result{
      .found = false, .x = 0, .y = 0, .cells_searched = 0};

  if (rank == 0) {
    const std::vector<Zone> zones =
        partition_into_zones(island.width, island.height, num_workers);
    for (std::size_t worker = 0; worker < num_workers; ++worker) {
      std::vector<char> buffer(static_cast<std::size_t>(zone_buffer_size));
      int position = 0;
      pack_zone(zones[worker], buffer, position, comm);
      MPI_Send(buffer.data(), position, MPI_PACKED,
               static_cast<int>(worker + 1), kZoneTag, comm);
    }

    for (std::size_t worker = 0; worker < num_workers; ++worker) {
      std::vector<char> buffer(static_cast<std::size_t>(result_buffer_size));
      MPI_Status status;
      MPI_Recv(buffer.data(), result_buffer_size, MPI_PACKED,
               static_cast<int>(worker + 1), kResultTag, comm, &status);
      int position = 0;
      const SearchResult worker_result = unpack_result(buffer, position, comm);
      local_result.cells_searched += worker_result.cells_searched;
      if (worker_result.found) {
        local_result.found = true;
        local_result.x = worker_result.x;
        local_result.y = worker_result.y;
      }
    }
  } else {
    std::vector<char> zone_buffer(static_cast<std::size_t>(zone_buffer_size));
    MPI_Status status;
    MPI_Recv(zone_buffer.data(), zone_buffer_size, MPI_PACKED, 0, kZoneTag,
             comm, &status);
    int position = 0;
    const Zone zone = unpack_zone(zone_buffer, position, comm);

    const SearchResult worker_result = search_zone(island, zone);
    std::vector<char> result_buffer(
        static_cast<std::size_t>(result_buffer_size));
    position = 0;
    pack_result(worker_result, result_buffer, position, comm);
    MPI_Send(result_buffer.data(), position, MPI_PACKED, 0, kResultTag, comm);
  }

  return local_result;
}

} // namespace lab9
