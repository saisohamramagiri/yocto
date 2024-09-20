# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appqml-client2_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appqml-client2_autogen.dir/ParseCache.txt"
  "appqml-client2_autogen"
  )
endif()
