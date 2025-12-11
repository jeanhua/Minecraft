//
// Created by jeanhua on 2025/8/19.
//

#ifndef MINECRAFT_TASK_H
#define MINECRAFT_TASK_H
#include <memory>
#include <mutex>
#include <vector>
#include <functional>
#include <thread>
#include <list>
#include <condition_variable>

template<typename retType,typename inType>
class Task
{
public:
    explicit Task(inType in);
    virtual retType doWork()=0;
    virtual ~Task()=default;
protected:
    inType m_in;
};


template<typename retType,typename inType>
class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();
    ThreadPool(const ThreadPool& t) = delete;
    ThreadPool& operator = (const ThreadPool& t) = delete;
public:
    void init(uint32_t maxThreadNumber = 5);
    void stop();
    void addTask(std::shared_ptr<Task<retType,inType>> task);
    void addTask(std::vector<std::shared_ptr<Task<retType,inType>>>&& task);
    void clear();

    bool isReady() const;

    std::vector<retType>&& getOutput();

private:
    void threadFunction();
private:
    std::vector<std::shared_ptr<std::thread>> m_threadVector;
    std::list<std::shared_ptr<Task<retType,inType>>> m_taskList;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_Running;

    std::atomic<bool> is_ready = {true};

    std::mutex m_outMutex;
    std::vector<retType> m_output;

};

template<typename retType, typename inType>
Task<retType, inType>::Task(inType in):m_in(in) {}

template<typename retType,typename inType>
ThreadPool<retType,inType>::ThreadPool() : m_Running(false) {
}

template<typename retType,typename inType>
ThreadPool<retType,inType>::~ThreadPool() {
    clear();
}

template<typename retType,typename inType>
void ThreadPool<retType,inType>::init(uint32_t maxThreadNumber) {
    if (maxThreadNumber <= 0)
        maxThreadNumber = 2;
    else if (maxThreadNumber > std::thread::hardware_concurrency())
        maxThreadNumber = std::thread::hardware_concurrency();

    m_Running = true;
    for (int i = 0; i < maxThreadNumber; i++) {
        std::shared_ptr<std::thread> spThread;
        spThread = std::make_shared<std::thread>(std::bind(&ThreadPool::threadFunction, this));
        m_threadVector.push_back(spThread);
    }
}

template<typename retType,typename inType>
void ThreadPool<retType,inType>::threadFunction() {
    while (true) {
        std::unique_lock lock(m_mutex);
        while (m_taskList.empty()) {
            if (!m_Running)
                break;
            is_ready.store(true);
            m_cv.wait(lock);
        }
        if (!m_Running)
            break;
        std::shared_ptr<Task<retType,inType>> spTask = m_taskList.front();
        m_taskList.pop_front();
        if (spTask) {
            retType ret = spTask->doWork();
            spTask.reset();
            m_outMutex.lock();
            m_output.push_back(ret);
            m_outMutex.unlock();
        }
    }
}

template<typename retType,typename inType>
void ThreadPool<retType,inType>::stop() {
    m_Running = false;
    m_cv.notify_all();
    for (const auto &iter: m_threadVector) {
        if (iter->joinable())
            iter->join();
    }
}

template<typename retType,typename inType>
void ThreadPool<retType,inType>::addTask(std::shared_ptr<Task<retType,inType>> task) {
    std::unique_lock lock(m_mutex);
    is_ready.store(false);
    m_taskList.push_back(task);
    m_cv.notify_one();
}

template<typename retType, typename inType>
void ThreadPool<retType, inType>::addTask(std::vector<std::shared_ptr<Task<retType, inType> > > &&task) {
    int size = task.size();
    std::unique_lock lock(m_mutex);
    is_ready.store(false);
    m_taskList.insert(m_taskList.end(), task.begin(), task.end());
    for (int i = 0; i < size; i++) m_cv.notify_one();
}


template<typename retType,typename inType>
void ThreadPool<retType,inType>::clear() {
    std::unique_lock lock(m_mutex);
    for (auto &iter: m_taskList)
        iter.reset();
    m_taskList.clear();
}

template<typename retType,typename inType>
std::vector<retType>&& ThreadPool<retType,inType>::getOutput() {
    std::unique_lock lock(m_outMutex);
    return std::move(m_output);
}

template<typename retType, typename inType>
bool ThreadPool<retType, inType>::isReady() const {
    return is_ready.load();
}


#endif //MINECRAFT_TASK_H