.SECONDARY:

APP = hello
LIBS =

OBJS = hello.o main.o

#To add libraries to a test, use the "unitname/testname_TEST_LIBS" variable:
#hello/hello_TEST_LIBS=-lm

#Same goes to integration test:
apptest1_TEST_LIBS=-lm
apptest1_TIMEOUT_MULT=2

CLEAN_MORE =

#These defaults are really aggressive. You may want to tweak them.
WNO_ERROR = -Wno-error=unused-variable -Wno-error=unused-parameter
CFLAGS += -Wall -Wextra -Werror -std=c99 -pedantic-errors $(WNO_ERROR) -fPIC -ggdb
VALGRIND_EXTRA = --suppressions=/dev/null
#CPPCHECK_EXTRA = --suppress=...

#Strictly speaking you should rebuild your entire project if you change the
#GNUmakefile, but it can be quite cumbersome if your project is really big
#and you are debugging or hacking the GNUmakefile.
REBUILD_ON=GNUmakefile

# ===== MODIFICATIONS SHOULD NOT BE NEEDED BELOW THIS LINE =====

VALGRIND_LINE = valgrind --error-exitcode=255 --leak-check=full -q --track-origins=yes

CPPCHECK_LINE = cppcheck --error-exitcode=1 --std=c99 --quiet

CLANG_LINE = clang --analyze -pedantic

# Disable builtin rules. This lets us avoid all kinds of surprises.
.SUFFIXES:

# ===== MODIFICATIONS SHOULD REALLY NOT BE NEEDED BELOW THIS LINE =====

# Copyright (c) 2013, Octavio Alvarez <alvarezp@alvarezp.com>
# Released under the Simplified BSD License. See the LICENSE file.

# Automatic dependency generation adapted from
# http://www.scottmcpeak.com/autodepend/autodepend.html

# Arithmetic taken from this amazing article by John Graham-Cumming:
# http://www.cmcrossroads.com/article/learning-gnu-make-functions-arithmetic
16 := x x x x x x x x x x x x x x x
input_int := $(foreach a,$(16),$(foreach b,$(16),$(foreach c,$(16),$(16))))
decode = $(words $1)
encode = $(wordlist 1,$1,$(input_int))
multiply = $(call decode,$(foreach a,$(call encode,$1),$(call encode,$2)))


APP_TESTS = $(wildcard tests/*.t.c)
APP_TESTS_OBJ = $(patsubst tests/%.t.c,.caddeus/testobj/%.to,$(APP_TESTS))
APP_TESTS_BIN = $(patsubst tests/%.t.c,.caddeus/testbin/%.t,$(APP_TESTS))
APP_TESTS_TS = $(patsubst tests/%.t.c,.caddeus/timestamps/%.ts,$(APP_TESTS))

APP_TESTS_TT = $(wildcard tests/*.tt)
APP_TESTS_TTS = $(patsubst tests/%.tt,.caddeus/timestamps/%.tts,$(APP_TESTS_TT))

OBJ_TESTS = $(foreach d,$(patsubst %.o,tests/%/*.t.c,$(OBJS)),$(wildcard $(d)))
OBJ_TESTS_OBJ = $(patsubst tests/%.t.c,.caddeus/testobj/%.to,$(OBJ_TESTS))
OBJ_TESTS_BIN = $(patsubst tests/%.t.c,.caddeus/testbin/%.t,$(OBJ_TESTS))
OBJ_TESTS_TS = $(patsubst tests/%.t.c,.caddeus/timestamps/%.ts,$(OBJ_TESTS))

#Make each ./tests/unitname/testname.t depend on ./unitname.o.
$(foreach f,$(OBJ_TESTS_BIN),$(call eval,$f:$(word 3,$(subst /, ,$f)).o))

DONT_HAVE_VALGRIND = $(if $(shell which valgrind),,y)
THIS_IS_A_RELEASE = $(shell ls RELEASE 2>/dev/null)

VALGRIND = $(if $(or $(DONT_HAVE_VALGRIND),$(SKIP_VALGRIND),$(THIS_IS_A_RELEASE)),,$(VALGRIND_LINE) $(VALGRIND_EXTRA))

DONT_HAVE_CPPCHECK = $(if $(shell which cppcheck),,y)

CPPCHECK = $(if $(or $(DONT_HAVE_CPPCHECK),$(SKIP_CPPCHECK),$(THIS_IS_A_RELEASE)),true '-- skipping Cppcheck --',$(CPPCHECK_LINE) $(CPPCHECK_EXTRA))

DONT_HAVE_CLANG = $(if $(shell which clang),,y)

CLANG = $(if $(or $(DONT_HAVE_CLANG),$(SKIP_CLANG),$(THIS_IS_A_RELEASE)),true '-- skipping Clang --',$(CLANG_LINE) $(CLANG_EXTRA))

CFLAGS := $(if $(THIS_IS_A_RELEASE),-DNDEBUG,) $(CFLAGS)

DEFAULT_TIMEOUT=0
ifdef TIMEOUT
	DEFAULT_TIMEOUT=$(TIMEOUT)
endif

.DEFAULT_GOAL := all

.PHONY : all
all: $(APP) $(APP_TESTS_TTS) $(APP_TESTS_TS)
	@echo
	@echo "Build completed successfully."

# Pull in dependency info for existing .o and .t files.
-include $(patsubst %.o,.caddeus/dependencies/%.d,$(OBJS))
-include $(patsubst %.o,.caddeus/dependencies/tests/%.t.d,$(OBJS))

# All lower targets depend on $(REBUILD_ON) so everything rebuilds if $(REBUILD_ON)
# changes.
$(REBUILD_ON):

$(APP): $(OBJS) $(OBJ_TESTS_TS)
	@echo -e '\n'===== $@, building app...
	gcc -o $(APP) $(OBJS) $(LIBS)

# Compile plus generate dependency information.
%.o: %.c $(REBUILD_ON)
	@echo -e '\n'===== $@, building module...
	$(CPPCHECK) $<
	@mkdir -p .caddeus/clang/$(*D)
	$(CLANG) $(CFLAGS) -o .caddeus/clang/$*.plist $<
	gcc $(CFLAGS) -o $@ -c $<
	@echo -e '\n'===== $@, generating dependency information...
	@mkdir -p .caddeus/dependencies/$(*D)
	gcc $(CPPFLAGS) $(CFLAGS) -M $< | sed '1s,^\(.*\).o:,$*.o:,' \
	  > .caddeus/dependencies/$*.d

.caddeus/timestamps/%.ts: .caddeus/testbin/%.t
	$(eval CALL_TIMEOUT=$(call multiply,$(firstword $($*_TIMEOUT_MULT) 1),$(DEFAULT_TIMEOUT)))
	@echo -e '\n'===== running test \"$*\" with timeout=$(CALL_TIMEOUT)...
	@mkdir -p $(@D)
	timeout $(CALL_TIMEOUT) $(VALGRIND) $< && touch $@

.caddeus/timestamps/%.tts: tests/%.tt $(APP)
	$(eval CALL_TIMEOUT=$(call multiply,$(firstword $($*_TIMEOUT_MULT) 1),$(DEFAULT_TIMEOUT)))
	@echo -e '\n'===== running test \"$*\" test with timeout=$(CALL_TIMEOUT)...
	@mkdir -p $(@D)
	timeout $(CALL_TIMEOUT) $< && touch $@

.caddeus/testobj/%.to: tests/%.t.c $(REBUILD_ON)
	@echo -e '\n'===== $@, building test module...
	$(CPPCHECK) -I. $<
	@mkdir -p .caddeus/clang/tests/$(*D)
	$(CLANG) $(CFLAGS) -I. -o .caddeus/clang/tests/$*.t.plist $<
	@mkdir -p $(@D)
	gcc $(CFLAGS) -I. -o $@ -c $<
	@echo -e '\n'===== $@, generating dependency information...
	@mkdir -p .caddeus/dependencies/tests/$(*D)
	gcc $(CPPFLAGS) $(CFLAGS) -I. -M $< | sed '1s,^\(.*\).to:,$*.to:,' \
	  > .caddeus/dependencies/tests/$*.t.d

.caddeus/testbin/%.t: .caddeus/testobj/%.to
	@echo -e '\n'===== $@, building test...
	@mkdir -p $(@D)
	gcc -o $@ $^ $($*_TEST_LIBS)

.PHONY : clean
clean:
	@echo -e '\n'===== Cleaning...
	rm -fr .caddeus
	rm -f $(APP)
	rm -f $(OBJS)
	rm -f $(CLEAN_MORE)

.PHONY : force
force:
	$(MAKE) clean
	$(MAKE) all
