# CMAKE VERSION
cmake_minimum_required(VERSION 3.5)

# PROJECT
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
set(PROJECT_NAME "bayan" CACHE STRING "Project name")
project(${PROJECT_NAME} VERSION ${PROJECT_VESRION})

# SOURCES
set (
    SOURCES
        main.cpp
)

# HEADERS
 set (
     HEADERS
#        matrix.h
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

# Compiler options
if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
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
