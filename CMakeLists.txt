cmake_minimum_required(VERSION 3.19)
project(bellerophon LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "/opt/Qt/6.11.0/gcc_64")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Svg SvgWidgets)

qt_standard_project_setup()

qt_add_executable(bellerophon
        WIN32 MACOSX_BUNDLE
        src/main.cpp
        src/mainwindow.h
        src/mainwindow.cpp
        ui/mainwindow.ui
        src/hexengine.h
        src/hexengine.cpp
        src/hexboard.h
        src/hexboard.cpp
        pieces.qrc
)
target_include_directories(bellerophon PRIVATE src ui)

target_link_libraries(bellerophon
        PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Svg
        Qt::SvgWidgets
)

install(TARGETS bellerophon
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
        TARGET bellerophon
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
