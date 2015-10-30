cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
project (CCTV)
find_package( OpenCV REQUIRED )

add_subdirectory( ext/curlcpp )
include_directories(${CURLCPP_SOURCE_DIR}/include)

add_executable( CCTV main.cpp axis.cpp )
target_link_libraries( CCTV ${OpenCV_LIBS} ${CURL_LIBRARIES} curlcpp )
