cmake_minimum_required(VERSION 3.16)

project(qml_demo VERSION 0.1 LANGUAGES CXX)

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


find_package(Qt6
    CONFIG
    REQUIRED
    COMPONENTS
        BuildInternals
        Quick
        Core
        Qml
        Gui
        QuickControls2
)

qt_add_executable(app_quick_controls
    main.cpp
)

qt_add_qml_module(app_quick_controls
    URI quick_controls
    VERSION 1.0
    QML_FILES Main2.qml
    SOURCES windowsan.h windowsan.cpp
)

set_target_properties(app_quick_controls PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(app_quick_controls
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
)

# install(TARGETS quick_controls
#  RUNTIME DESTINATION PROJECT_SOURCE_DIR
#  BUNDLE DESTINATION PROJECT_SOURCE_DIR
#  LIBRARY DESTINATION PROJECT_SOURCE_DIR
# )

install(TARGETS app_quick_controls
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
