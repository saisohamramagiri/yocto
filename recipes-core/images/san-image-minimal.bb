SUMMARY = "A very basic Wayland image with a terminal"

IMAGE_FEATURES += "splash package-management ssh-server-dropbear hwcodecs weston"

LICENSE = "MIT"

inherit core-image

CORE_IMAGE_BASE_INSTALL += "gtk+3-demo"
CORE_IMAGE_BASE_INSTALL += "${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'weston-xwayland matchbox-terminal', '', d)}"

QB_MEM = "-m 512"
IMAGE_INSTALL:append = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'qt3d-ptest', '', d)} \
    helloworld \
    qt-test \    
    qml-demo \
    qml-demo2 \
    qml-demo3 \
    qml-brightness-socket \
    qml-tab-space-func \
    qml-client2		\
    quick-controls \
    ivi-compositor \
    myenv    \
"
