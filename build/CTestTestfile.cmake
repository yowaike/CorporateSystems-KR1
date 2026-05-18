# CMake generated Testfile for 
# Source directory: C:/maze_generator
# Build directory: C:/maze_generator/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(UnitTests "C:/maze_generator/build/unit_tests.exe")
set_tests_properties(UnitTests PROPERTIES  _BACKTRACE_TRIPLES "C:/maze_generator/CMakeLists.txt;52;add_test;C:/maze_generator/CMakeLists.txt;0;")
add_test(ScenarioTests "C:/maze_generator/build/scenario_tests.exe")
set_tests_properties(ScenarioTests PROPERTIES  _BACKTRACE_TRIPLES "C:/maze_generator/CMakeLists.txt;66;add_test;C:/maze_generator/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
