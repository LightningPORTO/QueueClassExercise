#ifndef QUEUE_HPP_INCLUDED
#define QUEUE_HPP_INCLUDED

#include <queue>
#include <mutex>
#include <condition_variable>

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
        // Create an unique lock on the mutex. 
        // Automatically released at the end of the function
        std::unique_lock<std::mutex> lock(m_mutex);

        // Wait until lambda function returns true. 
        // Lambda function checks if queue is full
        m_writeCondition.wait(lock, [this]() { return m_queue.size() < m_maxQueueSize; });

        // Here the queue should have space
        // Add the new value to the queue
        m_queue.push(queueVal);

        // Notify other thread to proceed, unlocking it 
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
        // Create an unique lock on the mutex. 
        // Automatically released at the end of the function
        std::unique_lock<std::mutex> lock(m_mutex);

        // Wait until lambda function returns true. 
        // Lambda function checks if queue is NOT empty
        m_readCondition.wait(lock, [this]() { return !m_queue.empty(); });

        // Here the queue should not be empty so we can remove a value
        T queueVal = m_queue.front();
        m_queue.pop();

        // Notify other thread to proceed, unlocking it
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

#endif // QUEUE_HPP_INCLUDED