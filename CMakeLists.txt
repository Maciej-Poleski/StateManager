project(StateManager)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-std=c++11")
find_package(Boost REQUIRED filesystem regex)
add_definitions(-DBOOST_FILESYSTEM_DYN_LINK)

set(StateManager_SRCS main.cpp)

add_subdirectory(src)

add_executable(StateManager ${StateManager_SRCS})
target_link_libraries(StateManager core ${Boost_LIBRARIES})
install(TARGETS StateManager RUNTIME DESTINATION bin)
