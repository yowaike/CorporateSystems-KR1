/**
 * @file test_maze.cpp
 * @brief Модульные тесты для класса Maze и структуры Cell.
 */

#include <gtest/gtest.h>
#include <iostream>
#include "maze.h"

// Тесты создания лабиринта
TEST(MazeCreation, CreateSmallMaze)
{
    std::cout << "  Проверка: создание маленького лабиринта 5x5\n";
    Maze labyrinth(5, 5);
    EXPECT_EQ(labyrinth.width, 5);
    EXPECT_EQ(labyrinth.height, 5);
    EXPECT_EQ(labyrinth.grid.size(), 5);
    EXPECT_EQ(labyrinth.grid[0].size(), 5);
}

TEST(MazeCreation, CreateRectangularMaze)
{
    std::cout << "  Проверка: создание прямоугольного лабиринта 10x3\n";
    Maze labyrinth(10, 3);
    EXPECT_EQ(labyrinth.width, 10);
    EXPECT_EQ(labyrinth.height, 3);
    EXPECT_EQ(labyrinth.grid.size(), 3);
    EXPECT_EQ(labyrinth.grid[0].size(), 10);
}

TEST(MazeCreation, CreateSingleCellMaze)
{
    std::cout << "  Проверка: создание лабиринта из одной клетки 1x1\n";
    Maze labyrinth(1, 1);
    EXPECT_EQ(labyrinth.width, 1);
    EXPECT_EQ(labyrinth.height, 1);
}

TEST(MazeCreation, CreateLargeMaze)
{
    std::cout << "  Проверка: создание большого лабиринта 50x50\n";
    Maze labyrinth(50, 50);
    EXPECT_EQ(labyrinth.width, 50);
    EXPECT_EQ(labyrinth.height, 50);
}

TEST(MazeCreation, CreateVeryNarrowMaze)
{
    std::cout << "  Проверка: создание узкого лабиринта 1x20\n";
    Maze labyrinth(1, 20);
    EXPECT_EQ(labyrinth.width, 1);
    EXPECT_EQ(labyrinth.height, 20);
}

// Тесты начального состояния ячеек
TEST(CellInitialState, AllWallsExistByDefault)
{
    std::cout << "  Проверка: у всех ячеек все 4 стены целы\n";
    Maze labyrinth(3, 3);
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            Cell& cell = labyrinth.grid[y][x];
            EXPECT_TRUE(cell.topWall);
            EXPECT_TRUE(cell.rightWall);
            EXPECT_TRUE(cell.bottomWall);
            EXPECT_TRUE(cell.leftWall);
        }
    }
}

TEST(CellInitialState, AllCellsUnvisitedByDefault)
{
    std::cout << "  Проверка: ни одна ячейка не помечена как посещённая\n";
    Maze labyrinth(4, 4);
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 4; ++x)
            EXPECT_FALSE(labyrinth.grid[y][x].visited);
}

// Тесты удаления стен
TEST(WallRemoval, RemoveVerticalWall)
{
    std::cout << "  Проверка: удаление вертикальной стены между (0,0) и (1,0)\n";
    Maze labyrinth(3, 3);
    labyrinth.removeWall(0, 0, 1, 0);
    EXPECT_FALSE(labyrinth.grid[0][0].rightWall);
    EXPECT_FALSE(labyrinth.grid[0][1].leftWall);
}

TEST(WallRemoval, RemoveHorizontalWall)
{
    std::cout << "  Проверка: удаление горизонтальной стены между (0,0) и (0,1)\n";
    Maze labyrinth(3, 3);
    labyrinth.removeWall(0, 0, 0, 1);
    EXPECT_FALSE(labyrinth.grid[0][0].bottomWall);
    EXPECT_FALSE(labyrinth.grid[1][0].topWall);
}

TEST(WallRemoval, RemoveWallDoesNotAffectUnrelatedCells)
{
    std::cout << "  Проверка: удаление стены не ломает другие клетки\n";
    Maze labyrinth(5, 5);
    labyrinth.removeWall(2, 2, 3, 2);
    EXPECT_TRUE(labyrinth.grid[0][0].rightWall);
    EXPECT_TRUE(labyrinth.grid[4][4].leftWall);
}

// Тесты проверки границ
TEST(BoundsCheck, InsideBounds)
{
    std::cout << "  Проверка: координаты внутри лабиринта - true\n";
    Maze labyrinth(5, 5);
    EXPECT_TRUE(labyrinth.isInBounds(0, 0));
    EXPECT_TRUE(labyrinth.isInBounds(4, 4));
    EXPECT_TRUE(labyrinth.isInBounds(2, 3));
}

TEST(BoundsCheck, OutsideBounds)
{
    std::cout << "  Проверка: координаты снаружи лабиринта - false\n";
    Maze labyrinth(5, 5);
    EXPECT_FALSE(labyrinth.isInBounds(-1, 0));
    EXPECT_FALSE(labyrinth.isInBounds(0, -1));
    EXPECT_FALSE(labyrinth.isInBounds(5, 0));
    EXPECT_FALSE(labyrinth.isInBounds(100, 100));
}

// Тесты получения соседей
TEST(Neighbors, NoNeighborsWhenAllWallsIntact)
{
    std::cout << "  Проверка: все стены целы - соседей нет\n";
    Maze labyrinth(3, 3);
    std::vector<Point> neighbors = labyrinth.getNeighbors(1, 1);
    EXPECT_TRUE(neighbors.empty());
}

TEST(Neighbors, HasNeighborAfterWallRemoval)
{
    std::cout << "  Проверка: убрали стену - появился сосед\n";
    Maze labyrinth(3, 3);
    labyrinth.removeWall(0, 0, 1, 0);
    std::vector<Point> neighbors = labyrinth.getNeighbors(0, 0);
    EXPECT_EQ(neighbors.size(), 1);
    EXPECT_EQ(neighbors[0].x, 1);
    EXPECT_EQ(neighbors[0].y, 0);
}

TEST(Neighbors, CornerCellHasMaxTwoNeighbors)
{
    std::cout << "  Проверка: угловая клетка имеет максимум 2 соседа\n";
    Maze labyrinth(3, 3);
    labyrinth.removeWall(0, 0, 1, 0);
    labyrinth.removeWall(0, 0, 0, 1);
    std::vector<Point> neighbors = labyrinth.getNeighbors(0, 0);
    EXPECT_EQ(neighbors.size(), 2);
}