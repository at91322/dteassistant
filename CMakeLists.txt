cmake_minimum_required(VERSION 3.19)
project(dte LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

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
    enterkeyhandler.h
    documentationhelper.h documentationhelper.cpp
    docs/sample.md
    keyboardautomation.h keyboardautomation.cpp
    statusbarmanager.h statusbarmanager.cpp
    windowmanager.h windowmanager.cpp
    excelmanager.h excelmanager.cpp
    configmanager.h configmanager.cpp
    build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/docs/ARTA_howTo.md
    build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/docs/CHNG_howTo.md
    build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/docs/DIPL_howTo.md
    spreadsheetview.h spreadsheetview.cpp
    ValidationError.h
    transcriptreviewvalidator.h transcriptreviewvalidator.cpp
    LICENSE.md
    README.md
)

target_link_libraries(dte
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_link_libraries(dte PRIVATE Qt6::Widgets)

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
