# CMAKE VERSION
cmake_minimum_required(VERSION 3.5)

# PROJECT
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
set(PROJECT_NAME "bayan" CACHE STRING "Project name")
project(${PROJECT_NAME} VERSION ${PROJECT_VESRION})

# Boost
# NOTES:
# 1. Installing BOOST for Windows:
# https://gist.github.com/zrsmithson/0b72e0cb58d0cb946fc48b5c88511da8
# 2. CMAKE compatibility: set system environment variables
# BOOST_ROOT          D:\boost
# BOOST_INCLUDEDIR    D:\boost\include\boost-1_78
# BOOST_LIBRARYDIR    D:\boost\lib
# https://stackoverflow.com/questions/20969280/find-package-doesnt-detect-boost-on-windows-cmake
find_package(Boost COMPONENTS program_options filesystem regex REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# SOURCES
set (
    SOURCES
        hash.cpp
        filesfilter.cpp
        main.cpp
)

# HEADERS
set (
    HEADERS
        hash.h
        filesfilter.h
)

# EXECUTABLE
add_executable(bayan ${SOURCES} ${HEADERS})

# PROPERTIES
set_target_properties (
    bayan
        PROPERTIES
            CMAKE_CXX_STANDARD 17
            CMAKE_CXX_STANDARD_REQUIRED ON
)

# INCLUDEPATH

# Linking
target_link_libraries(
    bayan
        ${Boost_LIBRARIES}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_REGEX_LIBRARY}
        )


# Compiler options
if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic # -Werror
    )
endif()

### Package
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

### UNIT-tests
