cmake_minimum_required (VERSION 2.6)

ADD_DEFINITIONS(
-O0 -g -W -Wall
-Wunused-variable -Wunused-parameter -Wunused-function -Wunused
-Wno-deprecated -Woverloaded-virtual -Wwrite-strings
-D__WUR= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DTIXML_USE_STL
)


ADD_EXECUTABLE(quick_sort quick_sort.cpp)
