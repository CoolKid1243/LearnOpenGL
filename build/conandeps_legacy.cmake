message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(glfw3)
find_package(glew)
find_package(imgui)
find_package(assimp)
find_package(glm)
find_package(stb)

set(CONANDEPS_LEGACY  glfw  GLEW::GLEW  imgui::imgui  assimp::assimp  glm::glm  stb::stb )