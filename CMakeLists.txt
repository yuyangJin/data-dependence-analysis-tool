cmake_minimum_required(VERSION 3.16)
if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  message(FATAL_ERROR "Do not support non-Linux now!")
endif()

project(ddat CXX C)

set(CMAKE_CXX_FLAGS "-fpermissive -g")

#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
# add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/igraph)
# target_compile_options(igraph PRIVATE -fPIC)

# option(ENABLE_TEST "Enable test" ON)

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/third_party/dbg)


# link_libraries(igraph)
link_directories(${LLVM_LIBRARY_DIRS})

add_subdirectory(src)


# # explicitly list all source file
# add_library(
#   ddat
#   SHARED

#   src/data_dependence_analysis.cpp
#   # src/data_dependence_graph.cpp
#   # src/node.cpp
#   # src/utils.h
# )

# set_target_properties(
#   ddat

#   PROPERTIES
#   ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
# )

# target_compile_options(
#   ddat PRIVATE
#   -Wall -Wno-missing-braces
#   #-fno-rtti # We cannot open it because dyninst use `typeid` in header
#   -fno-omit-frame-pointer -fstack-protector-all
#   -rdynamic
# )

# # target_include_directories(baguatool PUBLIC ${PROJECT_SOURCE_DIR}/include)
# # target_include_directories(baguatool PRIVATE ${PROJECT_SOURCE_DIR}/src)

# if (ENABLE_TEST)
#   # List what we want to test
#   #add_subdirectory(${PROJECT_SOURCE_DIR}/test/igraph_test)
#   add_subdirectory(test)
# endif()