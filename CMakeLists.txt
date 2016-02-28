function(cxx_test name testdir)
  aux_source_directory(${testdir} TEST_DIR_SRCS)
  add_executable(${name} ${TEST_DIR_SRCS})
  target_link_libraries(${name} gtest_main)
  target_link_libraries(${name} gtest)
  foreach (lib "${ARGN}")
    target_link_libraries(${name} ${lib})
  endforeach()
endfunction()




project(ad_monitor)
add_definitions("-DLINUX -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
include_directories(. $ENV{MGJ_REL}/include/ckit)
link_directories($ENV{MGJ_REL}/lib)


#build
aux_source_directory(./src/ SRC_DIR_SRCS)
add_executable(ad_monitor ${SRC_DIR_SRCS})
target_link_libraries(ad_monitor ckit)
target_link_libraries(ad_monitor rdkafka)
target_link_libraries(ad_monitor jsoncpp)
target_link_libraries(ad_monitor zookeeper_mt)

#install library
#install (TARGETS ad_monitor DESTINATION bin)
