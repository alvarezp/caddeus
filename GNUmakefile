# Copyright (c) 2013, Octavio Alvarez <alvarezp@alvarezp.com>
# Released under the Simplified BSD License. See the LICENSE file.

# Automatic dependency generation adapted from
# http://www.scottmcpeak.com/autodepend/autodepend.html

WNO_ERROR = -Wno-error=unused-variable

CFLAGS += -Wall -Wextra -Werror -std=c99 -pedantic-errors $(WNO_ERROR) -fPIC -ggdb

VALGRIND_EXTRA = --suppressions=/dev/null

#CPPCHECK_EXTRA = --suppress=...

.SECONDARY:

APP = app
APP_TESTS = apptest1.t
LIBS =

OBJS_TDD = hello.o
OBJS_NO_TDD = main.o

apptest1.t: apptest2.to
apptest1_TEST_LIBS=-lm
apptest1_TIMEOUT_MULT=2

# ===== MODIFICATIONS SHOULD NOT BE NEEDED BELOW THIS LINE =====

APP_TESTS_TS = $(patsubst %.t,.caddeus/timestamps/%.ts,$(filter %.t,$(APP_TESTS)))
APP_TESTS_TTS = $(patsubst %.tt,.caddeus/timestamps/%.tts,$(filter %.tt,$(APP_TESTS)))
OBJ_TESTS_TS = $(patsubst %.o,.caddeus/timestamps/%.ts,$(OBJS_TDD))
ALL_OBJS = $(OBJS_TDD) $(OBJS_NO_TDD)

VALGRIND_LINE = valgrind --error-exitcode=255 --leak-check=full -q --track-origins=yes

CPPCHECK_LINE = cppcheck --error-exitcode=1 --std=c99 --quiet

CLANG_LINE = clang --analyze -pedantic

# ===== MODIFICATIONS SHOULD REALLY NOT BE NEEDED BELOW THIS LINE =====

DONT_HAVE_VALGRIND = $(if $(shell which valgrind),,y)
THIS_IS_A_RELEASE = $(shell ls RELEASE 2>/dev/null)

VALGRIND = $(if $(or $(DONT_HAVE_VALGRIND),$(SKIP_VALGRIND),$(THIS_IS_A_RELEASE)),,$(VALGRIND_LINE) $(VALGRIND_EXTRA))

DONT_HAVE_CPPCHECK = $(if $(shell which cppcheck),,y)

CPPCHECK = $(if $(or $(DONT_HAVE_CPPCHECK),$(SKIP_CPPCHECK),$(THIS_IS_A_RELEASE)),true '-- skipping Cppcheck --',$(CPPCHECK_LINE) $(CPPCHECK_EXTRA))

DONT_HAVE_CLANG = $(if $(shell which clang),,y)

CLANG = $(if $(or $(DONT_HAVE_CLANG),$(SKIP_CLANG),$(THIS_IS_A_RELEASE)),true '-- skipping Clang --',$(CLANG_LINE) $(CLANG_EXTRA))

CFLAGS := $(if $(THIS_IS_A_RELEASE),-DNDEBUG,) $(CFLAGS)

# Arithmetic taken from this amazing article by John Graham-Cumming:
# http://www.cmcrossroads.com/article/learning-gnu-make-functions-arithmetic
16 := x x x x x x x x x x x x x x x
input_int := $(foreach a,$(16),$(foreach b,$(16),$(foreach c,$(16),$(16))))
decode = $(words $1)
encode = $(wordlist 1,$1,$(input_int))
multiply = $(call decode,$(foreach a,$(call encode,$1),$(call encode,$2)))

DEFAULT_TIMEOUT=0
ifdef TIMEOUT
	DEFAULT_TIMEOUT=$(TIMEOUT)
endif

.DEFAULT_GOAL := all

.PHONY : all
all: $(APP) $(APP_TESTS_TTS) $(APP_TESTS_TS)
	@echo "Build completed successfully."

# Pull in dependency info for existing .o and .t files.
-include $(patsubst %.o,.caddeus/dependencies/%.d,$(ALL_OBJS))
-include $(patsubst %.o,.caddeus/dependencies/%.t.d,$(ALL_OBJS))

# All lower targets depend on GNUmakefile so everything rebuilds if GNUmakefile
# changes.
GNUmakefile:

$(APP): $(ALL_OBJS) $(OBJ_TESTS_TS)
	@echo -e '\n'===== $@, building app...
	gcc -o $(APP) $(OBJS_TDD) $(OBJS_NO_TDD) $(LIBS)

# Compile plus generate dependency information.
%.o: %.c GNUmakefile
	@echo -e '\n'===== $@, building module...
	$(CPPCHECK) $*.c
	@mkdir -p .caddeus/clang/$(*D)
	$(CLANG) $(CFLAGS) -o .caddeus/clang/$*.plist $*.c
	gcc $(CFLAGS) -o $*.o -c $*.c
	@echo -e '\n'===== $@, generating dependency information...
	@mkdir -p .caddeus/dependencies/$(*D)
	gcc $(CFLAGS) -MM -MP -MT $*.o $*.c > .caddeus/dependencies/$*.d

.caddeus/timestamps/%.ts: %.t
	$(eval CALL_TIMEOUT=$(call multiply,$(firstword $($(@:.ts=_TIMEOUT_MULT)) 1),$(DEFAULT_TIMEOUT)))
	@echo -e '\n'===== $@, running test with timeout=$(CALL_TIMEOUT)...
	@mkdir -p .caddeus/timestamps/$(@D)
	timeout $(CALL_TIMEOUT) $(VALGRIND) ./$*.t && touch $@

.caddeus/timestamps/%.tts: %.tt $(APP)
	$(eval CALL_TIMEOUT=$(call multiply,$(firstword $($(@:.tts=_TIMEOUT_MULT)) 1),$(DEFAULT_TIMEOUT)))
	@echo -e '\n'===== $@, running test with timeout=$(CALL_TIMEOUT)...
	@mkdir -p .caddeus/timestamps/$(@D)
	timeout $(CALL_TIMEOUT) $(VALGRIND) ./$*.tt && touch $@

%.t.c:
	@echo -e '\n'===== $@ doesn\'t exist\! Please create one.
	@false

%.tt:
	@echo -e '\n'===== $@ doesn\'t exist\! Please create one.
	@false

%.to: %.t.c GNUmakefile
	@echo -e '\n'===== $@, building test module...
	$(CPPCHECK) $*.t.c
	@mkdir -p .caddeus/clang/$(*D)
	$(CLANG) $(CFLAGS) -o .caddeus/clang/$*.t.plist $*.t.c
	gcc $(CFLAGS) -o $*.to -c $*.t.c
	@echo -e '\n'===== $@, generating dependency information...
	@mkdir -p .caddeus/dependencies/$(*D)
	gcc $(CFLAGS) -MM -MP -MT $*.to $*.t.c > .caddeus/dependencies/$*.t.d

%.t: %.to %.o
	@echo -e '\n'===== $@, building test...
	gcc -o $*.t $^ $($(@:.t=_TEST_LIBS))

%.t: %.to
	@echo -e '\n'===== $@, building test...
	gcc -o $*.t $^ $($(@:.t=_TEST_LIBS))

.PHONY : clean
clean:
	@echo -e '\n'===== Cleaning...
	rm -f $(APP)
	rm -fr .caddeus
	rm -f *.o
	rm -f *.t
	rm -f *.to

.PHONY : force
force:
	$(MAKE) clean
	$(MAKE) all
