CLOX_TARGET = bin/clox
TEST_TARGET = bin/clox_tests

CC = gcc
CFLAGS = -Wall -Wextra -O2 -W -pedantic -std=c99
TEST_CFLAGS = $(CFLAGS) -DUNIT_TESTING

SRCDIR = src
BUILDDIR = build
BINDIR = bin

# Основной clox - все файлы кроме тестовых
CLOX_SRCS = $(shell find $(SRCDIR) -name "*.c" \
            ! -name "*_test.c" \
            ! -path "*/test_mocks/*" \
            ! -path "*/test_framework/*" \
            ! -path "*/main/test_runner.c")
CLOX_OBJS = $(CLOX_SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/clox/%.o)

# Моки для тестов - переопределяют libc функции
MOCK_SRCS = $(wildcard $(SRCDIR)/test_mocks/*.c)
MOCK_OBJS = $(MOCK_SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/mocks/%.o)

# Тестовый фреймворк
FRAMEWORK_SRCS = $(wildcard $(SRCDIR)/test_framework/*.c)
FRAMEWORK_OBJS = $(FRAMEWORK_SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/framework/%.o)

# Тесты
TEST_MAIN = $(SRCDIR)/main/test_runner.c
TEST_MAIN_OBJ = $(BUILDDIR)/test/main/test_runner.o
TEST_SUITES = $(wildcard $(SRCDIR)/*/*_test.c)
TEST_SUITE_OBJS = $(TEST_SUITES:$(SRCDIR)/%.c=$(BUILDDIR)/test/%.o)

# Основные модули для тестов (без main/clox.c)
TEST_CORE_SRCS = $(filter-out $(SRCDIR)/main/clox.c, $(CLOX_SRCS))
TEST_CORE_OBJS = $(TEST_CORE_SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/clox/%.o)

all: dirs $(CLOX_TARGET) $(TEST_TARGET)

# Сборка clox
$(CLOX_TARGET): $(CLOX_OBJS) | dirs
	$(CC) $(CFLAGS) -o $@ $^

# Сборка тестов
# Моки идут первыми, чтобы переопределить символы libc
$(TEST_TARGET): $(MOCK_OBJS) $(FRAMEWORK_OBJS) $(TEST_CORE_OBJS) $(TEST_SUITE_OBJS) $(TEST_MAIN_OBJ) | dirs
	$(CC) $(TEST_CFLAGS) -o $@ $^

# Правило для компиляции основных файлов
$(BUILDDIR)/clox/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для компиляции моков
$(BUILDDIR)/mocks/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -c $< -o $@

# Правило для компиляции фреймворка
$(BUILDDIR)/framework/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -c $< -o $@

# Правило для компиляции тестов
$(BUILDDIR)/test/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -c $< -o $@

# Отдельное правило для test_runner
$(TEST_MAIN_OBJ): $(TEST_MAIN)
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -c $< -o $@

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
	mkdir -p $(BUILDDIR)/clox
	mkdir -p $(BUILDDIR)/mocks
	mkdir -p $(BUILDDIR)/framework
	mkdir -p $(BUILDDIR)/test/main
	mkdir -p $(BINDIR)

.PHONY: all run tests test-suite test-case clean dirs