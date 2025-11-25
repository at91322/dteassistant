cmake_minimum_required(VERSION 3.19)
project(dte LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(dte
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    articulator.h articulator.cpp articulator.ui
    termbuilder.h termbuilder.cpp termbuilder.ui
    transcriptreviewer.h transcriptreviewer.cpp transcriptreviewer.ui
    programchanger.h programchanger.cpp programchanger.ui
    diplomadates.h diplomadates.cpp diplomadates.ui
    app.rc
    autotab.h

)

target_link_libraries(dte
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS dte
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET dte
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
