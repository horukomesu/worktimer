cmake_minimum_required(VERSION 3.16)
project(WorkTimer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6 configuration
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.1/msvc2022_64")

# Try Qt6 first, then Qt5 as fallback
find_package(Qt6 QUIET COMPONENTS Core Widgets Multimedia)
if(Qt6_FOUND)
    message(STATUS "Found Qt6")
    set(QT_VERSION 6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia)
    if(Qt5_FOUND)
        message(STATUS "Found Qt5")
        set(QT_VERSION 5)
    else()
        message(FATAL_ERROR "Neither Qt6 nor Qt5 found. Please install Qt6 or Qt5.")
    endif()
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Source files
set(SOURCES
    src/main.cpp
    src/worktimer.cpp
    src/worktimer_methods.cpp
    src/settings.cpp
)

set(HEADERS
    include/worktimer.h
    include/settings.h
)

set(RESOURCES
    resources.qrc
)

# Create executable
add_executable(WorkTimer ${SOURCES} ${HEADERS} ${RESOURCES})

# Enable Qt MOC
set_target_properties(WorkTimer PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Include directories
target_include_directories(WorkTimer PRIVATE include)

# Link Qt libraries
if(QT_VERSION EQUAL 6)
    target_link_libraries(WorkTimer PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Multimedia
    )
else()
    target_link_libraries(WorkTimer PRIVATE
        Qt5::Core
        Qt5::Widgets
        Qt5::Multimedia
    )
endif()

# Windows specific settings
if(WIN32)
    set_target_properties(WorkTimer PROPERTIES
        WIN32_EXECUTABLE TRUE
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    
    # Set application icon for Windows
    set_target_properties(WorkTimer PROPERTIES
        WIN32_EXECUTABLE TRUE
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    
    # Add icon to executable
    if(EXISTS "${CMAKE_SOURCE_DIR}/resources/worktimer_icon.ico")
        set_target_properties(WorkTimer PROPERTIES
            WIN32_EXECUTABLE TRUE
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
        
        # Create Windows resource file for icon
        set(ICON_RC "${CMAKE_BINARY_DIR}/icon.rc")
        file(WRITE ${ICON_RC} "IDI_ICON1 ICON \"${CMAKE_SOURCE_DIR}/resources/worktimer_icon.ico\"")
        target_sources(WorkTimer PRIVATE ${ICON_RC})
    endif()
    
    # Copy sounds folder to output
    add_custom_command(TARGET WorkTimer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/sounds
        ${CMAKE_BINARY_DIR}/bin/sounds
    )
    
    # Deploy Qt dependencies using windeployqt
    if(QT_VERSION EQUAL 6)
        find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS "${CMAKE_PREFIX_PATH}/bin")
        if(WINDEPLOYQT_EXECUTABLE)
            add_custom_command(TARGET WorkTimer POST_BUILD
                COMMAND ${WINDEPLOYQT_EXECUTABLE}
                    --no-compiler-runtime
                    --no-translations
                    --no-system-d3d-compiler
                    --no-opengl-sw
                    --dir "${CMAKE_BINARY_DIR}/bin"
                    "$<TARGET_FILE:WorkTimer>"
                COMMENT "Deploying Qt6 dependencies with windeployqt"
            )
        else()
            message(WARNING "windeployqt not found. Qt dependencies may be missing.")
        endif()
    else()
        find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS "${CMAKE_PREFIX_PATH}/bin")
        if(WINDEPLOYQT_EXECUTABLE)
            add_custom_command(TARGET WorkTimer POST_BUILD
                COMMAND ${WINDEPLOYQT_EXECUTABLE}
                    --no-compiler-runtime
                    --no-translations
                    --no-system-d3d-compiler
                    --no-opengl-sw
                    --dir "${CMAKE_BINARY_DIR}/bin"
                    "$<TARGET_FILE:WorkTimer>"
                COMMENT "Deploying Qt5 dependencies with windeployqt"
            )
        else()
            message(WARNING "windeployqt not found. Qt dependencies may be missing.")
        endif()
    endif()
endif()

# Install
install(TARGETS WorkTimer
    RUNTIME DESTINATION bin
)

install(DIRECTORY sounds/
    DESTINATION bin/sounds
    FILES_MATCHING PATTERN "*.mp3"
) 