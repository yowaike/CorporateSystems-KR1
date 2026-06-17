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
git clone https://github.com/yowaike/CorporateSystems-KR1.git
cd CorporateSystems-KR1
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/MazeGenerator
./build/unit_tests
./build/scenario_tests
```

## Сборка и запуск через Docker

```bash
docker build -t maze-generator .
docker run -it --rm maze-generator ./MazeGenerator
docker run --rm maze-generator ./unit_tests
docker run --rm maze-generator ./scenario_tests
```

## Запуск через Docker Compose

```bash
docker-compose run --rm maze-app
docker-compose run --rm maze-tests
docker-compose run --rm maze-scenarios
```

## Структура проекта

```
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
```

## Отчёт

Полный отчёт по проекту: [Казакова_ЭФБО_03-24.pdf](docs/Казакова_ЭФБО_03-24.pdf)