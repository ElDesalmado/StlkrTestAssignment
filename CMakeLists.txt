cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(TestAssignment VERSION 0.0.1  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

add_library(Container 
	STATIC
		include/Container.h
		src/Container.cpp
	)
	
target_include_directories(Container
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	)
	
add_executable(ConsoleSort
	src/ConsoleSort.cpp
	)
	
target_link_libraries(ConsoleSort
	PRIVATE
		Container
	)

