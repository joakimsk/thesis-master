cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_FLAGS "-std=c++11")
project (CCTV)
find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED )

add_subdirectory( ext/curlcpp )
include_directories(${CURLCPP_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS})


add_executable( CCTV main.cpp axis.cpp cheap_functions.cpp )
target_link_libraries( CCTV ${OpenCV_LIBS} ${CURL_LIBRARIES} ${Boost_LIBRARIES} curlcpp )
