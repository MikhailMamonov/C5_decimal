# s21_decimal

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/MikhailMamonov/C5_decimal)
[![Coverage](https://img.shields.io/badge/coverage-100%25-brightgreen)](https://github.com/MikhailMamonov/C5_decimal)
[![Language](https://img.shields.io/badge/language-C-blue)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Standard](https://img.shields.io/badge/standard-C11-blue)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))

Данный проект реализует пользовательскую библиотеку `s21_decimal` на языке программирования C. Она предоставляет тип данных с фиксированной точкой для точных финансовых расчётов, устраняя ошибки округления, присущие стандарту IEEE 754 с плавающей запятой.

Библиотека эмулирует поведение типа `decimal` из C#, поддерживая арифметические, сравнительные, конвертирующие и округляющие операции.

---

## 🚀 Основные возможности

- **Арифметические операции**: сложение, вычитание, умножение и деление с банковским округлением.
- **Операторы сравнения**: `<`, `<=`, `>`, `>=`, `==` и `!=`.
- **Функции преобразования**: `int` ↔ `decimal` и `float` ↔ `decimal`.
- **Вспомогательные функции**: `floor`, `round`, `truncate` и `negate`.
- **Высокое покрытие тестами**: модульные тесты покрывают более 80% кода каждой функции.

## 🛠️ Технический стек

- **Язык**: C (стандарт C11)
- **Компилятор**: `gcc`
- **Фреймворк для тестирования**: [Check](https://libcheck.github.io/check/)
- **Покрытие кода**: `gcov` и `lcov`
- **Сборка**: `Makefile`
- **Стиль кодирования**: [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

---

## 📁 Структура проекта

Проект имеет следующую структуру:
C5_decimal/
├── src/ # Исходный код библиотеки
│ ├── s21_decimal.h # Основной заголовочный файл
│ ├── s21_.c # Файлы реализации функций
│ └── tests/ # Модульные тесты
│ └── s21__test.c # Файлы с тестами
├── materials/ # Материалы и спецификации проекта
├── misc/ # Различные файлы (изображения и т.д.)
├── Makefile # Автоматизация сборки
├── README.md # Этот файл
└── LICENSE # Информация о лицензии

text

## 🧱 Двоичное представление

Тип `s21_decimal` реализован как 128-битная структура, определённая в файле `s21_decimal.h`:

```c
typedef struct {
    int bits[4];
} s21_decimal;
```
bits[0], bits[1], bits[2]: содержат младшие, средние и старшие 32 бита 96-битного целого числа.

bits[3]: содержит коэффициент масштабирования (показатель степени от 0 до 28) в битах 16–23 и знак (бит 31).

Подробности описаны в документации проекта.

--- 

## 📦 Установка и сборка
Сборка библиотеки
```bash
make s21_decimal.a
```

Запуск всех модульных тестов

```bash
make test
```

Генерация отчёта о покрытии кода

```bash
make gcov_report
```

Очистка артефактов сборки

```bash
make clean
```

## 🧪 Пример использования

```c
#include "s21_decimal.h"
#include <stdio.h>

int main() {
    s21_decimal a = {{0}};
    s21_decimal b = {{0}};
    s21_decimal result = {{0}};

    s21_from_int_to_decimal(10, &a);
    s21_from_int_to_decimal(3, &b);

    s21_div(a, b, &result); // 10 / 3

    float res_float;
    s21_from_decimal_to_float(result, &res_float);
    printf("Результат: %f\n", res_float); // Вывод: 3.333333...

    return 0;
}
```

## 🤝 Вклад в проект

Этот проект является учебной реализацией в рамках программы Школы 21. Тем не менее, предложения и улучшения приветствуются! Вы можете форкнуть репозиторий и отправить pull request.
