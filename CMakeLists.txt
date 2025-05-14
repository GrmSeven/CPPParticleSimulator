cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp
        src/Renderer.cpp
        src/Renderer.h
        src/utils.h
        src/particles/particleSimulator.cpp
        src/particles/particleSimulator.h
        src/camera/camera.h
        src/camera/camera.cpp
        src/particles/behaviorManager.h
        src/gravityTable/gravityElement.cpp
        src/gravityTable/gravityElement.h
        src/gravityTable/Ctest.cpp
        src/gravityTable/gravityTable.cpp
        src/gravityTable/gravityTable.h
        src/userInterface/userInterface.cpp
        src/userInterface/userInterface.h
        src/userInterface/element.h
        src/userInterface/checkbox.h
        src/userInterface/range.h
        src/userInterface/button.h
        src/userInterface/dropdown.h
        src/userInterface/matrix.h
        src/threadManager.h
)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)
