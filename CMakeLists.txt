function(cxx_test name testdir)
  aux_source_directory(${testdir} TEST_DIR_SRCS)
  add_executable(${name} ${TEST_DIR_SRCS})
  target_link_libraries(${name} gtest_main)
  target_link_libraries(${name} gtest)
  foreach (lib "${ARGN}")
    target_link_libraries(${name} ${lib})
  endforeach()
endfunction()
 



project(mogu)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
add_definitions("-DLINUX -g")
include_directories(. $ENV{MGJ_REL}/include/ckit $ENV{MGJ_REL}/include/flowctl)
link_directories($ENV{MGJ_REL}/lib)
link_directories(/usr/local/aliws/lib)
SET(ADMERGE_MAIN_VERSION 1)
SET(ADMERGE_MINOR_VERSION 0)
exec_program(date ARGS +%Y%m%d OUTPUT_VARIABLE ADMERGE_REVISION_VERSION)

#build
aux_source_directory(./mogu KAFKA_BUILD_SRCS)
SET_TARGET_PROPERTIES(mogu PROPERTIES VERSION ${ADMERGE_MAIN_VERSION}.${ADMERGE_MINOR_VERSION}.${ADMERGE_REVISION_VERSION} SOVERSION ${ADMERGE_MAIN_VERSION})
target_link_libraries(ad_merger ckit_foundation)
target_link_libraries(ad_merger sqlite3)
target_link_libraries(ad_merger ckit_zk)
target_link_libraries(ad_merger flow_ctl)
target_link_libraries(ad_merger jsoncpp)
target_link_libraries(ad_merger ckit_kafka)


#build
aux_source_directory(./src INDEX_BUILD_SRCS)
add_executable(ad_merger_srv ${INDEX_BUILD_SRCS})
SET_TARGET_PROPERTIES(ad_merger_srv PROPERTIES VERSION ${ADMERGE_MAIN_VERSION}.${ADMERGE_MINOR_VERSION}.${ADMERGE_REVISION_VERSION} SOVERSION ${ADMERGE_MAIN_VERSION})
target_link_libraries(ad_merger_srv ad_merger)


cxx_test(ut_test ./test/ ad_merger)
