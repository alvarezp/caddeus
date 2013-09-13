caddeus-framework
=================

This is a sample C project with an already prepared GNUmakefile for TDD
and strict compilation.

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
  GNUmakefile does not break project build.
* All modifiable parts are together in the top section of the GNUmakefile
  to avoid confusion.
* Independent libraries can be individually specified for each test
  by using the "testname_TEST_LIBS" variable, where 'testname' belongs
  to the testname.t file.
