
#include <iostream>
#include <chrono>
#include <thread>

#include "problem_queue.hpp"
/**
 * @brief - ThreadFuncs class
 * Helper class to test the queue class.
 */
template<typename T>
class ThreadFuncs{
public:
    /**
     * @brief Construct a new Thread Funcs object
     * 
     * @param maxValue - Maximum value to be added to the queue
     */
    ThreadFuncs(int maxValue)
    {
        m_maxValue = maxValue;
    }

    /**
     * @brief - Pushes a new value to the queue incrementally, with a delay between cycles.
     * 
     * @param queue - Class object
     * @param ItDelayMiliSec - Iteration delay in milliseconds.
     */
    void writeValues(Queue<T>& queue, uint8_t ItDelayMiliSec)
    {
        for (int i = 1; i < m_maxValue + 1; ++i) 
        {
            queue.Push(i);
            std::cout << "Pushing " << i << " to queue" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(ItDelayMiliSec));
        }
    }

    /**
     * @brief - Removes a new value from the queue, with a delay between cycles.
     * 
     * @param queue - Class object
     * @param ItDelayMiliSec - Iteration delay in milliseconds.
     */
    void readValues(Queue<T>& queue, uint8_t ItDelayMiliSec)
    {
        for (int i = 1; i < m_maxValue + 1; ++i) 
        {
            T value = queue.Pop();
            std::cout << "Removed " << value << " from queue" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(ItDelayMiliSec));
        }
    }

private:
    int m_maxValue;
};

/**
 * @brief - Main function of the project.
 * Initiates the "problem's" class (Queue) and a helper class (ThreadFuncs) for the testing of the "problem's" class
 */
int main() {
    Queue<int> qObj(2);
    
    ThreadFuncs<int> threadFObj(5);

    std::thread writeThread(&ThreadFuncs<int>::writeValues, &threadFObj, std::ref(qObj), 1000);
    std::thread readThread(&ThreadFuncs<int>::readValues, &threadFObj, std::ref(qObj), 1500);

    writeThread.join();
    readThread.join();

    return 0;
}
