#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "alate_interfaces::alate_interfaces__rosidl_generator_py" for configuration "Debug"
set_property(TARGET alate_interfaces::alate_interfaces__rosidl_generator_py APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(alate_interfaces::alate_interfaces__rosidl_generator_py PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libalate_interfaces__rosidl_generator_py.so"
  IMPORTED_SONAME_DEBUG "libalate_interfaces__rosidl_generator_py.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS alate_interfaces::alate_interfaces__rosidl_generator_py )
list(APPEND _IMPORT_CHECK_FILES_FOR_alate_interfaces::alate_interfaces__rosidl_generator_py "${_IMPORT_PREFIX}/lib/libalate_interfaces__rosidl_generator_py.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
