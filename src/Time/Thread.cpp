#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <functional>

class Thread
{
        // a member variable used to indicate thread "status"
    bool busy;
        // thread identifier
    int i;
        // a reference to the "other" thread in this example
    Thread *other;
        // the object for mutual exclusion of execution
    std::mutex *_mutex; 
        // for messaging between threads
    std::condition_variable *_condition_variable; 
        // function that the thread should run
    std::function<void()> threadFunction;
        // object to run the function on
    // ObjectType& obj;

    public:
        Thread(int i, Thread *other, std::mutex *_mutex, std::condition_variable *_condition_variable, std::function<void()> threadFunction)
        {
            this->i = i; // set the id of this thread
            if(i==0) { busy = true; }
            else { this->other = other; }
            this->_mutex = _mutex;
            this->_condition_variable = _condition_variable;
            this->threadFunction = threadFunction;
            // this->obj = obj;
        }

        bool isBusy()
        {
            std::lock_guard<std::mutex> lock(*_mutex);  // this locks the mutuex until the variable goes out of scope (i.e., when the function returns in this case)
            return busy;
        }

        void run()
        {
            if(i==0)
            {
                try
                {
                    threadFunction();
                    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                    { // anonymous inner block to manage scope of mutex lock 
                        std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                        std::cout << "Thread " << i << " is done sleeping" << std::endl;
                        _condition_variable->notify_all();
                        std::cout << "Thread " << i << " is notifying with busy=" << busy << std::endl;
                    }
                    std::cout << "Thread " << i << " is sleeping" << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                    { // anonymous inner block to manage scope of mutex lock 
                        std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                        _condition_variable->notify_all();
                        std::cout << "Thread " << i << " is notifying with busy=" << busy << std::endl;
                    }
                    std::cout << "Thread " << i << " is sleeping" << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                    { // anonymous inner block to manage scope of mutex lock 
                        std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                        busy = !busy;
                        _condition_variable->notify_all();
                        std::cout << "Thread " << i << " is notifying with busy=" << busy << std::endl;
                        std::cout << "Thread " << i << " is FINISHED!" << std::endl;
                    }
                }
                catch (...)
                {
                    std::cerr << "Thread " << i << " caught exception." << std::endl;
                }
            }
            else // id == 1
            {
                while(other->isBusy())
                {
                    std::cout << "Thread " << i << " is waiting!" << std::endl;
                    try 
                    {
                        std::unique_lock<std::mutex> lock(*_mutex);
                        _condition_variable->wait(lock);
                        threadFunction();
                        std::cout << "Thread " << i << " inside lock scope." << std::endl;
                    }
                    catch (...)
                    {
                        std::cerr << "Thread " << i << " caught exception." << std::endl;
                    }
                }
                std::cout << "Thread " << i << " is FINISHED!" << std::endl;
            }
        }

};

/**
 * Wrapper function because threads can't take pointers to member functions.
 */
// void run_wrapper(Thread *fe)
// {
//     fe->run();
// }


// int main()
// {
//     // Mutex to handle locking, condition variable to handle notifications between threads
    // std::mutex m;
    // std::condition_variable cv;

    // // Create thread objects
    // Thread t1(0, NULL, &m, &cv);
    // Thread t2(1, &t1, &m, &cv);

    // // Start threads
    // std::thread first(run_wrapper, &t1);
    // std::thread second(run_wrapper, &t2);

    // // Make sure both threads are complete before stopping main thread
    // first.join();
    // second.join();
//     return 0;
// }