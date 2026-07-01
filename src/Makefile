CC=gcc
CFLAGS=-c -Wall -Wextra -Werror -std=c11 -I.
OFLAGS=-Wall -Wextra -Werror -std=c11 -I.
GCOVFLAGS=-fprofile-arcs -ftest-coverage

SOURCE_DIR = .
TEST_DIR = ./tests
BUILD_DIR = ../build

TARGET_NAME = s21_decimal.a

TEST_NAME = $(BUILD_DIR)/unittest
COVERAGE_NAME = $(BUILD_DIR)/coverage.info

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)

OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SOURCES))
.PHONY: all clean test unittest cppcheck memcheck gcov_report clang clang_check

all: $(TARGET_NAME)

$(TARGET_NAME): $(BUILD_DIR) $(OBJECTS)
	ar -rs $@ $(OBJECTS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@echo "Compiling $< to $@"
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@echo "Compiling $< to $@"
	$(CC) $(CFLAGS) $(shell pkg-config --cflags check) $< -o $@

test: unittest
	./$(TEST_NAME)
 
unittest: $(BUILD_DIR) $(TARGET_NAME) $(TEST_OBJECTS)
	$(CC) $(OFLAGS) $(TEST_OBJECTS) $(shell pkg-config --libs check) -L . $(TARGET_NAME) -lm -o $(TEST_NAME)

gcov_report: $(BUILD_DIR) $(TARGET_NAME) $(TEST_OBJECTS)
	$(CC) $(OFLAGS) $(GCOVFLAGS) $(TEST_OBJECTS) $(SOURCES) $(shell pkg-config --libs check) -L . -lm -o unittest
	./unittest
	lcov -t "gcov_report" --no-external -o coverage.info -c -d .
	genhtml -q -s -o ./report coverage.info
	rm -f unittest coverage.info *.gcda *.gcno

clang_check:
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.c
	clang-format -n *.h
	clang-format -n */*.c
	@rm .clang-format

clang:
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c
	clang-format -i *.h
	clang-format -i */*.c
	@rm -rf tests/*temp*
	@rm .clang-format

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(SOURCE_DIR)/$(TARGET_NAME)
	@rm -rf $(TEST_NAME)
	@rm -rf $(SOURCE_DIR)/report
	@rm -rf $(SOURCE_DIR)/$(COVERAGE_NAME)
	@rm -rf tests/*temp*
	@find . -name '*.gcov' -delete
	@find . -name '*.gcda' -delete
	@find . -name '*.gcno' -delete
	@rm -f valgrind_report.log
	
cppcheck:
	cppcheck --suppress=checkersReport --enable=all --suppress=missingIncludeSystem $(SOURCE_DIR)/*.c $(SOURCE_DIR)/*.h $(TEST_DIR)/*.c

memcheck: test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_report.log ./$(TEST_NAME)
	cat valgrind_report.log