SUMMARY = "QT QML Test Recipe"
LICENSE = "CLOSED"
PR = "Demo_2"

# SRCREV="${AUTOREV}"

SRC_URI = "file://CMakeLists.txt	     \
           file://main2.cpp              \
           file://Main_2.qml              "

DEPENDS += " qtbase \
             qtdeclarative-native \
             qtquick3d \
           "

S = "${WORKDIR}"

# Manually add this file, must be a better way!
do_install:append() {
    install -d ${D}${bindir}
    install -m 0644 ${S}/Main_2.qml ${D}${bindir}
}

inherit qt6-cmake
