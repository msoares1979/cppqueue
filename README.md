Description
-----------

C++ exercise

Usage Instructions
------------------

    cmake -B output
    cmake --build output
    make -C output test

Build Dependencies
------------------

- C++ compiler (gcc, clang)
- cmake, doxygen
- Makefile, ninja (optional)

General Remarks
---------------

As someone fond of tinkering with build-systems, it's hard to know when it's
enough. Good aspect of it is that one can have a solid template for future
projects.

The methodology or way-of-thought should be tracked via the commit messages. In
order to honor that, I've followed the template below to make objectives and
reasoning more explicit.

> short one-line telling WHAT was done
> 
> Longer text explaining WHY it's been done this way.
> Can include code snippets or usage examples to ilustrate it.

This small project comes as a nice opportunity to test my recently adopted
development setup for Neovim. Still need to figure out LSP support, improve
usage of quickfix lists, nvim-completion and so on.
