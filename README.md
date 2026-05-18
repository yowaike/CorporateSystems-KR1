# Генератор лабиринтов и поиск пути

## Требования
- CMake 3.10+
- Компилятор C++17 (GCC, Clang, MSVC)

## Сборка и запуск

cmake -S . -B build
cmake --build build
cd build
./MazeGenerator

## Структура проекта

- include/maze.h — все классы
- src/main.cpp — меню и точка входа
- src/maze.cpp — Maze, генераторы, поисковики
- src/ui.cpp — отрисовка в консоли