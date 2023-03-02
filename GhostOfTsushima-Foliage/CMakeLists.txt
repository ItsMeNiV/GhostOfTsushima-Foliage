# CMakeList.txt: CMake-Projekt für "GhostOfTsushima-Foliage". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)


set(INCLUDE_DIRS
"src"
"vendor/GLFW/include"
"vendor/glad/include"
"vendor/glm"
"vendor/OpenGLStarter"
"vendor/stb_image"
)

set(SRC_FILES
"src/Application.cpp"
)

#GLFW
add_subdirectory("vendor/glfw")
#GLM
add_subdirectory("vendor/glm")

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (GhostOfTsushima-Foliage ${SRC_FILES} "vendor/glad/src/glad.c" "vendor/stb_image/stb_image.cpp" "vendor/stb_image/stb_image.h")

include_directories(${INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(${CMAKE_PROJECT_NAME} copy_assets)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GhostOfTsushima-Foliage PROPERTY CXX_STANDARD 20)
endif()

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
