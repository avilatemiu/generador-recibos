cmake_minimum_required(VERSION 3.16)

project(GeneradorRecibos VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

qt_standard_project_setup()

qt_add_executable(GeneradorRecibos
    src/main.cpp
    src/recibo.h
    src/recibo.cpp
    src/database.h
    src/database.cpp
    src/pdfgenerator.h
    src/pdfgenerator.cpp
)

target_link_libraries(GeneradorRecibos PRIVATE Qt6::Widgets Qt6::Sql)
