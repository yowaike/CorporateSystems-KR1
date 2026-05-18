#pragma once
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <random>
#include <chrono>

// Координаты
struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Ячейка лабиринта
struct Cell {
    bool topWall    = true;
    bool rightWall  = true;
    bool bottomWall = true;
    bool leftWall   = true;
    bool visited    = false;
};

// Лабиринт
class Maze {
public:
    int width, height;
    std::vector<std::vector<Cell>> grid;

    Maze(int w, int h);
    void removeWall(int x1, int y1, int x2, int y2);
    bool isInBounds(int x, int y) const;
    std::vector<Point> getNeighbors(int x, int y) const;
};

// Интерфейс генератора (паттерн Стратегия)
class IMazeGenerator {
public:
    virtual Maze* generate(int width, int height) = 0;
    virtual ~IMazeGenerator() = default;
};

// DFS-генератор
class DFSGenerator : public IMazeGenerator {
public:
    Maze* generate(int width, int height) override;
};

// Kruskal-генератор
class DisjointSetUnion {
    std::vector<int> parent, rank;
public:
    DisjointSetUnion(int n);
    int find(int x);
    void unite(int x, int y);
};

class KruskalGenerator : public IMazeGenerator {
public:
    Maze* generate(int width, int height) override;
};

// Интерфейс поиска пути (паттерн Стратегия)
class IPathFinder {
public:
    virtual std::vector<Point> findPath(Maze* maze, Point start, Point finish) = 0;
    virtual int getVisitedCount() const = 0;
    virtual ~IPathFinder() = default;
};

// BFS-поиск
class BFSFinder : public IPathFinder {
    int visitedCount = 0;
public:
    std::vector<Point> findPath(Maze* maze, Point start, Point finish) override;
    int getVisitedCount() const override { return visitedCount; }
};

// A*-поиск
class AStarFinder : public IPathFinder {
    int visitedCount = 0;
    int heuristic(Point a, Point b);
public:
    std::vector<Point> findPath(Maze* maze, Point start, Point finish) override;
    int getVisitedCount() const override { return visitedCount; }
};

// Фабрики (паттерн Фабричный метод)
class MazeGeneratorFactory {
public:
    virtual IMazeGenerator* createGenerator() = 0;
    virtual ~MazeGeneratorFactory() = default;
};

class DFSGeneratorFactory : public MazeGeneratorFactory {
public:
    IMazeGenerator* createGenerator() override { return new DFSGenerator(); }
};

class KruskalGeneratorFactory : public MazeGeneratorFactory {
public:
    IMazeGenerator* createGenerator() override { return new KruskalGenerator(); }
};

class PathFinderFactory {
public:
    virtual IPathFinder* createFinder() = 0;
    virtual ~PathFinderFactory() = default;
};

class BFSFinderFactory : public PathFinderFactory {
public:
    IPathFinder* createFinder() override { return new BFSFinder(); }
};

class AStarFinderFactory : public PathFinderFactory {
public:
    IPathFinder* createFinder() override { return new AStarFinder(); }
};

// Статистика
struct PathStats {
    double genTimeMs;
    double searchTimeMs;
    int visitedCells;
    int pathLength;
    std::string algorithmName;
};

// Отрисовка (вынесена в отдельный модуль)
class MazeRenderer {
public:
    void display(Maze* maze);
    void displayWithPath(Maze* maze, const std::vector<Point>& path);
};