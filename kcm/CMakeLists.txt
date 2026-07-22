kcmutils_add_qml_kcm(kcm_bashrunner)
target_sources(kcm_bashrunner PRIVATE
        bashrunner_config.cpp
        bashrunner_config.h
)

kconfig_target_kcfg_file(kcm_bashrunner
        FILE config.kcfg
        CLASS_NAME Config
        MUTATORS
        DEFAULT_VALUE_GETTERS
        GENERATE_PROPERTIES
        PARENT_IN_CONSTRUCTOR
        SINGLETON
)


target_link_libraries(kcm_bashrunner PRIVATE
        Qt::Quick
        KF6::CoreAddons
        KF6::ConfigCore
        KF6::KCMUtils
        KF6::KCMUtilsQuick
        KF6::I18n
)