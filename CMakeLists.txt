cmake_minimum_required(VERSION 3.16)

project(FileXplorerInstaller VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)


file(GLOB SOURCES
    src/*.cpp
)

file(GLOB SRV_HEADER_PATH
    src
)

file(GLOB HEADERS
    src/*.h
)

set(RESOURCES
    "resources/images.qrc"
)

set(TS_FILES resources/Translate/FileXplorerInstaller_zh_CN.ts)
set(PROJECT_SOURCES
        ${SOURCES}
        main.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FileXplorerInstaller
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${RESOURCES}
    )
else()
    add_executable(FileXplorerInstaller
        ${PROJECT_SOURCES}
        ${RESOURCES}
    )
endif()
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_sources(FileXplorerInstaller PRIVATE
    ${HEADERS}
    ${QM_FILES}
    resources/Translate
    readme.md
)

target_include_directories(FileXplorerInstaller PRIVATE ${SRV_HEADER_PATH})

target_link_libraries(FileXplorerInstaller PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FileXplorerInstaller)
endif()
set_target_properties(FileXplorerInstaller PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS FileXplorerInstaller
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FileXplorerInstaller)
endif()
