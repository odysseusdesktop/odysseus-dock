cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME odysseus-dock)
project(${PROJECT_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT Core Widgets Quick QuickControls2 X11Extras DBus)
find_package(Qt5 REQUIRED ${QT})
find_package(KF5WindowSystem REQUIRED)

find_library(libodysseus-system NAMES libodysseus-system PATHS /usr/local/lib REQUIRED)

set(SRCS
    src/applicationitem.h
    src/applicationmodel.cpp
    src/applicationmodel.h
    src/docksettings.cpp
    src/docksettings.h
    src/iconthemeimageprovider.cpp
    src/iconthemeimageprovider.h
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/systemappmonitor.cpp
    src/systemappmonitor.h
    src/systemappitem.cpp
    src/systemappitem.h
    src/processprovider.cpp
    src/processprovider.h
    src/popuptips.cpp
    src/popuptips.h
    src/trashmanager.cpp
    src/trashmanager.h
    src/utils.cpp
    src/utils.h
    src/xwindowinterface.cpp
    src/xwindowinterface.h
)

set(RESOURCES
    resources.qrc
)

add_executable(${PROJECT_NAME} ${SRCS} ${DBUS_SRCS} ${RESOURCES})
target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Widgets
        Qt5::Quick
        Qt5::QuickControls2
        Qt5::X11Extras
        Qt5::DBus
        KF5::WindowSystem
        ${libodysseus-system}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION /usr/bin)
