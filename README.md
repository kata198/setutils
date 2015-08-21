# setutils
Commandline utilities for working with sets (difference, union, intersection) and shared library (C/C++)



Provides several utilities that are very fast for handling set operations, with an unlimited number of elements and sources.

Each command takes a minimum of two filename arguments, and performs the given set operation against them.

Each "Set" contains only unique elements and is not sorted. Pipe to the "sort" command (or use qsort) if you need sorting.



Files provided to the set\_\* commands designate an element as the contents of a line. For example, a file like:

	one
	two
	three

Is a set of three elements: [ "one", "two", "three" ]


set\_difference
---------------


Prints the members in the first set (Set A) which are not present in any other Sets.


**Y = A - B ( - C... )**


set\_intersection
-----------------


Prints the members in the first set (Set A) which are common in all other sets.


**Y = A & B ( & C... )**

set\_union
----------


Prints the union of all sets. This is a unique list of all members in any set.


**Y = A U B ( U C ...)**


Examples:
---------

	[timmy@silverslave setutils]$ cat set1

	one

	two

	three

	[timmy@silverslave setutils]$ cat set2

	two

	four

	cheese

	[timmy@silverslave setutils]$ echo -e "one\ntwo\nthree" > set1

	[timmy@silverslave setutils]$ echo -e "two\nfour\ncheese" > set2

	[timmy@silverslave setutils]$ set_union set1 set2

	three

	cheese

	two

	four

	one

	[timmy@silverslave setutils]$ set_intersection set1 set2

	two

	[timmy@silverslave setutils]$ set_intersection set2 set1

	two

	[timmy@silverslave setutils]$ set_difference set1 set2

	three

	one

	[timmy@silverslave setutils]$ set_difference set2 set1

	cheese

	four


Performance
-----------


Performance is AMAZING. The only operation that is "simple" to do with standard unix utils is the union, and the performance difference is extreme:

Here is a benchmark of creating a union with several sets of random integers, 1-500000:


*Show Dataset, 821271 integers across 4 files*


	[media@silverslave setutils]$ echo lst?.txt

	lst1.txt lst2.txt lst3.txt lst4.txt

	[media@silverslave setutils]$ cat lst?.txt | wc -l

	821271


**Show set_union simple union performance**


	[media@silverslave setutils]$ time (set_union lst?.txt >/dev/null )

	real    0m0.158s

	user    0m0.150s

	sys     0m0.007s

**Show set_union with additional sorting performance**


	[media@silverslave setutils]$ time (set_union lst?.txt | sort >/dev/null )

	real    0m0.175s

	user    0m0.170s

	sys     0m0.003s

**Show standard cat | sort | uniq performance**


	[media@silverslave setutils]$ time ( cat lst?.txt | sort | uniq >/dev/null )


	real    0m1.572s

	user    0m1.607s

	sys     0m0.007s



Library
-------

TODO: Document the associated library.


Installation
------------

Installation is done with standard "make; make install PREFIX=/usr" or to whatever prefix you want. Depends on glib-2.0.

Honours the environmental variable CFLAGS to specify flags.

The executables are all statically linked, and there is an additional shared library. The github comes with a "precompiled" folder which contains binaries I have built on my ArchLinux x86\_64 system. They may or may not work for you.
