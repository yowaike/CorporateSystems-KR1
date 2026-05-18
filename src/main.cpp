#include "maze.h"
#include <iostream>
#include <chrono>

int main() {
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    MazeGeneratorFactory* genFactory = nullptr;
    PathFinderFactory* finderFactory = nullptr;
    IMazeGenerator* generator = nullptr;
    IPathFinder* finder = nullptr;
    Maze* maze = nullptr;
    MazeRenderer renderer;

    int choice, width, height;

    std::cout << "Генератор лабиринтов и поиск пути\n\n";

    // Выбор алгоритма генерации
    std::cout << "Выберите алгоритм генерации:\n";
    std::cout << "1. DFS (обход в глубину)\n";
    std::cout << "2. Kruskal (алгоритм Крускала)\n> ";
    std::cin >> choice;

    if (choice == 1) genFactory = new DFSGeneratorFactory();
    else genFactory = new KruskalGeneratorFactory();
    generator = genFactory->createGenerator();
    delete genFactory;

    // Размер лабиринта
    std::cout << "Введите размеры (ширина высота): ";
    std::cin >> width >> height;

    // Генерация
    auto t1 = std::chrono::high_resolution_clock::now();
    maze = generator->generate(width, height);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto genMs = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "\nСгенерированный лабиринт:\n";
    renderer.display(maze);
    std::cout << "Время генерации: " << genMs << " мс\n\n";

    // Выбор алгоритма поиска
    std::cout << "Выберите алгоритм поиска пути:\n";
    std::cout << "1. BFS (обход в ширину)\n";
    std::cout << "2. A* (Манхэттен)\n> ";
    std::cin >> choice;

    if (choice == 1) finderFactory = new BFSFinderFactory();
    else finderFactory = new AStarFinderFactory();
    finder = finderFactory->createFinder();
    delete finderFactory;

    // Поиск пути
    t1 = std::chrono::high_resolution_clock::now();
    auto path = finder->findPath(maze, {0, 0}, {width - 1, height - 1});
    t2 = std::chrono::high_resolution_clock::now();
    auto searchMs = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "\nЛабиринт с путём:\n";
    renderer.displayWithPath(maze, path);
    std::cout << "Время поиска: " << searchMs << " мс\n";
    std::cout << "Посещено ячеек: " << finder->getVisitedCount() << "\n";
    std::cout << "Длина пути: " << path.size() << "\n";

    delete generator;
    delete finder;
    delete maze;

    return 0;
}