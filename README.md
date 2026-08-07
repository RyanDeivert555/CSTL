# CSTL

A library with generic implementations of common data structures

## Why use this library when you can just use C++'s STL?
Ultimately, that is up to you. If you want to use C and not C++, and need generic data structures, use this.
This library contains benchmarks comparing itself to data structures in the STL, and the performance should be comparable.

## Why isn't [insert data structure] here?
I just didn't get around to adding it. PRs welcome.

## Can you make this compatible with C89 for maximum portability?
No, I believe that C89 is too restrictive for the little portability that it offers.

## Why did you use [insert strategy a] to do generics in C? Why didn't you use [insert strategy b]?
I deemed this strategy the best. No strategy will be perfect, so you will find holes with any strategy.
Type erased structures used to be added, but they were removed because the cost of maintenance was not worth it, as the implementations needed to be in sync with the generic versions.

## Will you add support for [insert build system]?
Maybe. I might at least add support for makefiles. As always, PRs welcome.

## Does this library have any dependencies?
The only dependency is C standard library, using C11.
