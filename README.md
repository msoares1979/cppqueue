Simple Queue with Multi-Thread Access Support
---------------------------------------------

A small C++ project to experiment with multi-threading and cmake setup.

Ultimately I plan to make it a personal template for future projects.

Requirements
------------

Develop a class from scratch to queue a finite number of primitive
types (e.g., int). This class will be used for multi-threaded
communication as follows:

- Reading thread pops elements while Writing thread pushes elements.
- Two popping methods shall be available. If queue is empty, one of them
  shall block indefinitely, while the other one should throw an exception
  after a given timeout if no element is pushed meanwhile.
- If queue is full, pushing a new element shall drop the oldest element
  in-queue before storing a new one.
- To support different types, a template class shall be implemented.
- Dynamic memory allocation shall be used to store queue elements (no
  std library for storing elements). Race conditions shall be avoided.
- Code shall be document. Preferably using Doxygen style.
- Performance issues are optionally addressed.
- The class interface should look like this:

      class Queue<T> {
        Queue(int size) {...}
        void Push(T element) {...}
        T Pop() {...}
        T PopWithTimeout(int milliseconds) {...}
        int Count() {...} // Amount of elements stored now
        int Size() {...} // Max number of elements
      }


As an example implement the following in main():

| Writing Thread step | Queue (after step) | Reading Thread step |
|---------------------|--------------------|---------------------|
| New Queue<int>(2)   |                    |                     |
| Push(1)             |                  1 |                     |
|                     |                    | Pop() -> 1          |
| Push(2)             |                  2 |                     |
| Push(3)             |                2,3 |                     |
| Push(4) # 2 dropped |                3,4 |                     |
|                     |                  4 | Pop() -> 3          |
|                     |                    | Pop() -> 4          |
|                     |                    | Pop() // blocks     |
| Push(5)             |                  5 |                     |
|                     |                    | -> 5 // released    |


Develop unit tests for the Queue class with support of a framework (e.g.
cppunit, gtest, catch).

Develop a CMake configuration file to ease the build process (inc. tests).


Build Dependencies
------------------

- C++ compiler (gcc, clang)
- cmake, doxygen, lcov, genhtml
- Makefile, ninja (optional)


 Build and Usage Instructions
------------------

    cmake -S . -B output
    cmake --build output
    make -C output check
    make -C output doc
    make -C output coverage

General Remarks
---------------

* As someone fond of tinkering with build-systems, it's hard to know when it's
  enough. Good aspect of it is that one can have a solid template for future
  projects.

* The methodology or way-of-thought should be tracked via the commit messages. In
  order to honor that, I've followed the template below to make objectives and
  reasoning more explicit.

  > short one-line telling WHAT was done
  > 
  > Longer text explaining WHY it's been done this way.
  > Can include code snippets or usage examples to ilustrate it.

* This small project comes as a nice opportunity to check my recently adopted
  development setup for Neovim. Still need to figure out proper LSP support,
  improve usage of quickfix lists, DAP, nvim-completion and so on.
