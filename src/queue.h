#ifndef _QUEUE_H
#define _QUEUE_H 1

/** A simple templated queue (LiFo) implementation with multi-thread support
 * and dynamic allocation
 */
template<class T>
class Queue {
public:
  Queue(int size);
  void Push(T element);
  T Pop();
  T PopWithTimeout(int milliseconds);
  int Count();
  int Size();
};

/** ctor, not much fun here */
template<class T>
Queue<T>::Queue(int)
{
}

/** Pushes such @a element into the queue, respecting its limits */
template<class T>
void Queue<T>::Push(T element)
{
}

/** Pops last element inserted at the queue
*
* @return the last @a element, blocks calling thread if empty
*/
template<class T>
T Queue<T>::Pop()
{
  return T();
}

/** Pops last element inserted at the queue
*
* @param milliseconds  timeout for blocking the calling thread if the queue is empty
*
* @return the last @a element or an invalid value if queue is empty
*/
template<class T>
T Queue<T>::PopWithTimeout(int milliseconds)
{
  return T();
}

/** Gives the current number of queued items */
template<class T>
int Queue<T>::Count()
{
  return 0;
}

/** Limit for the queued items */
template<class T>
int Queue<T>::Size()
{
  return 0;
}

#endif // _QUEUE_H
