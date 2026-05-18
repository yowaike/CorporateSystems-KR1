cmake_minimum_required(VERSION 3.14)  # Для FetchContent
project(MazeGenerator LANGUAGES CXX)

# Стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Исходники основной программы
set(MAIN_SOURCES
    src/main.cpp
    src/maze.cpp
    src/ui.cpp
)

add_executable(MazeGenerator ${MAIN_SOURCES})
target_include_directories(MazeGenerator PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(MazeGenerator PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Включаем тестирование (CTest)
enable_testing()

# Модульные тесты (unit-тесты)
set(UNIT_TEST_SOURCES
    src/maze.cpp
    tests/test_main.cpp          
    tests/test_maze.cpp
    tests/test_generators.cpp
    tests/test_pathfinders.cpp
)

add_executable(unit_tests
    src/maze.cpp
    tests/test_main.cpp
    tests/test_maze.cpp
    tests/test_generators.cpp
    tests/test_pathfinders.cpp
)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(unit_tests PRIVATE gtest)

# Регистрируем юнит-тесты в CTest
add_test(NAME UnitTests COMMAND unit_tests)

# Тестовые сценарии (мини-программы)
set(SCENARIO_TEST_SOURCES
    src/maze.cpp
    src/ui.cpp
    tests/test_scenarios.cpp
)

add_executable(scenario_tests ${SCENARIO_TEST_SOURCES})
target_include_directories(scenario_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(scenario_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Регистрируем сценарии в CTest
add_test(NAME ScenarioTests COMMAND scenario_tests)