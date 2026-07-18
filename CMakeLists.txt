cmake_minimum_required(VERSION 3.16)
project(BashRunner)

set(CMAKE_CXX_STANDARD 20)

set(KF5_MIN_VERSION "5.90")

# Include the Extra-CMake-Modules project
find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS I18n Config Runner KIO Notifications)

# This takes care of building and installing the plugin
kcoreaddons_add_plugin(runner_bashrunner SOURCES bashrunner.cpp INSTALL_NAMESPACE "kf6/krunner")
# We need to link the KRunner and other used libraries  to it
target_link_libraries(runner_bashrunner
        KF6::I18n
        KF6::ConfigCore
        KF6::Runner
        KF6::KIOWidgets
        KF6::Notifications
)