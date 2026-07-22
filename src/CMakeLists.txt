kcoreaddons_add_plugin(runner_bashrunner SOURCES bashrunner.cpp INSTALL_NAMESPACE "kf6/krunner")

target_link_libraries(runner_bashrunner
        KF6::I18n
        KF6::ConfigCore
        KF6::Runner
        KF6::KIOWidgets
        KF6::Notifications
        Qt6::Core
)