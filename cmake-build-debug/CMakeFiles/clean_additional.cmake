# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/runner_bashrunner_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/runner_bashrunner_autogen.dir/ParseCache.txt"
  "runner_bashrunner_autogen"
  )
endif()
