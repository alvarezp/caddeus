# Copyright (c) 2013, Octavio Alvarez <alvarezp@alvarezp.com>
# Released under the Simplified BSD License. See the LICENSE file.

# Automatic dependency generation adapted from
# http://www.scottmcpeak.com/autodepend/autodepend.html

CFLAGS += -Wall -std=c99 -pedantic-errors -fPIC -ggdb

.SECONDARY:

APP = app
OBJS_TDD = hello.o
OBJS_NO_TDD = main.o
ALL_OBJS = $(OBJS_TDD) $(OBJS_NO_TDD)
TESTS = $(OBJS_TDD:.o=.ts)
LIBS =

.PHONY : all
all: $(APP)

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

%.t.c:
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


