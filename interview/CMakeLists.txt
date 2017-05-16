cmake_minimum_required (VERSION 2.6)

INCLUDE_DIRECTORIES(../build/include)
FIND_LIBRARY(LIB_GLOG glog ../build/lib NO_DEFAULT_PATH)

link_libraries(${LIB_GLOG})

ADD_DEFINITIONS(
-O0 -g -W -Wall
-Wunused-variable -Wunused-parameter -Wunused-function -Wunused
-Wno-deprecated -Woverloaded-virtual -Wwrite-strings
-D__WUR= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DTIXML_USE_STL
)


ADD_EXECUTABLE(quick_sort quick_sort.cpp)
ADD_EXECUTABLE(min_heap heap.cpp)
ADD_EXECUTABLE(list_reverse list_reverse.cpp)
