#define CATCH_CONFIG_MAIN
#include "catch.hpp"

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
