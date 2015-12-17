cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_FLAGS "-std=c++11")
project (APP_CCTV)
find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED )

add_subdirectory( ext/curlcpp )
include_directories(${CURLCPP_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS})

add_executable( APP_CCTV main.cpp camera.cpp cheap_functions.cpp )
add_executable( APP_TIMER timer_prog.cpp )
add_executable( APP_CVEXAMPLE ocv_example_main.cpp )
add_executable( APP_BLOBTEST blobparamtest.cpp )

target_link_libraries( APP_CCTV ${OpenCV_LIBS} ${CURL_LIBRARIES} ${Boost_LIBRARIES} curlcpp )
target_link_libraries( APP_TIMER ${OpenCV_LIBS} )
target_link_libraries( APP_CVEXAMPLE ${OpenCV_LIBS} )
target_link_libraries( APP_BLOBTEST ${OpenCV_LIBS} )

