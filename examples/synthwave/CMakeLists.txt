project(synthwave)
add_executable(
  ${PROJECT_NAME} main.cpp model.cpp ground.cpp sun.cpp gradient.cpp stars.cpp
  openglwindow.cpp
)
enable_abcg(${PROJECT_NAME})
