cmake_minimum_required(VERSION 3.16)
project(biosim_client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Quick)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


add_executable(biosim_client
    main.cpp
)

qt5_add_resources(biosim_client_QRC "qml.qrc")
target_sources(biosim_client PRIVATE ${biosim_client_QRC})

target_link_libraries(biosim_client Qt5::Core Qt5::Quick)

target_compile_definitions(biosim_client PRIVATE QT_QML_DEBUG)