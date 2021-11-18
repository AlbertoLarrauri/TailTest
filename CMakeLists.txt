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
        machines/incompatibility_data.cpp machines/incompatibility_data.h)


add_executable(main main.cpp utils.h)
# target_link_libraries(main PUBLIC SBCMin)