# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/app_quick_controls_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/app_quick_controls_autogen.dir/ParseCache.txt"
  "app_quick_controls_autogen"
  )
endif()
