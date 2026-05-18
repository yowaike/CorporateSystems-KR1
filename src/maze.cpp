#include "maze.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <queue>
#include <stack>

// Maze
Maze::Maze(int w, int h) : width(w), height(h) {
    grid.resize(height, std::vector<Cell>(width));
}

void Maze::removeWall(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (dx == 1)  { grid[y1][x1].rightWall = false; grid[y2][x2].leftWall = false; }
    if (dx == -1) { grid[y1][x1].leftWall = false;  grid[y2][x2].rightWall = false; }
    if (dy == 1)  { grid[y1][x1].bottomWall = false; grid[y2][x2].topWall = false; }
    if (dy == -1) { grid[y1][x1].topWall = false;    grid[y2][x2].bottomWall = false; }
}

bool Maze::isInBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

std::vector<Point> Maze::getNeighbors(int x, int y) const {
    std::vector<Point> result;
    if (!grid[y][x].topWall    && isInBounds(x, y - 1)) result.push_back({x, y - 1});
    if (!grid[y][x].rightWall  && isInBounds(x + 1, y)) result.push_back({x + 1, y});
    if (!grid[y][x].bottomWall && isInBounds(x, y + 1)) result.push_back({x, y + 1});
    if (!grid[y][x].leftWall   && isInBounds(x - 1, y)) result.push_back({x - 1, y});
    return result;
}

// DFS Generator
Maze* DFSGenerator::generate(int width, int height) {
    Maze* maze = new Maze(width, height);
    std::stack<Point> stack;
    std::mt19937 rng(std::random_device{}());

    maze->grid[0][0].visited = true;
    stack.push({0, 0});

    while (!stack.empty()) {
        Point current = stack.top();
        int x = current.x, y = current.y;

        std::vector<Point> unvisited;
        std::vector<std::pair<int,int>> dirs = {{0,-1},{1,0},{0,1},{-1,0}};
        for (auto [dx, dy] : dirs) {
            int nx = x + dx, ny = y + dy;
            if (maze->isInBounds(nx, ny) && !maze->grid[ny][nx].visited)
                unvisited.push_back({nx, ny});
        }

        if (unvisited.empty()) {
            stack.pop();
        } else {
            std::uniform_int_distribution<> dist(0, unvisited.size() - 1);
            Point next = unvisited[dist(rng)];
            maze->removeWall(x, y, next.x, next.y);
            maze->grid[next.y][next.x].visited = true;
            stack.push(next);
        }
    }
    return maze;
}

// DSU (для Kruskal)
DisjointSetUnion::DisjointSetUnion(int n) : parent(n), rank(n, 0) {
    for (int i = 0; i < n; i++) parent[i] = i;
}

int DisjointSetUnion::find(int x) {
    if (parent[x] != x) parent[x] = find(parent[x]);
    return parent[x];
}

void DisjointSetUnion::unite(int x, int y) {
    int rx = find(x), ry = find(y);
    if (rx == ry) return;
    if (rank[rx] < rank[ry]) parent[rx] = ry;
    else if (rank[rx] > rank[ry]) parent[ry] = rx;
    else { parent[ry] = rx; rank[rx]++; }
}

// Kruskal Generator
Maze* KruskalGenerator::generate(int width, int height) {
    Maze* maze = new Maze(width, height);
    std::mt19937 rng(std::random_device{}());
    int totalCells = width * height;
    DisjointSetUnion dsu(totalCells);

    struct Wall { int x1, y1, x2, y2; };
    std::vector<Wall> walls;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < width - 1)  walls.push_back({x, y, x + 1, y});
            if (y < height - 1) walls.push_back({x, y, x, y + 1});
        }
    }

    std::shuffle(walls.begin(), walls.end(),
        std::mt19937(std::random_device{}()));

    for (auto& w : walls) {
        int id1 = w.y1 * width + w.x1;
        int id2 = w.y2 * width + w.x2;
        if (dsu.find(id1) != dsu.find(id2)) {
            dsu.unite(id1, id2);
            maze->removeWall(w.x1, w.y1, w.x2, w.y2);
        }
    }
    return maze;
}

// BFS Finder
std::vector<Point> BFSFinder::findPath(Maze* maze, Point start, Point finish) {
    visitedCount = 0;
    int w = maze->width, h = maze->height;
    std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));
    std::vector<std::vector<Point>> parent(h, std::vector<Point>(w, {-1, -1}));
    std::queue<Point> q;

    q.push(start);
    visited[start.y][start.x] = true;
    visitedCount++;

    std::vector<std::pair<int,int>> dirs = {{0,-1},{1,0},{0,1},{-1,0}};
    while (!q.empty()) {
        Point cur = q.front(); q.pop();
        if (cur == finish) break;

        auto neighbors = maze->getNeighbors(cur.x, cur.y);
        for (auto& next : neighbors) {
            if (!visited[next.y][next.x]) {
                visited[next.y][next.x] = true;
                visitedCount++;
                parent[next.y][next.x] = cur;
                q.push(next);
            }
        }
    }

    std::vector<Point> path;
    Point cur = finish;
    while (!(cur == Point{-1, -1})) {
        path.push_back(cur);
        cur = parent[cur.y][cur.x];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

// A* Finder
int AStarFinder::heuristic(Point a, Point b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

struct AStarNode {
    Point pos;
    int fScore;
    bool operator>(const AStarNode& other) const { return fScore > other.fScore; }
};

std::vector<Point> AStarFinder::findPath(Maze* maze, Point start, Point finish) {
    visitedCount = 0;
    int w = maze->width, h = maze->height;
    std::vector<std::vector<int>> gScore(h, std::vector<int>(w, 999999));
    std::vector<std::vector<Point>> parent(h, std::vector<Point>(w, {-1, -1}));
    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> openSet;

    gScore[start.y][start.x] = 0;
    openSet.push({start, heuristic(start, finish)});

    while (!openSet.empty()) {
        Point cur = openSet.top().pos;
        openSet.pop();

        if (cur == finish) break;

        auto neighbors = maze->getNeighbors(cur.x, cur.y);
        for (auto& next : neighbors) {
            int tentativeG = gScore[cur.y][cur.x] + 1;
            if (tentativeG < gScore[next.y][next.x]) {
                visitedCount++;
                parent[next.y][next.x] = cur;
                gScore[next.y][next.x] = tentativeG;
                int fScore = tentativeG + heuristic(next, finish);
                openSet.push({next, fScore});
            }
        }
    }

    std::vector<Point> path;
    Point cur = finish;
    while (!(cur == Point{-1, -1})) {
        path.push_back(cur);
        cur = parent[cur.y][cur.x];
    }
    std::reverse(path.begin(), path.end());
    return path;
}