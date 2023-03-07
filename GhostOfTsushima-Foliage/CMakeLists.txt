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
 "src/pch.h" "src/pch.cpp"
 "src/Application.h" "src/Application.cpp"
 "src/World/Chunk.h" "src/World/Chunk.cpp"
 "src/World/World.h" "src/World/World.cpp"
 "src/Renderer/Renderer.h" "src/Renderer/Renderer.cpp"
 "src/Util/Texture.h" "src/Util/Texture.cpp"
 "src/Util/Util.h"
 "src/Renderer/TerrainMesh.h" "src/Renderer/TerrainMesh.cpp"
 "src/World/Skybox.h" "src/World/Skybox.cpp"
 "src/Renderer/Scene.h"
 "src/Renderer/GrassMesh.h" "src/Renderer/GrassMesh.cpp"
 "src/Renderer/GrassSystem.h" "src/Renderer/GrassSystem.cpp"
)

#GLFW
add_subdirectory("vendor/glfw")
#GLM
add_subdirectory("vendor/glm")

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (${CMAKE_PROJECT_NAME} ${SRC_FILES} "vendor/glad/src/glad.c" "vendor/stb_image/stb_image.cpp" "vendor/stb_image/stb_image.h")

include_directories(${INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)

target_precompile_headers(${CMAKE_PROJECT_NAME} PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:pch.h>")

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(${CMAKE_PROJECT_NAME} copy_assets)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
