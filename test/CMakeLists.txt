cmake_minimum_required (VERSION 2.6)

INCLUDE_DIRECTORIES(../build/include)

FIND_LIBRARY(LIB_GTEST gtest ../build/lib NO_DEFAULT_PATH)
FIND_LIBRARY(LIB_MEMORY memory ../build/lib NO_DEFAULT_PATH)
#no runtime lib needed, ignore it
#FIND_LIBRARY(RUNTIME_LIB rt /usr/lib  /usr/local/lib NO_DEFAULT_PATH)

link_libraries(${LIB_GTEST} ${LIB_MEMORY})

ADD_DEFINITIONS(
-O0 -g -W -Wall
-Wunused-variable -Wunused-parameter -Wunused-function -Wunused
-Wno-deprecated -Woverloaded-virtual -Wwrite-strings
-D__WUR= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DTIXML_USE_STL
)

SET(SRC main.cpp test_mempool.cpp)

ADD_EXECUTABLE(test ${SRC})
