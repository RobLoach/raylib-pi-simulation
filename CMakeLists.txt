cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(raylib-pi-simulation
    VERSION 0.0.1
    DESCRIPTION "raylib-pi-simulation"
    HOMEPAGE_URL "https://github.com/robloach/raylib-pi-simulation"
    LANGUAGES C
)

# Register the cmake folder for find_package()
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
find_package(raylib QUIET)

# Our Project

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.c)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME}
    raylib
)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

# Web Configurations
if (EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

# Copy the resources
file(GLOB resources resources/*)
set(test_resources)
list(APPEND test_resources ${resources})
file(COPY ${test_resources} DESTINATION "resources/")