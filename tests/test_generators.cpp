/**
 * @file test_generators.cpp
 * @brief Модульные тесты для алгоритмов генерации лабиринта.
 */

#include <gtest/gtest.h>
#include <iostream>
#include <queue>
#include "maze.h"

int countOpenPassages(Maze* maze)
{
    int count = 0;
    for (int y = 0; y < maze->height; ++y)
        for (int x = 0; x < maze->width; ++x) {
            if (!maze->grid[y][x].topWall)    count++;
            if (!maze->grid[y][x].rightWall)  count++;
            if (!maze->grid[y][x].bottomWall) count++;
            if (!maze->grid[y][x].leftWall)   count++;
        }
    return count;
}

int countReachableCells(Maze* maze)
{
    std::vector<std::vector<bool>> visited(maze->height, std::vector<bool>(maze->width, false));
    std::queue<Point> q;
    q.push({0, 0});
    visited[0][0] = true;
    while (!q.empty()) {
        Point cur = q.front(); q.pop();
        for (auto& next : maze->getNeighbors(cur.x, cur.y)) {
            if (!visited[next.y][next.x]) {
                visited[next.y][next.x] = true;
                q.push(next);
            }
        }
    }
    int reachable = 0;
    for (int y = 0; y < maze->height; ++y)
        for (int x = 0; x < maze->width; ++x)
            if (visited[y][x]) reachable++;
    return reachable;
}

// DFSGenerator

TEST(DFSGeneratorTest, CreatesCorrectSize)
{
    std::cout << "  Проверка: DFS создаёт лабиринт правильного размера\n";
    DFSGenerator gen;
    Maze* m = gen.generate(7, 7);
    EXPECT_EQ(m->width, 7);
    EXPECT_EQ(m->height, 7);
    delete m;
}

TEST(DFSGeneratorTest, HasOpenPassages)
{
    std::cout << "  Проверка: DFS открывает правильное число проходов\n";
    DFSGenerator gen;
    Maze* m = gen.generate(10, 10);
    EXPECT_EQ(countOpenPassages(m), 2 * (100 - 1));
    delete m;
}

TEST(DFSGeneratorTest, AllCellsReachable)
{
    std::cout << "  Проверка: DFS - все клетки достижимы\n";
    DFSGenerator gen;
    Maze* m = gen.generate(8, 8);
    EXPECT_EQ(countReachableCells(m), 64);
    delete m;
}

TEST(DFSGeneratorTest, GeneratesDifferentMazes)
{
    std::cout << "  Проверка: два запуска DFS дают разные лабиринты\n";
    DFSGenerator gen;
    Maze* a = gen.generate(5, 5);
    Maze* b = gen.generate(5, 5);
    bool diff = false;
    for (int y = 0; y < 5 && !diff; ++y)
        for (int x = 0; x < 5 && !diff; ++x)
            if (a->grid[y][x].topWall != b->grid[y][x].topWall ||
                a->grid[y][x].rightWall != b->grid[y][x].rightWall) diff = true;
    EXPECT_TRUE(diff);
    delete a; delete b;
}

TEST(DFSGeneratorTest, SingleCellMaze)
{
    std::cout << "  Проверка: DFS - лабиринт 1x1 (все стены на месте)\n";
    DFSGenerator gen;
    Maze* m = gen.generate(1, 1);
    EXPECT_EQ(countOpenPassages(m), 0);
    delete m;
}

TEST(DFSGeneratorTest, NarrowHorizontalMaze)
{
    std::cout << "  Проверка: DFS - узкий горизонтальный лабиринт 20x3\n";
    DFSGenerator gen;
    Maze* m = gen.generate(20, 3);
    EXPECT_EQ(countReachableCells(m), 60);
    delete m;
}

TEST(DFSGeneratorTest, NarrowVerticalMaze)
{
    std::cout << "  Проверка: DFS - узкий вертикальный лабиринт 3x20\n";
    DFSGenerator gen;
    Maze* m = gen.generate(3, 20);
    EXPECT_EQ(countReachableCells(m), 60);
    delete m;
}

// KruskalGenerator

TEST(KruskalGeneratorTest, CreatesCorrectSize)
{
    std::cout << "  Проверка: Kruskal создаёт лабиринт правильного размера\n";
    KruskalGenerator gen;
    Maze* m = gen.generate(9, 9);
    EXPECT_EQ(m->width, 9);
    EXPECT_EQ(m->height, 9);
    delete m;
}

TEST(KruskalGeneratorTest, HasOpenPassages)
{
    std::cout << "  Проверка: Kruskal открывает правильное число проходов\n";
    KruskalGenerator gen;
    Maze* m = gen.generate(10, 10);
    EXPECT_EQ(countOpenPassages(m), 2 * (100 - 1));
    delete m;
}

TEST(KruskalGeneratorTest, AllCellsReachable)
{
    std::cout << "  Проверка: Kruskal - все клетки достижимы\n";
    KruskalGenerator gen;
    Maze* m = gen.generate(8, 8);
    EXPECT_EQ(countReachableCells(m), 64);
    delete m;
}

TEST(KruskalGeneratorTest, GeneratesDifferentMazes)
{
    std::cout << "  Проверка: два запуска Kruskal дают разные лабиринты\n";
    KruskalGenerator gen;
    Maze* a = gen.generate(5, 5);
    Maze* b = gen.generate(5, 5);
    bool diff = false;
    for (int y = 0; y < 5 && !diff; ++y)
        for (int x = 0; x < 5 && !diff; ++x)
            if (a->grid[y][x].topWall != b->grid[y][x].topWall ||
                a->grid[y][x].rightWall != b->grid[y][x].rightWall) diff = true;
    EXPECT_TRUE(diff);
    delete a; delete b;
}

TEST(KruskalGeneratorTest, SingleCellMaze)
{
    std::cout << "  Проверка: Kruskal - лабиринт 1x1\n";
    KruskalGenerator gen;
    Maze* m = gen.generate(1, 1);
    EXPECT_EQ(countOpenPassages(m), 0);
    delete m;
}

TEST(KruskalGeneratorTest, LargeMaze)
{
    std::cout << "  Проверка: Kruskal - большой лабиринт 30x30\n";
    KruskalGenerator gen;
    Maze* m = gen.generate(30, 30);
    EXPECT_EQ(countReachableCells(m), 900);
    delete m;
}