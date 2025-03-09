cmake_minimum_required(VERSION 3.10)
project(SentimentAnalysis)

set(CMAKE_CXX_STANDARD 11)

# Include directories
include_directories(src)

# Add the executable
add_executable(sentiment 
    src/main.cpp
    src/DSString.cpp
    src/SentimentClassifier.cpp
)

# Copy data directory to build directory
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})