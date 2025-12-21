EXEC = pi-julia-set
CFLAGS = -Wall -std=gnu99 -lm
DEBUG = -DDEBUG -g -Og

OBJDIR = obj
SRCDIR = src

SRCS := $(shell find $(SRCDIR) -name "*.c")
OBJS = $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRCS:%.c=%.o))
DEPS = $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRCS:%.c=%.d))

.PHONEY: release debug clean
debug: CFLAGS += $(DEBUG)
debug: release

release: $(EXEC)

$(EXEC): $(OBJDIR)/ $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/%.d: $(SRCDIR)/%.c
	@set -e
	rm -f $@
	$(CC) -MM -I $(SRCDIR) $(CFLAGS) $< > $@.tmp
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@
	rm -f $@.tmp

$(OBJDIR)/:
	mkdir $(OBJDIR)

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPENDS)
endif

clean:
	rm -f $(OBJDIR)/* $(EXEC)
