# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appqml-tab-space-func_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appqml-tab-space-func_autogen.dir/ParseCache.txt"
  "appqml-tab-space-func_autogen"
  )
endif()
