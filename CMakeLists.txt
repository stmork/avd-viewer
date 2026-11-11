cmake_minimum_required(VERSION 3.16)
project(AVD-Viewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(JPEG REQUIRED)

add_executable(${PROJECT_NAME}
	main.cc
	yuvreader.cc yuvreader.h
	yuvwriter.cc yuvwriter.h
	yuvconverter.cc yuvconverter.h
)

add_executable(SyncSVG sync.cc)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE JPEG::JPEG)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

add_custom_target(astyle
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND astyle
	*.cc *.h
)

add_custom_target(cppcheck
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND cppcheck
	-D_DEBUG
	--inline-suppr
	--suppress=unusedStructMember
	--enable=style,warning,performance,portability
	--language=c++ --std=c++20
	--library=qt
	--xml-version=2 --force -q
	*.cc *.h
	2>cppcheck.xml
)

set_property(
	TARGET cppcheck
	APPEND
	PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_SOURCE_DIR}/cppcheck.xml
)
