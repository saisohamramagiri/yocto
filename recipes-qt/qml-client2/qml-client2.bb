SUMMARY = "QT QML Recipe to navigate btw components in client 2"
LICENSE = "CLOSED"
PR = "Demo_for_Navigation_in_Client2"

# SRCREV="${AUTOREV}"

SRC_URI = "file://CMakeLists.txt	     \
	   file://client2localsocketwrapper.h     \
	   file://client2localsocketwrapper.cpp       \
           file://Client2main.cpp              \
           file://Client2Main.qml              "

DEPENDS += " qtbase \
             qtdeclarative-native \
             qtquick3d \
           "

S = "${WORKDIR}"

# Manually add this file, must be a better way!
do_install:append() {
    install -d ${D}${bindir}
    install -m 0644 ${S}/Client2Main.qml ${D}${bindir}
}

inherit qt6-cmake
