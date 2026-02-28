TARGET = bin/clox

CC = gcc
CFLAGS = -Wall -Wextra -O2 -W -pedantic -std=c99

SRCDIR = src
BUILDDIR = build
BINDIR = bin

SRCS = $(wildcard $(SRCDIR)/*/*.c)

OBJS = $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

all: dirs $(TARGET)

dirs:
	mkdir -p $(BUILDDIR)/chunk $(BUILDDIR)/common $(BUILDDIR)/compiler $(BUILDDIR)/debug $(BUILDDIR)/main $(BUILDDIR)/memory $(BUILDDIR)/scanner $(BUILDDIR)/value $(BUILDDIR)/vm
	mkdir -p $(BINDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all dirs run clean
