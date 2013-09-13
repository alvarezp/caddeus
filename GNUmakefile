# Copyright (c) 2013, Octavio Alvarez <alvarezp@alvarezp.com>
# Released under the Simplified BSD License. See the LICENSE file.

# Automatic dependency generation adapted from
# http://www.scottmcpeak.com/autodepend/autodepend.html

CFLAGS += -Wall -std=c99 -pedantic-errors -fPIC -ggdb

.SECONDARY:

APP = app
APP_TESTS = apptest1.t
LIBS =

OBJS_TDD = hello.o
OBJS_NO_TDD = main.o

# ===== MODIFICATIONS SHOULD NOT BE NEEDED BELOW THIS LINE =====

APP_TESTS_TS = $(APP_TESTS:.tt=.tts) $(APP_TESTS:.t=.ts)
TESTS = $(OBJS_TDD:.o=.ts)
ALL_OBJS = $(OBJS_TDD) $(OBJS_NO_TDD)

# ===== MODIFICATIONS SHOULD REALLY NOT BE NEEDED BELOW THIS LINE =====

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
	gcc $(CFLAGS) -o $*.o -c $*.c
	@echo -e '\n'===== $@, generating dependency information...
	gcc $(CFLAGS) -MM -MP -MT $*.o $*.c > $*.d

%.ts: %.t
	@echo -e '\n'===== $@, testing...
	./$*.t && touch $*.ts;

%.tts: %.tt $(APP)
	@echo -e '\n'===== $@, testing...
	./$*.tt && touch $*.tts

%.t.c:
	@echo -e '\n'===== $@ doesn\'t exist\! Please create one.
	@false

%.tt:
	@echo -e '\n'===== $@ doesn\'t exist\! Please create one.
	@false

%.to: %.t.c GNUmakefile
	@echo -e '\n'===== $@, building test module...
	gcc $(CFLAGS) -o $*.to -c $*.t.c
	@echo -e '\n'===== $@, generating dependency information...
	gcc $(CFLAGS) -MM -MP -MT $*.to $*.t.c > $*.t.d

%.t: %.to %.o
	@echo -e '\n'===== $@, building test...
	gcc -o $*.t $^

%.t: %.to
	@echo -e '\n'===== $@, building test...
	gcc -o $*.t $^

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
