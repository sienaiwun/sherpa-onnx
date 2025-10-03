#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "kissfft::kissfft-float" for configuration "MinSizeRel"
set_property(TARGET kissfft::kissfft-float APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(kissfft::kissfft-float PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_MINSIZEREL "C"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/kissfft-float.lib"
  )

list(APPEND _cmake_import_check_targets kissfft::kissfft-float )
list(APPEND _cmake_import_check_files_for_kissfft::kissfft-float "${_IMPORT_PREFIX}/lib/kissfft-float.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
