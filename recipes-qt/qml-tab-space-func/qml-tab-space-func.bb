SUMMARY = "QT QML Recipe to navigate btw components"
LICENSE = "CLOSED"
PR = "Demo_for_TABSPACE_FUNCTIONALITY"

# SRCREV="${AUTOREV}"

SRC_URI = "file://CMakeLists.txt	     \
	   file://LocalSocketWrapperNew.h     \
	   file://LocalSocketWrapperNew.cpp       \
           file://TABmain.cpp              \
           file://TABMain.qml              "

DEPENDS += " qtbase \
             qtdeclarative-native \
             qtquick3d \
           "

S = "${WORKDIR}"

# Manually add this file, must be a better way!
do_install:append() {
    install -d ${D}${bindir}
    install -m 0644 ${S}/TABMain.qml ${D}${bindir}
}

inherit qt6-cmake
