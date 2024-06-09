#define CATCH_CONFIG_MAIN
#include "catch.hpp"

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
  }
}
