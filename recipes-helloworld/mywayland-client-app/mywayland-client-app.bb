SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"
S = "${WORKDIR}/sources-unpack"

DEPENDS += "wayland"

SRC_URI += "file://init_window.c \
            file://init_window.h \
            file://log.h \
            file://xdg-shell.c \
            file://xdg-shell.h"

do_compile() {
    ${CC} ${LDFLAGS} ${CFLAGS} -o ${S}/color_change_egl ${S}/init_window.c ${S}/xdg-shell.c -I${S} -lwayland-client -lwayland-server -lwayland-egl -lEGL -lGLESv2 -lm
}
 
do_install() {
    install -d ${D}${bindir}
    install -m 0755 color_change_egl ${D}${bindir}
}
 
FILES_${PN} += "${bindir}/color_change_egl"
