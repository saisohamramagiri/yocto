SUMMARY = "QT IVI Compositor"
LICENSE = "CLOSED"
PR = "QtCompositor"

# SRCREV="${AUTOREV}"

SRC_URI = "file://CMakeLists.txt	     \
           file://IVImain.cpp              \
           file://IVImain.qml              \
           file://LocalServer.h		    \
           file://LocalServer.cpp             \
           file://TcpServerHandler.h		\
           file://TcpServerHandler.cpp		\
           file://eglfs.json              "


EXTRA_OECMAKE += "-DCMAKE_FIND_DEBUG_MODE=TRUE, -DQT_DEBUG_FIND_PACKAGE=ON"

DEPENDS += " qtbase \
             qtdeclarative-native \
             qtquick3d \
           "

S = "${WORKDIR}"

# Manually add this file, must be a better way!
do_install:append() {
    install -d ${D}${bindir}
    install -m 0644 ${S}/IVImain.qml ${D}${bindir}
    install -m 0644 ${S}/eglfs.json ${D}${bindir}
}

inherit qt6-cmake
