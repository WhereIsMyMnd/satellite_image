cmake_minimum_required(VERSION 3.16)

project(satellite_picture VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

find_package(GeographicLib QUIET)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        picturelabel.h
        picturelabel.cpp
)

qt_add_executable(satellite_picture
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(satellite_picture 
	PRIVATE 
		Qt6::Widgets 
		Qt6::Network
)

if (GeographicLib_FOUND)
	target_link_libraries (satellite_picture PRIVATE GeographicLib::GeographicLib)
	target_compile_definitions(satellite_picture PRIVATE GEO_LIB=1)
else()
	target_compile_definitions(satellite_picture PRIVATE GEO_LIB=0)
endif()


include(GNUInstallDirs)
install(TARGETS satellite_picture
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(satellite_picture)
