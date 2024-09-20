cmake_minimum_required(VERSION 3.16)

project(app_qml_demo2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CMakePrintHelpers)

set(PROJECT_VERSION_MAJOR 6)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION 6.2.4)

cmake_print_variables(PROJECT_SOURCE_DIR)
cmake_print_variables(PROJECT_VERSION_MAJOR)
# Should output WORKDIR
cmake_print_variables(${S})

# install(TARGETS qml_demo
#  RUNTIME DESTINATION PROJECT_SOURCE_DIR
#  BUNDLE DESTINATION PROJECT_SOURCE_DIR
#  LIBRARY DESTINATION PROJECT_SOURCE_DIR
# )

find_package(Qt6
    CONFIG
    REQUIRED
    COMPONENTS
        BuildInternals
        Quick
        Core
        Qml
        Gui
)

qt_add_executable(app_qml_demo2
    main2.cpp
)

qt_add_qml_module(app_qml_demo2
    URI qml_demo
    VERSION 1.0
    QML_FILES Main_2.qml
)

set_target_properties(app_qml_demo2 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(app_qml_demo2
    PRIVATE Qt6::Quick
)

install(TARGETS app_qml_demo2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
