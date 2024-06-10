#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <functional>
#include <mutex>
#include <thread>

#include <queue.h>

class ThreadHandler {
public:
  enum class ThreadHandlerStates {
    /** meaning it's ready for op but there's no meaningful value stored */
    StateInitialized,

    /** ready for op, last returned value available */
    StateReady,

    /** blocked until op is finished */
    StateBlocked
  };

  ThreadHandler(std::function<int(int)> func) : mFunc(func), mState(ThreadHandlerStates::StateInitialized) {}

  /** returns last retrieved value or an invalid one if no op was executed */
  int value() const { return mValue; }

  /** get current state */
  ThreadHandlerStates state() const { return mState; }

  /** executes the defined operation assynchronously in the lower thread */
  void execute(int v = 0) {

    auto op = [this, v]() {
      std::lock_guard<std::mutex> lg(mtx);

      mState = ThreadHandlerStates::StateBlocked;
      mValue = mFunc(v);
      mState = ThreadHandlerStates::StateReady;
    };

    std::thread th(op);
    th.detach();

    // not guaranteed, by gives a chance for the lower thread to perform its duties before returning to caller
    std::this_thread::yield();
  }

private:
  ThreadHandlerStates mState;
  std::function<int(int)> mFunc;
  std::mutex mtx;
  int mValue;
};

SCENARIO( "multi-threaded usage", "[threaded]" ) {

  GIVEN( "a queue of integers with size 2" ) {
    Queue<int> q(2);

    REQUIRE( q.Size() == 2 );
    REQUIRE( q.Count() == 0 );

    WHEN( "one reader and one writer threads are available" ) {
      auto pusher = [&](int v) -> int { q.Push(v); return v; };
      auto poper  = [&](int v) -> int { return q.Pop(); };
      ThreadHandler writer(pusher);
      ThreadHandler reader(poper);
      REQUIRE( writer.state() == ThreadHandler::ThreadHandlerStates::StateInitialized );
      REQUIRE( reader.state() == ThreadHandler::ThreadHandlerStates::StateInitialized );

      THEN( "they should follow the proposed script blocking when needed" ) {
        auto Push = [&](int val){ writer.execute(val); };
        auto Pop = [&](){ reader.execute(); };

        Push(1);
        REQUIRE( writer.state() == ThreadHandler::ThreadHandlerStates::StateReady );

        Pop();
        REQUIRE( reader.value() == 1 );

        Push(2);
        Push(3);
        Push(4);

        Pop();
        REQUIRE( reader.value() == 3 );
        Pop();
        REQUIRE( reader.value() == 4);
        Pop();
        REQUIRE( reader.state() == ThreadHandler::ThreadHandlerStates::StateBlocked );

        Push(5);
        REQUIRE( reader.value() == 5 );
      }
    }
  }
}
