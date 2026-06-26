CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

# Определение операционной системы для корректной линковки Check
OS := $(shell uname -s)
ifeq ($(OS), Linux)
    LIBS = -lcheck -lm -lsubunit -lrt -lpthread
else ifeq ($(OS), Darwin) # macOS
    LIBS = -lcheck -lm -lpthread
endif

# Названия финальных файлов и папок
LIB_NAME = s21_decimal.a
TEST_EXEC = test_runner
OBJ_DIR = obj
GCOV_DIR = gcov_report
TEST_DIR = tests

# Поиск всех исходных файлов библиотеки в корневом каталоге
SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

# Поиск всех файлов тестов внутри папки tests/
SRC_TEST = $(wildcard $(TEST_DIR)/*.c)
OBJ_TEST = $(SRC_TEST:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

# Сбор всех файлов проекта (.c и .h) для проверки стиля
ALL_FORMAT_FILES = $(wildcard *.c) $(wildcard *.h) $(wildcard $(TEST_DIR)/*.c) $(wildcard $(TEST_DIR)/*.h)

# Главная цель
all: $(LIB_NAME)

# Сборка статической библиотеки
$(LIB_NAME): $(OBJ)
	@ar rcs $@ $^
	@echo "--- Статическая библиотека $(LIB_NAME) успешно собрана ---"

# Компиляция объектных файлов библиотеки из корня
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Компиляция объектных файлов тестов из папки tests/ (добавляем -I. для поиска s21_decimal.h in root)
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

# Сборка и автоматический запуск тестов
test: $(LIB_NAME) $(OBJ_TEST)
	@$(CC) $(CFLAGS) $(OBJ_TEST) $(LIB_NAME) $(LIBS) -o $(TEST_EXEC)
	@echo "--- Запуск тестов ---"
	@./$(TEST_EXEC)

# Проверка на утечки памяти с помощью Valgrind (Memcheck)
valgrind: clean $(OBJ)
	@ar rcs $(LIB_NAME) $(OBJ)
	@mkdir -p $(OBJ_DIR)
	@echo "--- Сборка тестов с отладочной информацией ---"
	@$(CC) $(CFLAGS) -g -I. -c $(SRC_TEST)
	@mv *.o $(OBJ_DIR)/
	@$(CC) $(CFLAGS) -g $(OBJ_DIR)/*.o $(LIB_NAME) $(LIBS) -o $(TEST_EXEC)
	@echo "--- Анализ памяти с помощью Valgrind Memcheck ---"
	@valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_EXEC)

# Генерация HTML отчета о покрытии кода тестами
gcov_report: clean_gcov
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(GCOV_DIR)
	@echo "--- Компиляция библиотеки с поддержкой GCOV ---"
	@$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $(SRC)
	@mv *.o *.gcno $(OBJ_DIR)/
	@echo "--- Компиляция тестов ---"
	@$(CC) $(CFLAGS) -I. -c $(SRC_TEST)
	@mv *.o $(OBJ_DIR)/
	@$(CC) $(CFLAGS) $(GCOV_FLAGS) $(OBJ_DIR)/*.o $(LIBS) -o $(TEST_EXEC)
	@./$(TEST_EXEC)
	@mv *.gcda $(OBJ_DIR)/ 2>/dev/null || true
	@echo "--- Сбор статистики покрытия ---"
	@lcov -t "s21_decimal_tests" -o $(OBJ_DIR)/coverage.info -c -d $(OBJ_DIR)
	@genhtml -o $(GCOV_DIR) $(OBJ_DIR)/coverage.info
	@echo "--- Отчет успешно создан в папке $(GCOV_DIR)/index.html ---"

# Проверка стиля кода по стандарту Google
style_check:
	@echo "--- Проверка стиля clang-format (Google style) ---"
	@clang-format -style=Google -n --Werror $(ALL_FORMAT_FILES)
	@echo "--- Проверка стиля пройдена успешно ---"

# Автоматическое исправление стиля во всех файлах проекта
style_fix:
	@echo "--- Исправление стиля clang-format (Google style) ---"
	@clang-format -style=Google -i $(ALL_FORMAT_FILES)
	@echo "--- Форматирование файлов завершено ---"

# Полная очистка проекта
clean: clean_gcov
	@rm -rf $(OBJ_DIR)
	@rm -f $(LIB_NAME)
	@rm -f $(TEST_EXEC)
	@echo "--- Проект полностью очищен ---"

# Вспомогательная очистка файлов покрытия
clean_gcov:
	@rm -rf $(GCOV_DIR)
	@rm -f *.gcda *.gcno *.info

# Пересборка проекта с нуля
rebuild: clean all

# Указание, что эти цели не являются реальными файлами
.PHONY: all test valgrind gcov_report style_check style_fix clean clean_gcov rebuild
