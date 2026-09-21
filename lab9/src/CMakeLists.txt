find_package(MPI REQUIRED)

add_library(lab9 STATIC
  island.cpp
  zone.cpp
  search.cpp
  treasure_hunt_sequential.cpp
  treasure_hunt_parallel.cpp
  treasure_hunt_parallel_packed.cpp
)

target_include_directories(lab9 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lab9 PUBLIC MPI::MPI_CXX)

add_executable(treasure_hunt_demo
  main.cpp
)
target_link_libraries(treasure_hunt_demo PRIVATE lab9)

set(MPIEXEC_RUN_FLAGS ${MPIEXEC_PREFLAGS})
if(MPIEXEC_EXECUTABLE)
  execute_process(
    COMMAND ${MPIEXEC_EXECUTABLE} --version
    OUTPUT_VARIABLE MPIEXEC_VERSION_OUTPUT
    ERROR_VARIABLE MPIEXEC_VERSION_OUTPUT
  )
  if(MPIEXEC_VERSION_OUTPUT MATCHES "Open MPI")
    list(APPEND MPIEXEC_RUN_FLAGS --oversubscribe)
  endif()
endif()

foreach(np 2 4)
  add_test(
    NAME treasure_hunt_mpi_np${np}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${np}
            ${MPIEXEC_RUN_FLAGS} $<TARGET_FILE:treasure_hunt_demo>
            ${MPIEXEC_POSTFLAGS}
  )
endforeach()
