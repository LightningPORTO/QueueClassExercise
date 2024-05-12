#include <gtest/gtest.h>
#include <thread>

#include "problem_queue.hpp"

// Test case for Push and Pop functions of Queue
TEST(QueueTest, PushPopTest) {
    Queue<int> qObj(2);
    qObj.Push(1);
    EXPECT_EQ(qObj.Pop(), 1);
}

// Test case for Count function of Queue
TEST(QueueTest, CountTest) {
    Queue<int> qObj(2);
    qObj.Push(1);
    qObj.Push(2);
    EXPECT_EQ(qObj.Count(), 2);
}

// Test case for Size function of Queue
TEST(QueueTest, SizeTest) {
    Queue<int> qObj(2);
    EXPECT_EQ(qObj.Size(), 2);
}

// Test case for multithreaded operations
TEST(QueueTest, MultithreadingTest) {
    Queue<int> qObj(2);
    std::thread writeThread([&qObj]() {
        qObj.Push(1);
        qObj.Push(2);
    });
    std::thread readThread([&qObj]() {
        qObj.Pop();
        qObj.Pop();
    });
    writeThread.join();
    readThread.join();
    EXPECT_EQ(qObj.Count(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
