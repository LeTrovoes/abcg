project(synthwave)
add_executable(${PROJECT_NAME} main.cpp model.cpp ground.cpp sun.cpp pyramid.cpp rosa.cpp stars.cpp openglwindow.cpp)
enable_abcg(${PROJECT_NAME})
