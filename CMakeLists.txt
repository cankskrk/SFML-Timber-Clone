cmake_minimum_required(VERSION 3.28)
project(SFML-Timber-Clone)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Add executable and link required libraries
add_executable(timber src/Timber.cpp)
target_compile_features(timber PRIVATE cxx_std_17)
target_link_libraries(timber PRIVATE SFML::Graphics SFML::Audio) # Add SFML::Audio here

# Copy the graphics folder to the output directory
add_custom_command(
    TARGET timber POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/graphics $<TARGET_FILE_DIR:timber>/graphics
)

# Copy the fonts folder to the output directory
add_custom_command(
    TARGET timber POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/fonts $<TARGET_FILE_DIR:timber>/fonts
)

# Copy the sound folder to the output directory
add_custom_command(
    TARGET timber POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/sound $<TARGET_FILE_DIR:timber>/sound
)
