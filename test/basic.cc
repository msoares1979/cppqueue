#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <chrono>
#include <string>

#include <queue.h>

SCENARIO( "basic queue manipulation", "[basic]" ) {

  GIVEN( "a queue of integers with limited size" ) {
    const int size = 2;
    Queue<int> q(size);

    REQUIRE( q.Size() == size );
    REQUIRE( q.Count() == 0 );

    WHEN( "an element is pushed" ) {
      q.Push(1);

      THEN( "only the count is modified" ) {
        REQUIRE( q.Size() == size );
        REQUIRE( q.Count() == 1 );
      }
    }

    WHEN( "many elements are pushed" ) {
      for(int i = 0; i < (2 * size); i++)
        q.Push(i);

      THEN( "count is never bigger than size" ) {
        REQUIRE( q.Size() == size );
        REQUIRE( q.Count() == size );
      }
    }

    WHEN( "a known value is pushed" ) {
      const int known = 3;
      q.Push(known);

      THEN( "the same value is poped" ) {
        REQUIRE( q.Pop() == known );
      }

      WHEN( "another value is pushed" ) {
        const int another = 2;
        q.Push(another);

        THEN( "the previous value is poped" ) {
          REQUIRE( q.Pop() == known );
        }
      }
    }

    WHEN( "a queue is full" ) {
      for(int i = 0; i < size; i++)
        q.Push(i);

      REQUIRE( q.Count() == size );

      THEN( "poping one item decreases the queue by one" ) {
        auto item = q.Pop();
        REQUIRE( q.Count() == (size - 1) );
      }

      THEN( "poping all items resets the queue" ) {
        for(int i = 0; i < size; i++)
          auto item = q.Pop();

        REQUIRE( q.Count() == 0 );
      }
    }
  }

  GIVEN( "a queue of integers with size 2" ) {
    Queue<int> q(2);

    WHEN( "when a sequence of 5 consecutive numbers are pushed into it" ) {
      for(int i = 1; i <= 5; i++)
        q.Push(i);

      THEN( "the next poped numbers are 4 and 5" ) {
        REQUIRE( q.Pop() == 4 );
        REQUIRE( q.Pop() == 5 );
      }
    }

    WHEN( "no numbers are pushed" ) {
      THEN( "PopWithTimeout will thrown an exception after a while" ) {
        int awhile = 100; // ms

        auto before = std::chrono::high_resolution_clock::now();
        REQUIRE_THROWS_AS(q.PopWithTimeout(awhile), QueueTimeoutException);
        auto after = std::chrono::high_resolution_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
        // consider a small delta since this is not a real-time hard requirement
        REQUIRE((elapsed.count() - awhile) < 10);
      }
    }

    WHEN( "any element is available" ) {
      const int any = 1;
      q.Push(any);

      THEN( "PopWithTimeout returns immediatelly" ) {
        REQUIRE( q.PopWithTimeout(10) == any );
      }
    }
  }

  GIVEN( "a queue of size zero" ) {
    Queue<int> q(0);
    REQUIRE( q.Size() == 0 );

    WHEN( "an element is pushed" ) {
      q.Push(1);

      THEN( "the queue is not modified" ) {
        REQUIRE( q.Count() == 0 );
      }
    }
  }

  // this is for testing multi datatype support
  GIVEN( "a queue of strings with limited size" ) {
    Queue<std::string> q(2);

    WHEN( "a known string is pushed") {
      const std::string fb = "foobar";
      q.Push(fb);

      THEN( "the same string is poped" ) {
        REQUIRE( q.Pop() == fb );
      }
    }
  }
}
