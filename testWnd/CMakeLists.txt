cmake_minimum_required(VERSION 3.18)
project(testWnd)

set(GLFW_USE_WAYLAND ON)

add_subdirectory(glfw EXCLUDE_FROM_ALL)
add_subdirectory(glad EXCLUDE_FROM_ALL)

# X11 Client
add_executable(testWnd_x11 ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_include_directories(testWnd_x11 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
target_sources(testWnd_x11 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(testWnd_x11 PRIVATE glad_static)
target_link_libraries(testWnd_x11 PRIVATE glfw ${GLFW_LIBRARIES})
target_link_libraries(testWnd_x11 PRIVATE IngameIME_Static)
target_compile_definitions(testWnd_x11 PRIVATE DISPLAY_SERVER_X11)

# Wayland Client
add_executable(testWnd_wl ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_include_directories(testWnd_wl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
target_sources(testWnd_wl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(testWnd_wl PRIVATE glad_static)
target_link_libraries(testWnd_wl PRIVATE glfw ${GLFW_LIBRARIES})
target_link_libraries(testWnd_wl PRIVATE IngameIME_Static)


# Install
install(TARGETS testWnd_x11 testWnd_wl)
