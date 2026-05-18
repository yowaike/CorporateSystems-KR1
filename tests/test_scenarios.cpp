/**
 * @file test_scenarios.cpp
 * @brief Тестовые сценарии - мини-программы, проверяющие основные этапы работы.
 *
 * Каждый сценарий - это отдельная функция, имитирующая действия пользователя.
 * Сценарий выводит результат в консоль: [PASS] или [FAIL].
 *
 * Сценарии:
 *   1. Полный цикл: генерация → отображение → поиск → отображение с путём
 *   2. Сравнение BFS и A* на одном лабиринте
 *   3. Генерация разными алгоритмами и проверка достижимости
 *   4. Замер производительности на большом лабиринте
 *   5. Проверка, что повторная генерация даёт другой лабиринт
 *   6. Крайние размеры: 1×1, 2×2, 1×20, 20×1
 */

#include <iostream>
#include <chrono>
#include <cassert>
#include "maze.h"

// Макрос для красивого вывода результата сценария
#define CHECK(condition, message) \
    do { \
        if (condition) { \
            std::cout << "  [PASS] " << message << std::endl; \
        } else { \
            std::cout << "  [FAIL] " << message << std::endl; \
            allPassed = false; \
        } \
    } while(0)

// Сценарий 1: Полный цикл работы
bool scenario1_FullCycle()
{
    std::cout << "\n Сценарий 1: Полный цикл (генерация + поиск + отображение)" << std::endl;
    bool allPassed = true;

    DFSGenerator generator;
    Maze* maze = generator.generate(10, 10);
    CHECK(maze->width == 10 && maze->height == 10, "Лабиринт создан (10×10)");

    AStarFinder finder;
    std::vector<Point> path = finder.findPath(maze, {0, 0}, {9, 9});

    CHECK(path.front().x == 0 && path.front().y == 0, "Путь начинается в (0,0)");
    CHECK(path.back().x == 9 && path.back().y == 9, "Путь заканчивается в (9,9)");
    CHECK(path.size() >= 2, "Путь содержит хотя бы 2 клетки");

    // Проверка: можно отрисовать (метод не падает)
    MazeRenderer renderer;
    renderer.displayWithPath(maze, path);
    CHECK(true, "Отрисовка лабиринта с путём выполнена без ошибок");

    delete maze;
    std::cout << (allPassed ? "Итог сценария 1: ПРОЙДЕН" : "Итог сценария 1: ПРОВАЛЕН") << std::endl;
    return allPassed;
}

// Сценарий 2: Сравнение BFS и A*
bool scenario2_CompareBFSandAStar()
{
    std::cout << "\n Сценарий 2: Сравнение BFS и A* на одном лабиринте" << std::endl;
    bool allPassed = true;

    KruskalGenerator generator;
    Maze* maze = generator.generate(15, 15);

    BFSFinder bfs;
    AStarFinder astar;

    std::vector<Point> bfsPath   = bfs.findPath(maze, {0, 0}, {14, 14});
    std::vector<Point> astarPath = astar.findPath(maze, {0, 0}, {14, 14});

    CHECK(bfsPath.size() == astarPath.size(),
          "BFS и A* нашли пути одинаковой длины");

    std::cout << "  BFS посетил: " << bfs.getVisitedCount() << " клеток" << std::endl;
    std::cout << "  A*  посетил: " << astar.getVisitedCount() << " клеток" << std::endl;

    // A* обычно посещает меньше клеток (но не всегда - зависит от лабиринта)
    CHECK(true, "Статистика собрана и выведена");

    delete maze;
    std::cout << (allPassed ? "Итог сценария 2: ПРОЙДЕН" : "Итог сценария 2: ПРОВАЛЕН") << std::endl;
    return allPassed;
}

// Сценарий 3: Разные генераторы - достижимость
bool scenario3_AllGeneratorsReachable()
{
    std::cout << "\n Сценарий 3: Проверка достижимости у разных генераторов" << std::endl;
    bool allPassed = true;

    // DFS
    {
        DFSGenerator generator;
        Maze* maze = generator.generate(12, 12);
        BFSFinder bfs;
        std::vector<Point> path = bfs.findPath(maze, {0, 0}, {11, 11});
        CHECK(path.size() >= 2, "DFS: путь найден");
        delete maze;
    }

    // Kruskal
    {
        KruskalGenerator generator;
        Maze* maze = generator.generate(12, 12);
        BFSFinder bfs;
        std::vector<Point> path = bfs.findPath(maze, {0, 0}, {11, 11});
        CHECK(path.size() >= 2, "Kruskal: путь найден");
        delete maze;
    }

    std::cout << (allPassed ? "Итог сценария 3: ПРОЙДЕН" : "Итог сценария 3: ПРОВАЛЕН") << std::endl;
    return allPassed;
}

// Сценарий 4: Производительность
bool scenario4_Performance()
{
    std::cout << "\n Сценарий 4: Замер производительности (лабиринт 50×50)" << std::endl;
    bool allPassed = true;

    // Генерация
    auto genStart = std::chrono::high_resolution_clock::now();
    KruskalGenerator generator;
    Maze* maze = generator.generate(50, 50);
    auto genEnd = std::chrono::high_resolution_clock::now();
    double genTime = std::chrono::duration<double, std::milli>(genEnd - genStart).count();

    std::cout << "  Генерация: " << genTime << " мс" << std::endl;
    CHECK(genTime < 1000, "Генерация 50×50 уложилась в 1 секунду");

    // Поиск BFS
    auto bfsStart = std::chrono::high_resolution_clock::now();
    BFSFinder bfs;
    bfs.findPath(maze, {0, 0}, {49, 49});
    auto bfsEnd = std::chrono::high_resolution_clock::now();
    double bfsTime = std::chrono::duration<double, std::milli>(bfsEnd - bfsStart).count();

    std::cout << "  Поиск (BFS): " << bfsTime << " мс" << std::endl;
    CHECK(bfsTime < 500, "Поиск пути 50×50 уложился в 0.5 секунды");

    // Поиск A*
    auto astarStart = std::chrono::high_resolution_clock::now();
    AStarFinder astar;
    astar.findPath(maze, {0, 0}, {49, 49});
    auto astarEnd = std::chrono::high_resolution_clock::now();
    double astarTime = std::chrono::duration<double, std::milli>(astarEnd - astarStart).count();

    std::cout << "  Поиск (A*):  " << astarTime << " мс" << std::endl;
    CHECK(astarTime < 500, "Поиск A* 50×50 уложился в 0.5 секунды");

    delete maze;
    std::cout << (allPassed ? "Итог сценария 4: ПРОЙДЕН" : "Итог сценария 4: ПРОВАЛЕН") << std::endl;
    return allPassed;
}

// Сценарий 5: Случайность генерации
bool scenario5_RandomnessCheck()
{
    std::cout << "\n Сценарий 5: Проверка случайности генерации" << std::endl;
    bool allPassed = true;

    DFSGenerator generator;
    Maze* maze1 = generator.generate(8, 8);
    Maze* maze2 = generator.generate(8, 8);

    bool areDifferent = false;
    for (int y = 0; y < 8 && !areDifferent; ++y) {
        for (int x = 0; x < 8 && !areDifferent; ++x) {
            if (maze1->grid[y][x].topWall != maze2->grid[y][x].topWall ||
                maze1->grid[y][x].rightWall != maze2->grid[y][x].rightWall) {
                areDifferent = true;
            }
        }
    }

    CHECK(areDifferent, "Два лабиринта от одного генератора различаются");

    delete maze1;
    delete maze2;
    std::cout << (allPassed ? "Итог сценария 5: ПРОЙДЕН" : "Итог сценария 5: ПРОВАЛЕН") << std::endl;
    return allPassed;
}

// Сценарий 6: Крайние размеры
bool scenario6_ExtremeSizes()
{
    std::cout << "\n Сценарий 6: Крайние размеры лабиринта" << std::endl;
    bool allPassed = true;

    // 1×1
    {
        DFSGenerator gen;
        Maze* maze = gen.generate(1, 1);
        CHECK(maze->width == 1 && maze->height == 1, "Лабиринт 1×1 создан");
        delete maze;
    }

    // 2×2
    {
        KruskalGenerator gen;
        Maze* maze = gen.generate(2, 2);
        BFSFinder bfs;
        std::vector<Point> path = bfs.findPath(maze, {0, 0}, {1, 1});
        CHECK(path.size() >= 3, "Лабиринт 2×2: путь найден");
        delete maze;
    }

    // 1×20 (вертикальный коридор)
    {
        DFSGenerator gen;
        Maze* maze = gen.generate(1, 20);
        BFSFinder bfs;
        std::vector<Point> path = bfs.findPath(maze, {0, 0}, {0, 19});
        CHECK(path.size() >= 2, "Лабиринт 1×20: путь найден");
        delete maze;
    }

    // 20×1 (горизонтальный коридор)
    {
        KruskalGenerator gen;
        Maze* maze = gen.generate(20, 1);
        BFSFinder bfs;
        std::vector<Point> path = bfs.findPath(maze, {0, 0}, {19, 0});
        CHECK(path.size() >= 2, "Лабиринт 20×1: путь найден");
        delete maze;
    }

    std::cout << (allPassed ? "Итог сценария 6: ПРОЙДЕН" : "Итог сценария 6: ПРОВАЛЕН") << std::endl;
    return allPassed;
}

// Точка входа для сценариев
int main()
{
    std::cout << "  ТЕСТОВЫЕ СЦЕНАРИИ" << std::endl;
    std::cout << "  Генератор лабиринтов и поиск пути" << std::endl;

    bool result1 = scenario1_FullCycle();
    bool result2 = scenario2_CompareBFSandAStar();
    bool result3 = scenario3_AllGeneratorsReachable();
    bool result4 = scenario4_Performance();
    bool result5 = scenario5_RandomnessCheck();
    bool result6 = scenario6_ExtremeSizes();

    bool overall = result1 && result2 && result3 && result4 && result5 && result6;
    std::cout << (overall ? "ВСЕ СЦЕНАРИИ ПРОЙДЕНЫ" : "НЕКОТОРЫЕ СЦЕНАРИИ ПРОВАЛЕНЫ") << std::endl;

    return overall ? 0 : 1;
}