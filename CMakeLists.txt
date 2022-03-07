cmake_minimum_required(VERSION 3.18)
project(IngameIME-Linux)

set(CMAKE_CXX_STANDARD 17)

# ECM - Extra CMake Modules
find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_FIND_MODULE_DIR})

# X11
find_package(X11 REQUIRED)

# Wayland
find_package(Wayland REQUIRED COMPONENTS Client)
find_package(WaylandProtocols REQUIRED)
find_package(WaylandScanner REQUIRED)
# Wayland Text Input
ecm_add_wayland_client_protocol(source_file
                                PROTOCOL "${WaylandProtocols_DATADIR}/unstable/text-input/text-input-unstable-v3.xml"
                                BASENAME text-input)

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

include(GetGitRevisionDescription)
git_describe(VERSION --tags --dirty=-dirty)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in
                ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
set(version_file ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
message(STATUS "Build version: ${VERSION}")

include(IngameIME-Common/IngameIME.cmake)

set(include_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/wl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/x11
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${X11_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${WAYLAND_INCLUDE_DIRS})
set(source_file ${CMAKE_CURRENT_SOURCE_DIR}/src/IngameIMEImpl.cpp ${version_file} ${source_file})

IngameIME_add_library(IngameIME_Static STATIC ${source_file})
IngameIME_add_library(IngameIME_Shared SHARED ${source_file})
target_include_directories(IngameIME_Static PRIVATE "${include_dir}")
target_include_directories(IngameIME_Shared PRIVATE "${include_dir}")
target_link_libraries(IngameIME_Static PRIVATE ${X11_LIBRARIES} ${Wayland_LIBRARIES})
target_link_libraries(IngameIME_Shared PRIVATE ${X11_LIBRARIES} ${Wayland_LIBRARIES})

# Test Window
add_subdirectory(testWnd)

# Install
install(TARGETS IngameIME_Static IngameIME_Shared testWnd_x11 testWnd_wl PUBLIC_HEADER)

# Pack
set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${CMAKE_GENERATOR_PLATFORM}-${VERSION})
set(CPACK_PACKAGE_VENDOR "Windmill_City")
set(CPACK_PACKAGE_CHECKSUM "SHA1")
set(CPACK_GENERATOR "ZIP")

set(CPACK_PACKAGE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pack)

include(CPack)