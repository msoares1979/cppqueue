#ifndef _QUEUE_H
#define _QUEUE_H 1

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

template<class T>
Queue<T>::Queue(int)
{
}

template<class T>
void Queue<T>::Push(T element)
{
}

template<class T>
T Queue<T>::Pop()
{
  return T();
}

template<class T>
T Queue<T>::PopWithTimeout(int milliseconds)
{
  return T();
}

template<class T>
int Queue<T>::Count()
{
  return 0;
}

template<class T>
int Queue<T>::Size()
{
  return 0;
}

#endif // _QUEUE_H
