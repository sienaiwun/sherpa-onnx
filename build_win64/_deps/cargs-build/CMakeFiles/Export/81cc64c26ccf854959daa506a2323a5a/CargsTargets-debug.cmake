#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cargs" for configuration "Debug"
set_property(TARGET cargs APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(cargs PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/cargs.lib"
  )

list(APPEND _cmake_import_check_targets cargs )
list(APPEND _cmake_import_check_files_for_cargs "${_IMPORT_PREFIX}/lib/cargs.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
