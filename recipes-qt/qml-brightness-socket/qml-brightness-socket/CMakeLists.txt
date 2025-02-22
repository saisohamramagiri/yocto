cmake_minimum_required(VERSION 3.16)

project(qml-brightness-socket VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appqml-brightness-socket
    Brightnessmain.cpp
)

qt_add_qml_module(appqml-brightness-socket
    URI qml-brightness-socket
    VERSION 1.0
    QML_FILES
    BrightnessMain.qml
        SOURCES localsocketwrapper.h localsocketwrapper.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appqml-brightness-socket PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appqml-brightness-socket
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqml-brightness-socket
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appqml-brightness-socket
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
