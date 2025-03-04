/*
    * 线程池类定义
    * 作者：何秋蓉
    * 时间：2025年3月
*/
#include <thread>
#include <queue>               // 用于任务队列
#include <functional>          // 用于 std::function
#include <mutex>               // 用于互斥锁
#include <condition_variable>  // 用于条件变量

// ThreadPool 类定义
class ThreadPool {
public:
    // 任务结构体定义
    struct Task {
        std::function<void()> func;  // 存储任务的函数
        int priority;                // 任务的优先级（数值越小优先级越高）

        // 重载小于运算符用于优先级队列，数值越小优先级越高
        bool operator<(const Task& other) const {
            return priority > other.priority; // 小顶堆：优先级低的任务排在队列前面
        }
    };

    // 构造函数：初始化线程池
    explicit ThreadPool(size_t threads = std::thread::hardware_concurrency())
        : stop(false) {  // 默认不停止线程池
        // 创建线程池
        for(size_t i = 0; i < threads; ++i) {
            // 每个线程执行一个任务循环
            workers.emplace_back([this] {
                // 无限循环，直到接收到停止信号
                for(;;) {
                    Task task;  // 定义任务
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);  // 锁定任务队列
                        
                        // 等待任务队列有任务或者线程池停止
                        this->condition.wait(lock, [this]{ 
                            return this->stop || !this->tasks.empty(); // 等待停止或者任务队列非空
                        });
                        
                        // 如果线程池停止且任务队列为空，退出线程
                        if(this->stop && this->tasks.empty()) return;
                        
                        // 从优先级队列中获取一个任务
                        task = std::move(this->tasks.top());
                        this->tasks.pop();  // 弹出任务
                    }
                    task.func();  // 执行任务
                }
            });
        }
    }

    // 模板函数：向线程池中添加任务
    template<class F>
    void enqueue(int priority, F&& f) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);  // 锁定队列
            // 将任务和优先级封装成 Task 放入优先级队列中
            tasks.push({std::forward<F>(f), priority});
        }
        condition.notify_one();  // 通知一个线程可以开始执行任务
    }

    // 析构函数：停止线程池并清理线程
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);  // 锁定队列
            stop = true;  // 设置停止标志
        }
        condition.notify_all();  // 唤醒所有线程
        for(auto& worker : workers) {
            worker.join();  // 等待所有线程执行完毕
        }
    }

private:
    std::vector<std::thread> workers;  //存储工作线程
    std::priority_queue<Task> tasks;   //优先级任务队列（小顶堆）
    std::mutex queue_mutex;            //用于同步对队列的访问
    std::condition_variable condition; //用于线程间的同步，通知线程执行任务
    bool stop;                         //用于判断线程池是否需要停止
};
