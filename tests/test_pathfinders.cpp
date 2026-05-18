/**
 * @file test_pathfinders.cpp
 * @brief Модульные тесты для алгоритмов поиска пути.
 */

#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "maze.h"

Maze* makeMaze(int w, int h, const std::string& algo)
{
    if (algo == "DFS") {
        DFSGenerator g;
        return g.generate(w, h);
    } else {
        KruskalGenerator g;
        return g.generate(w, h);
    }
}

void checkPath(Maze* maze, const std::vector<Point>& path, Point start, Point finish)
{
    ASSERT_GE(path.size(), 2);
    EXPECT_EQ(path.front().x, start.x);
    EXPECT_EQ(path.front().y, start.y);
    EXPECT_EQ(path.back().x, finish.x);
    EXPECT_EQ(path.back().y, finish.y);
    for (size_t i = 1; i < path.size(); ++i) {
        int dx = std::abs(path[i].x - path[i-1].x);
        int dy = std::abs(path[i].y - path[i-1].y);
        EXPECT_TRUE((dx == 1 && dy == 0) || (dx == 0 && dy == 1));
    }
}

// BFS

TEST(BFSFinderTest, FindsPathInDFSMaze)
{
    std::cout << "  Проверка: BFS находит путь в DFS-лабиринте 10x10\n";
    Maze* m = makeMaze(10, 10, "DFS");
    BFSFinder bfs;
    auto path = bfs.findPath(m, {0, 0}, {9, 9});
    checkPath(m, path, {0, 0}, {9, 9});
    delete m;
}

TEST(BFSFinderTest, FindsPathInKruskalMaze)
{
    std::cout << "  Проверка: BFS находит путь в Kruskal-лабиринте 10x10\n";
    Maze* m = makeMaze(10, 10, "Kruskal");
    BFSFinder bfs;
    auto path = bfs.findPath(m, {0, 0}, {9, 9});
    checkPath(m, path, {0, 0}, {9, 9});
    delete m;
}

TEST(BFSFinderTest, StartEqualsFinish)
{
    std::cout << "  Проверка: BFS - старт и финиш совпадают\n";
    Maze* m = makeMaze(5, 5, "DFS");
    BFSFinder bfs;
    auto path = bfs.findPath(m, {0, 0}, {0, 0});
    EXPECT_EQ(path.size(), 1);
    delete m;
}

TEST(BFSFinderTest, FindsShortestPath)
{
    std::cout << "  Проверка: BFS находит кратчайший путь в лабиринте 6x6\n";
    Maze* m = makeMaze(6, 6, "DFS");
    BFSFinder bfs;
    auto path = bfs.findPath(m, {0, 0}, {5, 5});
    EXPECT_GE(path.size(), 10);
    EXPECT_LE(path.size(), 36);
    delete m;
}

TEST(BFSFinderTest, SmallMaze)
{
    std::cout << "  Проверка: BFS работает в лабиринте 2x2\n";
    Maze* m = makeMaze(2, 2, "DFS");
    BFSFinder bfs;
    auto path = bfs.findPath(m, {0, 0}, {1, 1});
    EXPECT_GE(path.size(), 3);
    delete m;
}

TEST(BFSFinderTest, VisitedCountReasonable)
{
    std::cout << "  Проверка: BFS - счётчик посещённых клеток не превышает общее число\n";
    Maze* m = makeMaze(12, 12, "Kruskal");
    BFSFinder bfs;
    bfs.findPath(m, {0, 0}, {11, 11});
    EXPECT_LE(bfs.getVisitedCount(), 144);
    EXPECT_GT(bfs.getVisitedCount(), 0);
    delete m;
}

// A*

TEST(AStarFinderTest, FindsPathInDFSMaze)
{
    std::cout << "  Проверка: A* находит путь в DFS-лабиринте 10x10\n";
    Maze* m = makeMaze(10, 10, "DFS");
    AStarFinder astar;
    auto path = astar.findPath(m, {0, 0}, {9, 9});
    checkPath(m, path, {0, 0}, {9, 9});
    delete m;
}

TEST(AStarFinderTest, FindsPathInKruskalMaze)
{
    std::cout << "  Проверка: A* находит путь в Kruskal-лабиринте 10x10\n";
    Maze* m = makeMaze(10, 10, "Kruskal");
    AStarFinder astar;
    auto path = astar.findPath(m, {0, 0}, {9, 9});
    checkPath(m, path, {0, 0}, {9, 9});
    delete m;
}

TEST(AStarFinderTest, StartEqualsFinish)
{
    std::cout << "  Проверка: A* - старт и финиш совпадают\n";
    Maze* m = makeMaze(5, 5, "Kruskal");
    AStarFinder astar;
    auto path = astar.findPath(m, {2, 2}, {2, 2});
    EXPECT_EQ(path.size(), 1);
    delete m;
}

TEST(AStarFinderTest, SameLengthAsBFS)
{
    std::cout << "  Проверка: A* и BFS находят пути одинаковой длины\n";
    Maze* m = makeMaze(8, 8, "DFS");
    BFSFinder bfs;
    AStarFinder astar;
    auto p1 = bfs.findPath(m, {0, 0}, {7, 7});
    auto p2 = astar.findPath(m, {0, 0}, {7, 7});
    EXPECT_EQ(p1.size(), p2.size());
    delete m;
}

TEST(AStarFinderTest, VisitedCountNotExcessive)
{
    std::cout << "  Проверка: A* - счётчик посещённых клеток в пределах нормы\n";
    Maze* m = makeMaze(15, 15, "DFS");
    AStarFinder astar;
    astar.findPath(m, {0, 0}, {14, 14});
    EXPECT_LE(astar.getVisitedCount(), 225);
    EXPECT_GT(astar.getVisitedCount(), 0);
    delete m;
}

TEST(AStarFinderTest, LargeMaze)
{
    std::cout << "  Проверка: A* работает на большом лабиринте 25x25\n";
    Maze* m = makeMaze(25, 25, "Kruskal");
    AStarFinder astar;
    auto path = astar.findPath(m, {0, 0}, {24, 24});
    checkPath(m, path, {0, 0}, {24, 24});
    delete m;
}