cmake_minimum_required(VERSION 3.21)
project(serever)

set(CMAKE_CXX_STANDARD 17)

# Set the path to the MongoDB C++ driver
set(MONGOCXX_ROOT_DIR "C:\\Users\\yoavl\\Desktop\\mongo-cxx-driver-r3.7.1")

# Manually set the mongocxx_DIR variable
set(mongocxx_DIR "${MONGOCXX_ROOT_DIR}/lib/cmake/mongocxx-3.6.1")

# Find the MongoDB C++ driver
find_package(mongocxx REQUIRED)

# Specify the source files of your project
set(SOURCE_FILES main.cpp)

# Create the executable
add_executable(serever ${SOURCE_FILES})

# Link the required libraries
target_link_libraries(serever PRIVATE mongocxx::mongocxx_shared)
