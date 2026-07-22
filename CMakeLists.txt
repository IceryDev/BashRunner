cmake_minimum_required(VERSION 3.16)
project(BashRunner)

set(CMAKE_CXX_STANDARD 20)

set(KF6_MIN_VERSION "6.0.0")
set(QT_MIN_VERSION "6.5.0")

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Quick Gui Qml)
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS I18n Config Runner KIO Notifications KCMUtils)

add_subdirectory(src)
#add_subdirectory(kcm)