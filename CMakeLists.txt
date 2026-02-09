cmake_minimum_required(VERSION 3.16)

project(usb-pci-explorer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Transparent search for Qt6 or Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/usb_mapper.h
        src/usb_mapper.cpp
        src/usb_adapter.h
        src/usb_adapter.cpp
)

add_executable(usb-pci-explorer
    ${PROJECT_SOURCES}
)

target_link_libraries(usb-pci-explorer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

include(GNUInstallDirs)

# Standard Linux / RPM installation paths
install(TARGETS usb-pci-explorer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES resources/usb-pci-explorer.svg
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps
)

install(FILES desktop/usb-pci-explorer.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
)

# Qt 6 specific build optimizations
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(usb-pci-explorer)
endif()
