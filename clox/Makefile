CLOX_TARGET = bin/clox
TEST_TARGET = bin/clox_tests

CC = gcc
CFLAGS = -Wall -Wextra -O2 -W -pedantic -std=c99

SRCDIR = src
BUILDDIR = build
BINDIR = bin

CLOX_MAIN = $(SRCDIR)/main/clox.c
TEST_MAIN = $(SRCDIR)/main/test_runner.c

ALL_SRCS = $(wildcard $(SRCDIR)/*/*.c)
CORE_SRCS = $(filter-out $(CLOX_MAIN) $(TEST_MAIN), $(ALL_SRCS))


CORE_OBJS = $(CORE_SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
CLOX_OBJ  = $(CLOX_MAIN:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
TEST_OBJ  = $(TEST_MAIN:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

all: dirs $(CLOX_TARGET) $(TEST_TARGET)

$(CLOX_TARGET): $(CORE_OBJS) $(CLOX_OBJ) | dirs
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_TARGET): $(CORE_OBJS) $(TEST_OBJ) | dirs
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(CLOX_TARGET)
	./$(CLOX_TARGET)

tests: $(TEST_TARGET)
	./$(TEST_TARGET)

test-suite: $(TEST_TARGET)
	./$(TEST_TARGET) $(SUITE)

test-case: $(TEST_TARGET)
	./$(TEST_TARGET) $(SUITE):$(TEST)

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

dirs:
	mkdir -p $(BUILDDIR)
	mkdir -p $(BINDIR)

.PHONY: all run tests test-suite test-case clean dirs