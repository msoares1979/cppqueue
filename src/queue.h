#ifndef _QUEUE_H
#define _QUEUE_H 1

#include <exception>
#include <mutex>
#include <condition_variable>

/** Custom exception for signaling timeout errors */
class QueueTimeouException: public std::exception {
public:
  virtual const char* what() const noexcept override {
    return "timeout during operation";
  }
};

/** A simple templated queue (LiFo) implementation with multi-thread support
 * and dynamic allocation
 *
 * Allocation occurs once during initialization so we avoid memory
 * fragmentation. If the client requests a size too big, it's not really a
 * problem since the OS should take care of it via pagination.
 *
 * Concurrency is implemented via a simple condition variable. It should be
 * enough.
 *
 * Notice we don't care about handling stored resources. They are all
 * considered primitive types, so no need for it.
 */
template<class T>
class Queue {
public:
  Queue(int size);
  virtual ~Queue();
  void Push(T element);
  T Pop();
  T PopWithTimeout(int milliseconds);
  int Count() const;
  int Size() const;

private:
  void advanceIndex(int &index);

  std::mutex mMutex;
  std::condition_variable mCondition;
  int mSize;
  int mHead, mLength;
  T* mItems;
};

/** ctor, not much fun here
 *
 * although it's not a good idea mess with things that can fail inside the
 * constructor such as dynamic allocation, I want to avoid the penalty of
 * dealing with it during regular operations.
 */
template<class T>
Queue<T>::Queue(int size) :
    mSize(size),
    mHead(0), mLength(0),
    mItems(nullptr)
{
  mItems = new T[size];
}

/** dtor */
template<class T>
Queue<T>::~Queue()
{
  delete[] mItems;
}

/** Boilerplate to advance the given index without trespassing queue size
 *
 * @param index  reference to index to be increased
 */
template<class T>
void Queue<T>::advanceIndex(int &index)
{
  index = (index + 1) % mSize;
}

/** Pushes such @a element into the queue, respecting its limits
 *
 * Queues push elements at its end, let's use the array bounds to cycle it into
 * the list's beginning. Advances head when needed.
 */
template<class T>
void Queue<T>::Push(T element)
{
  std::lock_guard<std::mutex> guard(mMutex);

  if (0 == mSize) return;

  mItems[(mHead + mLength) % mSize] = element;
  if (mLength == mSize)
    advanceIndex(mHead);

  if (mLength < mSize)
    mLength++;

  // this gives a chance for any waiting thread to execute
  mCondition.notify_one();
}

/** Pops last element inserted at the queue
*
* Using a condition_variable predicate to determine whether we can fetch a
* value from the queue.
*
* @return the last @a element, blocks calling thread if empty
*/
template<class T>
T Queue<T>::Pop()
{
  std::unique_lock<std::mutex> lock(mMutex);
  mCondition.wait(lock, [this] { return mLength > 0; });

  auto e = mItems[mHead];
  if (mLength > 0)
    mLength--;

  advanceIndex(mHead);

  return e;
}

/** Pops last element inserted at the queue
*
* @param milliseconds  timeout for blocking the calling thread if the queue is empty
*
* @todo handle timeout
*
* @return the last @a element or an invalid value if queue is empty
*/
template<class T>
T Queue<T>::PopWithTimeout(int milliseconds)
{
  return Pop();
}

/** Gives the current number of queued items */
template<class T>
int Queue<T>::Count() const
{
  return mLength;
}

/** Limit capacity for the queued items */
template<class T>
int Queue<T>::Size() const
{
  return mSize;
}

#endif // _QUEUE_H
