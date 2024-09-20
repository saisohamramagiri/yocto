inherit logging
SUMMARY = "Simple helloworld application"
SECTION = "examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://helloworld.c 	\
		   file://config.h		\
		   "

S = "${WORKDIR}"

TARGET_CC_ARCH += "${LDFLAGS}"

do_configure() {
	# Make clean required?
	echo "#define YOCTO_BUILD_VERSION \"${PR}\"" >> config.h
}

do_compile() {
    ${CC} helloworld.c -o helloworld
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 helloworld ${D}${bindir}
}
