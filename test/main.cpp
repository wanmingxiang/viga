/***********************************************
*      filename: main.cpp
*      version: ---
*      
*      @author: wanmx@foxmail.com
*      @description: ---
*      @create: 2017-05-10 22:59:07
*      @last Modified: 2017-05-10 23:18:08
************************************************/

#include <gtest/gtest.h>

int main(int argc, char **argv) 
{ 
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); //顺序执行下述所有以TEST/TEST_F/TEST_P开头的UT用例 
}

