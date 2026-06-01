# Генератор лабиринтов и поиск пути

## Требования
- CMake 3.10+
- Компилятор C++17 (GCC, Clang, MSVC)

## Сборка, запуск и тесты

cd C:\maze_generator
cmake --build build
.\build\MazeGenerator.exe
.\build\unit_tests.exe
cmd /c "chcp 65001 > nul && .\build\scenario_tests.exe"

# Основная программа
docker-compose run --rm maze-app

# Тесты
docker-compose run --rm maze-tests

# Сценарии
docker-compose run --rm maze-scenarios

## Структура проекта

- include/maze.h — все классы
- src/main.cpp — меню и точка входа
- src/maze.cpp — Maze, генераторы, поисковики
- src/ui.cpp — отрисовка в консоли