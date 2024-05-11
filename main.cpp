#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>


template<typename T>
class Queue {
public:
    Queue(int size)
    {
        m_maxQueueSize = size;
    }

    void Push(T queueVal)
    {
        m_writeMutex.lock();
        
        while (m_queue.size() + 1 >= m_maxQueueSize)
        {
            std::cout << "Queue is full" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        m_queue.push(queueVal);
        
        m_writeMutex.unlock();
    }

    T Pop()
    {
        m_readMutex.lock();

        while (m_queue.empty())
        {
            std::cout << "Queue is empty" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        T queueVal = m_queue.front();
        m_queue.pop();

        m_readMutex.unlock();
        return queueVal;
    }

    int Count()
    {
        return m_queue.size();
    }

    int Size()
    {
        return m_maxQueueSize;
    }

private:
    int m_maxQueueSize;
    std::queue<T> m_queue;
    std::mutex m_writeMutex;
    std::mutex m_readMutex;

};

template<typename T>
class ThreadFuncs{
public:
    ThreadFuncs(int maxValue)
    {
        m_maxValue = maxValue;
    }

    void writeValues(Queue<T>& queue, uint8_t ItDelaySec)
    {
        for (int i = 1; i < m_maxValue + 1; ++i) 
        {
            queue.Push(i);
            std::cout << "Pushing " << i << " to queue" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(ItDelaySec));
        }
    }

    void readValues(Queue<T>& queue, uint8_t ItDelaySec)
    {
        for (int i = 1; i < m_maxValue + 1; ++i) 
        {
            T value = queue.Pop();
            std::cout << "Removed " << value << " from queue" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(ItDelaySec));
        }
    }

private:
    int m_maxValue;
};



// Example usage
int main() {
    Queue<int> qObj(2);
    
    ThreadFuncs<int> threadFObj(5);

    std::thread writeThread(&ThreadFuncs<int>::writeValues, &threadFObj, std::ref(qObj),1);
    std::thread readThread(&ThreadFuncs<int>::readValues, &threadFObj, std::ref(qObj),5);

    writeThread.join();
    readThread.join();

    std::cin.get(); // Wait for user input before exiting

    return 0;
}
