OUT = bin

SEARCHPATH += $(shell find src -type d | sort)

vpath %.c $(SEARCHPATH)
vpath %.h $(SEARCHPATH)

DEPS += defs.h structs.h

_OBJS = $(shell find src -iname *.c -type f -exec basename {} \; | sed 's/\.c/\.o/g' | sort | grep -v win32)

OBJS = $(patsubst %,$(OUT)/%,$(_OBJS))

# top-level rule to create the program.
all: $(PROG)

# compiling other source files.
$(OUT)/%.o: %.c %.h $(DEPS)
	@mkdir -p $(OUT)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c -o $@ $<

# cleaning everything that can be automatically recreated with "make".
clean:
	$(RM) -rf $(OUT) $(PROG)
