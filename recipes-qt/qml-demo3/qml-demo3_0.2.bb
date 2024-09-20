SUMMARY = "QT QML Test Recipe"
LICENSE = "CLOSED"
PR = "Spedo"

# SRCREV="${AUTOREV}"

SRC_URI = "file://CMakeLists.txt	     \
           file://main.cpp              \
           file://speedo.qml              "

DEPENDS += " qtbase \
             qtdeclarative-native \
             qtquick3d \
           "

S = "${WORKDIR}"

# Manually add this file, must be a better way!
do_install:append() {
    install -d ${D}${bindir}
    install -m 0644 ${S}/speedo.qml ${D}${bindir}
}

inherit qt6-cmake
