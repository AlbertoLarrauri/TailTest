cmake_minimum_required(VERSION 3.19)
project(TailTest)

set(CMAKE_CXX_STANDARD 17)


add_library( TailTest SHARED
        machines/machines.cpp
        machines/machines.h
        machines/builders.h
        machines/builders.cpp
        machines/serializer.cpp
        machines/serializer.h
        machines/incompatibility_data.cpp machines/incompatibility_data.h machines/input_tree.cpp machines/input_tree.h machines/quotient.cpp machines/quotient.h testers/basic.cpp testers/basic.h)


add_executable(main main.cpp utils.h machines/input_tree.h machines/input_tree.cpp
        machines/machines.h machines/machines.cpp
        machines/incompatibility_data.h machines/incompatibility_data.cpp
        )
# target_link_libraries(main PUBLIC SBCMin)