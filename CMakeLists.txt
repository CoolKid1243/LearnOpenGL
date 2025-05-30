cmake_minimum_required(VERSION 3.24)
project(LearnOpenGl)

# Find libraries
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)
find_package(stb REQUIRED)
find_package(imgui REQUIRED)
find_package(assimp REQUIRED)

# Add all .cpp files in the src directory and its subdirectories
file(GLOB_RECURSE CPP_SOURCES src/*.cpp)

# Add all .h files in the src directory and its subdirectories
file(GLOB_RECURSE HEADER_FILES src/*.h)

# Create the library target
add_executable(${PROJECT_NAME} src/main.cpp ${CPP_SOURCES} ${HEADER_FILES})

# Copy shaders
file(COPY src/shaders DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY src/textures DESTINATION ${CMAKE_BINARY_DIR}/)

# Link libraries to the target
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} stb::stb)
target_link_libraries(${PROJECT_NAME} imgui::imgui)
target_link_libraries(${PROJECT_NAME} assimp::assimp)
