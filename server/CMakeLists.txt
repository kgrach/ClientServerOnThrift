project(server)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig)
pkg_check_modules(thrift REQUIRED thrift_c_glib)

add_executable(${PROJECT_NAME} main.c
                               ../gen-c_glib/my_types.c
                               ../gen-c_glib/hello_svc.c)

target_link_libraries(${PROJECT_NAME} ${thrift_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${thrift_INCLUDE_DIRS})