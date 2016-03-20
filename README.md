About Caddeus
=============

Caddeus is a prepared GNUmakefile oriented to the strict quality of C programs.

It features an optimal test runner, strict C flags by default, automatic
Cppcheck, Clang static analyzer and Valgrind usage if installed on the system.

Check out `doc/how-to-use.txt` file for a tutorial.

Rationale
=========

I found out that I'm not a good enough C programmer:

- Cppcheck and the Clang static analyzer constantly reveals mistakes and
  omissions in my code.

- I found out that running Valgrind on my legacy code revealed many
  memory leaks.

- Furthermore, I could only run Valgrind on specific code flows. I needed
  to find a way to systematically test multiple code flows.

- This meant that I had to redisign part of my code just to make it
  systematically analyzable.

So I attempted the testing paradigm under C. This helped because I could run
Valgrind on my tests to systematically check different code flows, but:

- I had to redesign most of my C code.

- At first I had to manually run the tests each time I changed a file, so I
  automated it. Then I found out that I was running the full Valgrindly
  slow test suite each time even after a change on a single file.

- I started defining dependencies on tests to my GNUmakefile to prevent
  testing redundancy but this was quite time consuming even for small projects.

So I needed a tool automate all this in a simpler way. This is how the
Caddeus GNUmakefile was born. It's nothing more than the consolidation of
multiple well-known techniques into a single GNUmakefile.

This repository hosts the Caddeus GNUmakefile, but also is a sample "Hello
world" C project that uses it.

Features and characteristics
============================

* Multiple tests per unit. For a `unitname.o` target, tests named
  `tests/unitname/testname.t.c` can exist. A `unitname/testname.t` binary will
  be automatically compiled and run.

* Prevent useless tests from running if none of its dependencies has been
  touched. This works by creating a timestamp file when a tests succeed and
  using it along with GNU Make.

* Allow usage of scripted tests under the `.tt` extension.

* Application tests can be run after full build.

* CFLAGS are tight. Compiler errors usually indicate programming
  mistakes and should now let compilation be successful. They can be relaxed
  by the developer.

* Automatic test discovery. If a `.t.c` or `.tt` file exists under `tests/`
  it's an application test If a `.t.c` or `.tt` file exists under
  `tests/unitname`, it's a unit test.

* Optionally disable problematic tests by renaming them to something outside
  of `.t.c` or `.tt`, like `.t.c.disabled` or `.tt.disabled`.

* Automatic generation of dependency information. If `module.c` includes
  `module.h`, touching `module.h` will automatically force `module.o`
  regeneration.

* Lower targets depend on GNUmakefile. This causes the whole project
  rebuild if GNUmakefile changes. This ensures that a change on
  GNUmakefile does not break project build or build partially. This is
  tweakable, though.

* All the magic is located in a single GNUmakefile section to help prevent
  accidental modification.

* Independent libraries can be individually specified for each test
  by using `testname_TEST_LIBS` or `unitname/testname_TEST_LIBS` variables,

* Optional timeout support to kill a test if it infinite-loops.

* Automatic Valgrind Memcheck usage, if installed.

* Automatic Cppcheck usage if installed.

* Automatic Clang static analyzer usage, if installed.

* Automatic overriding of Valgrind, Cppcheck and Clang checks on release
  trees.
