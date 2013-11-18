# Copyright (c) 2013, Octavio Alvarez <alvarezp@alvarezp.com>
# Released under the Simplified BSD License. See the LICENSE file.

# Automatic dependency generation adapted from
# http://www.scottmcpeak.com/autodepend/autodepend.html

CFLAGS += -Wall -std=c99 -pedantic-errors -fPIC -ggdb

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

APP_TESTS_TS = $(APP_TESTS:.tt=.tts) $(APP_TESTS:.t=.ts)
TESTS = $(OBJS_TDD:.o=.ts)
ALL_OBJS = $(OBJS_TDD) $(OBJS_NO_TDD)

VALGRIND_LINE = valgrind --error-exitcode=255 --leak-check=full -q --track-origins=yes

CPPCHECK_LINE = cppcheck --error-exitcode=1 --std=c99 --quiet

# ===== MODIFICATIONS SHOULD REALLY NOT BE NEEDED BELOW THIS LINE =====

DONT_HAVE_VALGRIND = $(if $(shell which valgrind),,y)
THIS_IS_A_RELEASE = $(shell ls RELEASE 2>/dev/null)

VALGRIND = $(if $(or $(DONT_HAVE_VALGRIND),$(SKIP_VALGRIND),$(THIS_IS_A_RELEASE)),,$(VALGRIND_LINE) $(VALGRIND_EXTRA))

DONT_HAVE_CPPCHECK = $(if $(shell which cppcheck),,y)

CPPCHECK = $(if $(or $(DONT_HAVE_CPPCHECK),$(SKIP_CPPCHECK),$(THIS_IS_A_RELEASE)),,$(CPPCHECK_LINE) $(CPPCHECK_EXTRA))

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
all: $(APP) $(APP_TESTS_TS)
	@echo "Build completed successfully."

# Pull in dependency info for existing .o and .t files.
-include $(ALL_OBJS:.o=.d)
-include $(ALL_OBJS:.o=.t.d)

# All lower targets depend on GNUmakefile so everything rebuilds if GNUmakefile
# changes.
GNUmakefile:

$(APP): $(ALL_OBJS) $(TESTS)
	@echo -e '\n'===== $@, building app...
	gcc -o $(APP) $(LIBS) $(OBJS_TDD) $(OBJS_NO_TDD)

# Compile plus generate dependency information.
%.o: %.c GNUmakefile
	@echo -e '\n'===== $@, building module...
	$(CPPCHECK) $*.c
	gcc $(CFLAGS) -o $*.o -c $*.c
	@echo -e '\n'===== $@, generating dependency information...
	gcc $(CFLAGS) -MM -MP -MT $*.o $*.c > $*.d

%.ts: %.t
	$(eval CALL_TIMEOUT=$(call multiply,$(firstword $($(@:.ts=_TIMEOUT_MULT)) 1),$(DEFAULT_TIMEOUT)))
	@echo -e '\n'===== $@, running test with timeout=$(CALL_TIMEOUT)...
	timeout $(CALL_TIMEOUT) $(VALGRIND) ./$*.t && touch $*.ts

%.tts: %.tt $(APP)
	$(eval CALL_TIMEOUT=$(call multiply,$(firstword $($(@:.tts=_TIMEOUT_MULT)) 1),$(DEFAULT_TIMEOUT)))
	@echo -e '\n'===== $@, running test with timeout=$(CALL_TIMEOUT)...
	timeout $(CALL_TIMEOUT) $(VALGRIND) ./$*.tt && touch $*.tts

%.t.c:
	@echo -e '\n'===== $@ doesn\'t exist\! Please create one.
	@false

%.tt:
	@echo -e '\n'===== $@ doesn\'t exist\! Please create one.
	@false

%.to: %.t.c GNUmakefile
	@echo -e '\n'===== $@, building test module...
	$(CPPCHECK) $*.t.c
	gcc $(CFLAGS) -o $*.to -c $*.t.c
	@echo -e '\n'===== $@, generating dependency information...
	gcc $(CFLAGS) -MM -MP -MT $*.to $*.t.c > $*.t.d

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
	rm -f *.o
	rm -f *.d
	rm -f *.t
	rm -f *.to
	rm -f *.ts
	rm -f *.tts

.PHONY : force
force:
	$(MAKE) clean
	$(MAKE) all
