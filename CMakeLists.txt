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
find_package(Boost COMPONENTS program_options filesystem regex unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# SOURCES
set (
    SOURCES
        hash.cpp
        filesfilter.cpp
)

# HEADERS
set (
    HEADERS
        hash.h
        filesfilter.h
)

# EXECUTABLE
add_executable(bayan main.cpp ${SOURCES} ${HEADERS})
add_executable(test_hash test_hash.cpp hash.cpp)
add_executable(test_samefilesfinder test_samefilesfinder.cpp ${SOURCES} ${HEADERS})

# PROPERTIES
set_target_properties (
    bayan
    test_hash
    test_samefilesfinder
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
target_link_libraries(
    test_hash
        ${Boost_LIBRARIES}
        )
target_link_libraries(
    test_samefilesfinder
        ${Boost_LIBRARIES}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_REGEX_LIBRARY}
        )


# Compiler options
if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
    target_compile_options(test_hash PRIVATE
        /W4
    )
    target_compile_options(test_samefilesfinder PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_hash PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_samefilesfinder PRIVATE
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
enable_testing()
add_test(test_hash test_samefilesfinder)
add_test(test_samefilesfinder test_samefilesfinder)
