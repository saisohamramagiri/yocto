# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/QtIVI_compositor_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/QtIVI_compositor_autogen.dir/ParseCache.txt"
  "QtIVI_compositor_autogen"
  )
endif()
