#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>

/**
 * @brief - Queue class
 * Implements functions to push and pop values on a queue.
 * Multithread compatible, protects resource access.
 */
template<typename T>
class Queue {
public:
    /**
     * @brief - Construct a new Queue object
     * 
     * @param size - Maximum size of the queue
     */
    Queue(int size)
    {
        m_maxQueueSize = size;
    }

    /**
     * @brief - Pushes a new value to the queue.
     * If queue is full the function will block until free
     * 
     * @param queueVal - Value to be pushed
     */
    void Push(T queueVal)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_writeCondition.wait(lock, [this]() { return m_queue.size() < m_maxQueueSize; });

        m_queue.push(queueVal);

        m_readCondition.notify_one();
    }

    /**
     * @brief - Removes the first value from the queue. 
     * Blocks if queue is empty
     * 
     * @return T - Value removed from queue
     */
    T Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_readCondition.wait(lock, [this]() { return !m_queue.empty(); });

        T queueVal = m_queue.front();
        m_queue.pop();

        m_writeCondition.notify_one();
        return queueVal;
    }

    /**
     * @brief - Returns the current queue size
     * 
     * @return int - queue size
     */
    int Count()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    /**
     * @brief - Returns the maximum queue size
     * 
     * @return int - Max queue size value
     */
    int Size()
    {
        return m_maxQueueSize;
    }

private:
    int m_maxQueueSize;
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_readCondition;
    std::condition_variable m_writeCondition;

};

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
