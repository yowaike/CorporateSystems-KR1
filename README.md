# Генератор лабиринтов и поиск пути

Консольное приложение на C++ для генерации случайных лабиринтов и поиска кратчайшего пути.

## Алгоритмы

### Генерация
- DFS (рекурсивный обход с возвратом)
- Kruskal (с системой непересекающихся множеств)

### Поиск пути
- BFS (обход в ширину)
- A* (с манхэттенской эвристикой)

## Требования

- CMake 3.10+
- Компилятор C++17 (GCC, Clang, MSVC)
- Google Test (подключается автоматически через CMake FetchContent)
- Docker 20.10+ (опционально, для контейнеризации)

## Сборка и запуск (локально)

```bash
# Клонирование
git clone https://github.com/yowaike/CorporateSystems-KR1.git
cd CorporateSystems-KR1

# Конфигурация и сборка
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Запуск основной программы
./build/MazeGenerator        # Linux/Mac
.\build\MazeGenerator.exe    # Windows

# Запуск unit-тестов (Google Test)
./build/unit_tests           
.\build\unit_tests.exe       

# Запуск тестовых сценариев
./build/scenario_tests       
.\build\scenario_tests.exe   

## Сборка и запуск через Docker

# Собрать образ
docker build -t maze-generator .

# Основная программа (интерактивный режим)
docker run -it --rm maze-generator ./MazeGenerator

# Unit-тесты
docker run --rm maze-generator ./unit_tests

# Тестовые сценарии
docker run --rm maze-generator ./scenario_tests

## Запуск через Docker Compose

docker-compose run --rm maze-app        # Основная программа
docker-compose run --rm maze-tests      # Unit-тесты
docker-compose run --rm maze-scenarios  # Тестовые сценарии

## Структура проекта

├── include/          # Заголовочные файлы
│   └── maze.h
├── src/              # Исходный код
│   ├── main.cpp
│   ├── maze.cpp
│   └── ui.cpp
├── tests/            # Тесты
│   ├── test_main.cpp
│   ├── test_maze.cpp
│   ├── test_generators.cpp
│   ├── test_pathfinders.cpp
│   └── test_scenarios.cpp
├── CMakeLists.txt    # Сценарий сборки
├── Dockerfile        # Сборка Docker-образа
└── README.md

## Отчёт

Полный отчёт по проекту: [Казакова_ЭФБО_03-24.pdf](docs/Казакова_ЭФБО_03-24.pdf)