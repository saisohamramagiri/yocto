cmake_minimum_required(VERSION 3.16)

project(qml-tab-space-func VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appqml-tab-space-func
    TABmain.cpp
)

qt_add_qml_module(appqml-tab-space-func
    URI qml-tab-space-func
    VERSION 1.0
    QML_FILES
    TABMain.qml
    SOURCES LocalSocketWrapperNew.h LocalSocketWrapperNew.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appqml-tab-space-func PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appqml-tab-space-func
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqml-tab-space-func
    PRIVATE Qt6::Quick Qt6::Core Qt6::Network
)

include(GNUInstallDirs)
install(TARGETS appqml-tab-space-func
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
