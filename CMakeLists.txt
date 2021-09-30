# CMakeのバージョンを設定
cmake_minimum_required(VERSION 3.21)
# プロジェクト名と使用する言語を設定
project(httf2018-qual CXX)
# a.outという実行ファイルをmain.cppとhello.cppから作成
set(TARGET_NAME a.out)
add_executable(a.out main.cpp)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
# set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)

target_compile_options(${TARGET_NAME} PRIVATE "-O3;-Wall;-Wextra")

# Boost
set(boost_min_ver 1.76.0)
set(boost_libs system filesystem random)
find_package(Boost ${boost_min_ver})

if(Boost_FOUND)
    find_package(Boost ${boost_min_ver} COMPONENTS ${boost_libs})
endif()

target_link_libraries(${TARGET_NAME} ${Boost_LIBRARIES})
# target_link_libraries(${TARGET_NAME} ${boost_libs})
