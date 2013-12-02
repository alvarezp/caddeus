About Caddeus
=============

The quality and security of many C projects solely depend on their developers
practices. Although there are well known techniques, adopting them is an opt-in
process. Developers use analyzers only when the need is perceived, only for
specific program flows and only once. By the time the program proves to have a
regression, adopting a framework can be a daunting task, up to requiring code
redesign.

Program correctness over speed is particularly important for libraries.
Developers that rely on a library can see their debugging process encumbered
if the library has bugs of its own. As a particularly important case,
regressions on a library may affect multiple programs, and tests on a program
may fail because of a library.

By using a TDD-oriented Makefile from the project start, project owners can
enforce a building paradigm with conventions for automatic analysis and easy
TDD adoption without running irrelevant tests.

Caddeus is such an attempt.

This repository hosts the Caddeus makefile, but also is a sample C project that
uses it.

Features and characteristics
============================

* Have one test file per module. For a module.o target, a test named
  module.t.c can exist. A module.t binary will be compiled and run.
  If succeeds, a timestamp file module.ts file will be generated.
* Allow usage of scripted tests under the .tt extension.
* Allows specification of manual tests to run after full build.
* Dependency chain for modules:
  - module.ts: module.t
  - module.t: module.to module.o
  - module.to: module.t.c GNUmakefile
  - module.o: module.c GNUmakefile
* Dependency chain for the final binary:
  - app: all-objects all-test-timestamps
* Dependencies for the compilation process:
  - all: app app-tests
* CFLAGS are tight. Compiler errors usually indicate programming
  mistakes and should now let compilation be successful.
* Allows testing to be required for some but not all modules by
  specifying objects in OBJS_TDD and OBJS_NO_TDD.
* Will automatically generate module.d with generated dependency
  information. If module.c includes module.h, module.d will say
  module.c: module.h and included each time make is run.
* Lower targets depend on GNUmakefile. This causes the whole project
  rebuild if GNUmakefile changes. This ensures that a change on
  GNUmakefile does not break project build or build partially.
* All modifiable parts are together in the top section of the GNUmakefile
  to avoid confusion.
* Independent libraries can be individually specified for each test
  by using the "testname_TEST_LIBS" variable, where 'testname' belongs
  to the testname.t file.
* Optional timeout support to kill a test if it cycles.
* Automatic Valgrind Memcheck usage, if available.
* Automatic Cppcheck usage if available.
* Automatic Clang static analyzer usage, if available.
* Automatic definition of NDEBUG on release trees.
* Automatic overriding of Valgrind, Cppcheck and Clang checks on release
  trees
