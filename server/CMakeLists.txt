project(server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig)
pkg_check_modules(thrift REQUIRED thrift)

add_executable(${PROJECT_NAME} main.cpp
                               gen-cpp/my_types.cpp
                               gen-cpp/helloSvc.cpp)

target_link_libraries(${PROJECT_NAME} ${thrift_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${thrift_INCLUDE_DIRS})