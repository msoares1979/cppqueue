#ifndef _QUEUE_H
#define _QUEUE_H 1

#include <chrono>
#include <exception>
#include <mutex>
#include <memory>
#include <condition_variable>

/** Custom exception for signaling timeout errors */
class QueueTimeoutException: public std::exception {
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
  Queue(std::size_t size);
  ~Queue() = default;

  void Push(T element);
  T Pop();
  T PopWithTimeout(int milliseconds);
  std::size_t Count() const;
  std::size_t Size() const;

private:
  void advanceIndex(std::size_t &index);

  std::mutex mMutex;
  std::condition_variable mCondition;
  std::size_t mSize;
  std::size_t mHead, mLength;
  std::unique_ptr<T[]> mItems;
};

/** ctor
 *
 * Using an unique_ptr for items to avoid issues between allocation and initialization
 */
template<class T>
Queue<T>::Queue(std::size_t size) :
    mSize(size),
    mHead(0), mLength(0),
    mItems(std::make_unique<T[]>(size))
{
}

/** Boilerplate to advance the given index without trespassing queue size
 *
 * @param index  reference to index to be increased
 */
template<class T>
void Queue<T>::advanceIndex(std::size_t &index)
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
* Very similar to Pop(), but using wait_for instead.
*
* @param ms  timeout for blocking the calling thread if the queue is empty
*
* @return the last @a element or an invalid value if queue is empty
*/
template<class T>
T Queue<T>::PopWithTimeout(int ms)
{
  std::unique_lock<std::mutex> lock(mMutex);
  auto condition_ok = mCondition.wait_for(lock, std::chrono::milliseconds(ms), [this] { return mLength > 0; });
  if (!condition_ok)
    throw QueueTimeoutException();

  auto e = mItems[mHead];
  if (mLength > 0)
    mLength--;

  advanceIndex(mHead);

  return e;
}

/** Gives the current number of queued items */
template<class T>
std::size_t Queue<T>::Count() const
{
  return mLength;
}

/** Limit capacity for the queued items */
template<class T>
std::size_t Queue<T>::Size() const
{
  return mSize;
}

#endif // _QUEUE_H
