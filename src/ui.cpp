#include "maze.h"
#include <iostream>

void MazeRenderer::display(Maze* maze) {
    int w = maze->width, h = maze->height;

    // Верхняя граница
    std::cout << "+";
    for (int x = 0; x < w; x++) std::cout << "--+";
    std::cout << "\n";

    for (int y = 0; y < h; y++) {
        // Левая стена + содержимое ячеек
        std::cout << "|";
        for (int x = 0; x < w; x++) {
            std::cout << "  ";
            if (maze->grid[y][x].rightWall) std::cout << "|";
            else std::cout << " ";
        }
        std::cout << "\n";

        // Нижние стены
        std::cout << "+";
        for (int x = 0; x < w; x++) {
            if (maze->grid[y][x].bottomWall) std::cout << "--+";
            else std::cout << "  +";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void MazeRenderer::displayWithPath(Maze* maze, const std::vector<Point>& path) {
    int w = maze->width, h = maze->height;
    std::vector<std::vector<bool>> onPath(h, std::vector<bool>(w, false));
    for (auto& p : path) onPath[p.y][p.x] = true;

    // Верхняя граница
    std::cout << "+";
    for (int x = 0; x < w; x++) std::cout << "--+";
    std::cout << "\n";

    for (int y = 0; y < h; y++) {
        std::cout << "|";
        for (int x = 0; x < w; x++) {
            if (x == 0 && y == 0)
                std::cout << "S ";
            else if (x == w-1 && y == h-1)
                std::cout << "F ";
            else if (onPath[y][x])
                std::cout << ". ";
            else
                std::cout << "  ";

            if (maze->grid[y][x].rightWall) std::cout << "|";
            else std::cout << " ";
        }
        std::cout << "\n+";
        for (int x = 0; x < w; x++) {
            if (maze->grid[y][x].bottomWall) std::cout << "--+";
            else std::cout << "  +";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}