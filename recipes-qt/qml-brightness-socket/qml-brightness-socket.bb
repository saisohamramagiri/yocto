SUMMARY = "QT QML Recipe to adjust brightness"
LICENSE = "CLOSED"
PR = "Demo_for_Dimming_Layer"

# SRCREV="${AUTOREV}"

SRC_URI = "file://CMakeLists.txt	     \
	   file://localsocketwrapper.cpp     \
	   file://localsocketwrapper.h       \
           file://Brightnessmain.cpp              \
           file://BrightnessMain.qml              "

DEPENDS += " qtbase \
             qtdeclarative-native \
             qtquick3d \
           "

S = "${WORKDIR}"

# Manually add this file, must be a better way!
do_install:append() {
    install -d ${D}${bindir}
    install -m 0644 ${S}/BrightnessMain.qml ${D}${bindir}
}

inherit qt6-cmake
