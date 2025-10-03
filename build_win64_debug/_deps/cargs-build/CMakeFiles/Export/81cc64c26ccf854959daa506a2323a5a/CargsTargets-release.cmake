#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cargs" for configuration "Release"
set_property(TARGET cargs APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(cargs PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/cargs.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/cargs.dll"
  )

list(APPEND _cmake_import_check_targets cargs )
list(APPEND _cmake_import_check_files_for_cargs "${_IMPORT_PREFIX}/lib/cargs.lib" "${_IMPORT_PREFIX}/bin/cargs.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
