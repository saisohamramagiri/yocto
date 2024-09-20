#!/bin/sh

echo -e "Killing Weston...\n"
killall weston

echo -e "Setting environment variables for QtWayland...\n"
export WESTON_USER=weston
export LOGNAME=weston
export HOME=/home/weston
export QT_WAYLAND_SHELL_INTEGRATION=ivi-shell
mkdir -p /tmp/$USER-weston
chmod 0700 /tmp/$USER-weston
export XDG_RUNTIME_DIR=/tmp/$USER-weston
export QT_QPA_PLATFORM=wayland
export WAYLAND_DISPLAY=wayland-1
export QT_QPA_EGLFS_KMS_CONFIG=/usr/bin/eglfs.json
export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/plugins
 
 
export QT_XCB_GL_INTEGRATION=xcb_egl 
export QT_WAYLAND_CLIENT_BUFFER_INTEGRATION=wayland-egl
export QT_DEBUG_PLUGINS = 1

echo -e "Successful\!\n"

cd /usr/bin
