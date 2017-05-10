/***********************************************
*      filename: test_mempool.cpp
*      version: ---
*      
*      @author: wanmx@foxmail.com
*      @description: ---
*      @create: 2017-05-11 00:27:15
*      @last Modified: 2017-05-11 00:27:32
************************************************/
#include <gtest/gtest.h>
#include "memory/mempool.h"

namespace testing {
/**
 * @brief
**/
class test_mempool_suite : public ::testing::Test {
protected:
    test_mempool_suite() {};
    virtual ~test_mempool_suite() {};
    virtual void SetUp() {
    };
    virtual void TearDown() {
    };
};

/**
 * @brief
 * @begin_version
**/
TEST_F(test_mempool_suite, test_init)
{
    MultiMemoryPool mempool;
    mempool.register_item(sizeof(uint64_t));
    mempool.register_item(sizeof(uint32_t));
    int ret = mempool.init(100);
    ASSERT_EQ(ret, 0);
}

} //namespace testing
