if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/cut-cpp-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package cut-cpp)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT cut-cpp_Development
)

install(
    TARGETS cut-cpp_cut-cpp
    EXPORT cut-cppTargets
    RUNTIME #
    COMPONENT cut-cpp_Runtime
    LIBRARY #
    COMPONENT cut-cpp_Runtime
    NAMELINK_COMPONENT cut-cpp_Development
    ARCHIVE #
    COMPONENT cut-cpp_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    cut-cpp_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE cut-cpp_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(cut-cpp_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${cut-cpp_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT cut-cpp_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${cut-cpp_INSTALL_CMAKEDIR}"
    COMPONENT cut-cpp_Development
)

install(
    EXPORT cut-cppTargets
    NAMESPACE cut-cpp::
    DESTINATION "${cut-cpp_INSTALL_CMAKEDIR}"
    COMPONENT cut-cpp_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
