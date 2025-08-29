cmake_minimum_required(VERSION 3.16)
project(gxde-kernel-manager VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

qt_add_executable(gxde-kernel-manager WIN32 MACOSX_BUNDLE
    aboutwindow.cpp aboutwindow.h aboutwindow.ui
    aptpkginfo.cpp aptpkginfo.h
    kernelinformation.cpp kernelinformation.h
    kernelinformationdialog.cpp kernelinformationdialog.h kernelinformationdialog.ui
    kernelinstaller.cpp kernelinstaller.h kernelinstaller.ui
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    programinfo.cpp programinfo.h
)
target_link_libraries(gxde-kernel-manager PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
)

# generate qm
execute_process(COMMAND bash "translate_generation.sh"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# Resources:
set(Resource_resource_files
    "Donate/Alipay.jpg"
    "Donate/QQ.png"
    "Donate/Wechat.png"
    "Donate/advertisement0.jpg"
    "data/new/en_US.html"
    "data/new/zh_CN.html"
    "data/version"
    "icon/dialog-apply.svg"
    "icon/dialog-cancel.svg"
    "icon/dialog-error.svg"
    "icon/dialog-information.svg"
    "icon/dialog-ok.svg"
    "icon/dialog-question.svg"
    "icon/dialog-warning.svg"
    "icon/gxde-kernel-manager.svg"
    "icon/icon1.png"
    "icon/tux.png"
    "shell/kernel-installer-reconfigure-template.sh"
    "shell/kernel-installer-remove-template.sh"
    "shell/kernel-installer-root-loader-template.sh"
    "shell/kernel-installer-template.sh"
    "shell/kernel-installer-update-template.sh"
    "shell/kernel-installer-upgrade-template.sh"
    "translations/gxde-kernel-manager_zh_CN.qm"
)

qt_add_resources(gxde-kernel-manager "Resource"
    PREFIX
        "/"
    FILES
        ${Resource_resource_files}
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(gxde-kernel-manager PRIVATE
        Qt::Widgets
    )
endif()

if(UNIX AND NOT MACOS)
    target_link_libraries(gxde-kernel-manager PRIVATE
        qtermwidget6
    )
endif()

install(TARGETS gxde-kernel-manager
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
