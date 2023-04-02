#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <deque>
#include <mutex>
#include <condition_variable>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
public:
  SafeQueue(void)
    : q()
    , m()
    , c()
  {}

  ~SafeQueue(void)
  {}

  // Add an element to the queue.
  void push(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    c.notify_one();
  }

  // Get the "front"-element.
  // If the queue is empty, wait till a element is avaiable.
  T get_front(void)
  {
    std::unique_lock<std::mutex> lock(m);
    while(q.empty())
    {
      // release lock as long as the wait and reaquire it afterwards.
      c.wait(lock);
    }
    T val = q.front();
    q.pop();
    return val;
  }

  T at(int i)
  {
    std::lock_guard<std::mutex> lock(m);
    T temp = q.front() + i;
    c.notify_one();
    return temp;
  }

  void erase(int i)
  {
    std::lock_guard<std::mutex> lock(m);
    q.erase(q.front() + i);
    c.notify_one();
  }

  void size()
  {
      return q.size();
  }

private:
  std::deque<T> q; //dequeue is used to be able to erase elements
  mutable std::mutex m;
  std::condition_variable c;
};
#endif
