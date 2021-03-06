SET(mmapper_SRCS
    main.cpp
    clock/mumemoment.cpp
    clock/mumeclock.cpp
    clock/mumeclockwidget.cpp
    client/clientwidget.cpp
    client/stackedinputwidget.cpp
    client/inputwidget.cpp
    client/displaywidget.cpp
    client/ctelnet.cpp
    configuration/configuration.cpp
    display/connectionselection.cpp
    display/mapcanvas.cpp
    display/mapwindow.cpp
    display/prespammedpath.cpp
    expandoracommon/component.cpp
    expandoracommon/coordinate.cpp
    expandoracommon/frustum.cpp
    expandoracommon/parseevent.cpp
    expandoracommon/property.cpp
    mainwindow/aboutdialog.cpp
    mainwindow/findroomsdlg.cpp
    mainwindow/infomarkseditdlg.cpp
    mainwindow/mainwindow.cpp
    mainwindow/roomeditattrdlg.cpp
    mainwindow/welcomewidget.cpp
    mapdata/customaction.cpp
    mapdata/roomfilter.cpp
    mapdata/shortestpath.cpp
    mapdata/mapdata.cpp
    mapdata/mmapper2exit.cpp
    mapdata/mmapper2room.cpp
    mapdata/roomfactory.cpp
    mapdata/roomselection.cpp
    mapfrontend/intermediatenode.cpp
    mapfrontend/mapaction.cpp
    mapfrontend/map.cpp
    mapfrontend/mapfrontend.cpp
    mapfrontend/roomcollection.cpp
    mapfrontend/roomlocker.cpp
    mapfrontend/searchtreenode.cpp
    mapstorage/abstractmapstorage.cpp
    mapstorage/basemapsavefilter.cpp
    mapstorage/filesaver.cpp
    mapstorage/jsonmapstorage.cpp
    mapstorage/mapstorage.cpp
    mapstorage/oldconnection.cpp
    mapstorage/progresscounter.cpp
    mapstorage/roomsaver.cpp
    pandoragroup/CGroupChar.cpp
    pandoragroup/CGroupClient.cpp
    pandoragroup/CGroupCommunicator.cpp
    pandoragroup/CGroup.cpp
    pandoragroup/CGroupServer.cpp
    pandoragroup/mmapper2group.cpp
    pandoragroup/groupaction.cpp
    pandoragroup/groupselection.cpp
    pandoragroup/groupwidget.cpp
    parser/abstractparser.cpp
    parser/mumexmlparser.cpp
    parser/parserutils.cpp
    parser/patterns.cpp
    parser/roompropertysetter.cpp
    parser/mmapper2event.cpp
    mpi/remoteedit.cpp
    mpi/mpifilter.cpp
    mpi/viewsessionprocess.cpp
    mpi/editsessionprocess.cpp
    mpi/remoteeditwidget.cpp
    pathmachine/approved.cpp
    pathmachine/crossover.cpp
    pathmachine/experimenting.cpp
    pathmachine/mmapper2pathmachine.cpp
    pathmachine/onebyone.cpp
    pathmachine/path.cpp
    pathmachine/pathmachine.cpp
    pathmachine/pathparameters.cpp
    pathmachine/roomsignalhandler.cpp
    pathmachine/syncing.cpp
    preferences/ansicombo.cpp
    preferences/configdialog.cpp
    preferences/generalpage.cpp
    preferences/groupmanagerpage.cpp
    preferences/parserpage.cpp
    preferences/pathmachinepage.cpp
    preferences/clientpage.cpp
    preferences/mumeprotocolpage.cpp
    proxy/connectionlistener.cpp
    proxy/proxy.cpp
    proxy/telnetfilter.cpp
)

SET(mmapper_RCS
    resources/mmapper2.qrc
    resources/mmapper2.rc
    resources/macosx/m.icns
    resources/win32/m.ico
    resources/icons/hi16-app-mmapper.png
    resources/icons/hi32-app-mmapper.png
    resources/icons/hi48-app-mmapper.png
)

SET(mmapper_UIS
    mainwindow/aboutdialog.ui
    mainwindow/findroomsdlg.ui
    mainwindow/infomarkseditdlg.ui
    mainwindow/roomeditattrdlg.ui
    mainwindow/welcomewidget.ui
    clock/mumeclockwidget.ui
    preferences/configdialog.ui
    preferences/generalpage.ui
    preferences/groupmanagerpage.ui
    preferences/parserpage.ui
    preferences/pathmachinepage.ui
    preferences/clientpage.ui
    preferences/mumeprotocolpage.ui
)

IF(WITH_QTIOCOMPRESSOR)
    IF(NOT QTIOCOMPRESSOR_FOUND)
        # Build QtIOCompressor as an object library
        SET(mmapper_SRCS ${mmapper_SRCS} ${QTIOCOMPRESSOR_OBJECTS})
    ENDIF()
    INCLUDE_DIRECTORIES(${QTIOCOMPRESSOR_INCLUDE_DIRS})
ENDIF()

SET(mmapper_DATA
    mmapper.desktop
    mmapper.astylerc
    MacOSXBundleInfo.plist.in
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/clock
    ${CMAKE_CURRENT_SOURCE_DIR}/configuration
    ${CMAKE_CURRENT_SOURCE_DIR}/display
    ${CMAKE_CURRENT_SOURCE_DIR}/expandoracommon
    ${CMAKE_CURRENT_SOURCE_DIR}/global
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow
    ${CMAKE_CURRENT_SOURCE_DIR}/mapdata
    ${CMAKE_CURRENT_SOURCE_DIR}/mapfrontend
    ${CMAKE_CURRENT_SOURCE_DIR}/mapstorage
    ${CMAKE_CURRENT_SOURCE_DIR}/pandoragroup
    ${CMAKE_CURRENT_SOURCE_DIR}/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/pathmachine
    ${CMAKE_CURRENT_SOURCE_DIR}/preferences
    ${CMAKE_CURRENT_SOURCE_DIR}/proxy
    ${OPENGL_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
)

# Build the executable
ADD_EXECUTABLE(mmapper WIN32 MACOSX_BUNDLE
    ${mmapper_SRCS}
    ${mmapper_UIS}
    ${mmapper_RCS}
    ${mmapper_DATA}
)

TARGET_LINK_LIBRARIES(mmapper
    Qt5::Core
    Qt5::Widgets
    Qt5::Xml
    Qt5::Network
    Qt5::OpenGL
    ${OPENGL_gl_LIBRARY}
)

IF(WITH_QTIOCOMPRESSOR)
    IF(QTIOCOMPRESSOR_FOUND)
        TARGET_LINK_LIBRARIES(mmapper ${QTIOCOMPRESSOR_LIBRARIES})
    ELSE()
        # Build QtIOCompressor as an object library
        IF (NOT ZLIB_FOUND)
            # Assume we are building a static zlib
            ADD_DEPENDENCIES(mmapper zlib)
        ENDIF()
        TARGET_LINK_LIBRARIES(mmapper ${ZLIB_LIBRARIES})
    ENDIF()
ENDIF()

# Begin CPack Settings
SET(CPACK_PACKAGE_VERSION ${MMAPPER_VERSION})
STRING(REGEX REPLACE "[.-]" " " VERSION_NUMBERS ${MMAPPER_VERSION})
SEPARATE_ARGUMENTS(VERSION_NUMBERS)
LIST(GET VERSION_NUMBERS 0 CPACK_PACKAGE_VERSION_MAJOR)
LIST(REMOVE_AT VERSION_NUMBERS 0)
LIST(GET VERSION_NUMBERS 0 CPACK_PACKAGE_VERSION_MINOR)
LIST(REMOVE_AT VERSION_NUMBERS 0)
LIST(GET VERSION_NUMBERS 0 CPACK_PACKAGE_VERSION_PATCH)
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-source")
SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")

# Linux Install Settings
IF(UNIX AND NOT APPLE)
    # Install Executables
    INSTALL(
        TARGETS mmapper
        RUNTIME DESTINATION bin
        COMPONENT applications
    )

    # Install Map
    INSTALL(
        FILES ${MMAPPER_MAP_FILE}
        DESTINATION share/games/mmapper
        COMPONENT map
    )

    # Linux Desktop Integration
    INSTALL(FILES mmapper.desktop
            DESTINATION share/applications
            COMPONENT desktopintegration
    )

    # Install icons
    FOREACH(RES 16 32 48)
        INSTALL(
            FILES resources/icons/hi${RES}-app-mmapper.png
            DESTINATION share/icons/hicolor/${RES}x${RES}/apps/
            COMPONENT desktopintegration
            RENAME mmapper.png
        )
    ENDFOREACH(RES)

    INSTALL(
        FILES resources/icons/m.png
        DESTINATION share/icons/hicolor/128x128/apps/
        COMPONENT desktopintegration
        RENAME mmapper.png
    )

    SET(CPACK_GENERATOR "DEB;TGZ") # Others: RPM, STGZ
    SET(CPACK_SOURCE_GENERATOR "TGZ")

    # Debian
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "nschimme@gmail.com")
    SET(CPACK_DEBIAN_PACKAGE_NAME "mmapper")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5core5a (>= 5.4.0), libqt5gui5 (>= 5.4.0), libqt5opengl5 (>= 5.4.0), libqt5network5 (>= 5.4.0), libqt5xml5 (>= 5.4.0)")
    IF(WITH_QTIOCOMPRESSOR)
        SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, zlib1g")
    ENDIF()

    SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
ENDIF(UNIX AND NOT APPLE)

# Windows Install Settings
IF(WIN32)
    SET(CMAKE_WIN32_EXECUTABLE ON)

    # Install Executables
    INSTALL(
        TARGETS mmapper RUNTIME
        DESTINATION bin
        COMPONENT applications
    )

    # Install Map
    INSTALL(
        FILES ${MMAPPER_MAP_FILE}
        DESTINATION .
        COMPONENT map
    )

    # Bundle Library Files
    IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
            SET(WINDEPLOYQT_ARGS --debug)
    ELSEIF(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
            SET(WINDEPLOYQT_ARGS --release-with-debug-info)
    ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
            SET(WINDEPLOYQT_ARGS --release)
    ENDIF()
    FIND_PROGRAM(WINDEPLOYQT_APP windeployqt)
    MESSAGE(" - windeployqt path: ${WINDEPLOYQT_APP}")
    ADD_CUSTOM_COMMAND(
            TARGET mmapper
            POST_BUILD
            COMMAND ${WINDEPLOYQT_APP} ${WINDEPLOYQT_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/mmapper.exe --dir ${CMAKE_CURRENT_BINARY_DIR}/bin --compiler-runtime
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Finding the Qt framework dependencies"
    )
    INSTALL(DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}/bin
        DESTINATION .
        COMPONENT libraries
    )

    # Install Help Documentation
    INSTALL(FILES
        "${PROJECT_SOURCE_DIR}/docs/Installation Guide.URL"
        "${PROJECT_SOURCE_DIR}/docs/mapping_advice.txt"
        DESTINATION .
        COMPONENT help
    )

    # ZIP requires 7-zip
    SET(CPACK_GENERATOR "ZIP;NSIS")
    SET(CPACK_SOURCE_GENERATOR "ZIP")

    # Windows (NSIS) Settings
    SET(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
    SET(CPACK_NSIS_MENU_LINKS "bin/mmapper.exe;MMapper;Installation Guide.URL;Installation Guide")
    SET(CPACK_NSIS_INSTALLED_ICON_NAME "mmapper.exe")
    SET(CPACK_NSIS_DISPLAY_NAME "MMapper")
    SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\github.com\\\\mume\\\\mmapper")
    SET(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/win32\\\\m.ico")
    SET(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/win32\\\\m.ico")
    SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons\\\\m.png")
    SET(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_CURRENT_SOURCE_DIR}/resources/win32\\\\m.ico")
    SET(CPACK_NSIS_COMPRESSOR "/SOLID lzma")
    SET(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON")
    SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "mmapper.exe")
    SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "Caligor soft")

ENDIF(WIN32)

# Apple Install Settings
IF(APPLE)
    SET(CMAKE_MACOSX_BUNDLE ON)
    SET_SOURCE_FILES_PROPERTIES(resources/macosx/m.icns PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources)
    SET_TARGET_PROPERTIES(mmapper PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/MacOSXBundleInfo.plist.in )

    # Bundle the libraries with the binary
    FIND_PROGRAM(MACDEPLOYQT_APP macdeployqt)
    MESSAGE(" - macdeployqt path: ${MACDEPLOYQT_APP}")
    ADD_CUSTOM_COMMAND(
        TARGET mmapper
        POST_BUILD
        COMMAND ${MACDEPLOYQT_APP} ${CMAKE_CURRENT_BINARY_DIR}/mmapper.app
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Deploying the Qt Framework onto the bundle"
        )

    # Install Executables
    INSTALL(
        TARGETS mmapper RUNTIME BUNDLE
        DESTINATION MMapper
        COMPONENT applications
    )

    # Install Map
    INSTALL(
        FILES ${MMAPPER_MAP_FILE}
        DESTINATION MMapper
        COMPONENT map
    )

    # Install Help Documentation
    INSTALL(FILES
        "${PROJECT_SOURCE_DIR}/docs/Installation Guide.URL"
        "${PROJECT_SOURCE_DIR}/docs/mapping_advice.txt"
        DESTINATION MMapper
        COMPONENT help
    )

    # Users don't understand the term Darwin
    STRING(REGEX REPLACE "Darwin" "Mac" CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})

    # Package Settings
    SET(CPACK_GENERATOR "DragNDrop;ZIP")
    SET(CPACK_SOURCE_GENERATOR "TGZ")

    # Libraries are bundled directly
    SET(CPACK_COMPONENT_LIBRARIES_HIDDEN TRUE)

    # Bundle Properties
    SET(MACOSX_BUNDLE_BUNDLE_NAME MMapper)
    SET(MACOSX_BUNDLE_BUNDLE_VERSION ${MMAPPER_VERSION})
    SET(MACOSX_BUNDLE_SHORT_VERSION_STRING ${MMAPPER_VERSION})
    SET(MACOSX_BUNDLE_LONG_VERSION_STRING "Version ${MMAPPER_VERSION}")
    SET(MACOSX_BUNDLE_ICON_FILE m.icns)
ENDIF(APPLE)

# More General CPack Settings
SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING.txt")
#SET(CPACK_RESOURCE_FILE_README  "${PROJECT_SOURCE_DIR}/INSTALL.txt")
SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET(CPACK_PACKAGE_VENDOR "Nils Schimmelmann")
SET(CPACK_PACKAGE_CONTACT "nschimme@gmail.com")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A mud mapper especially written for the mud MUME")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "MMapper")

# Components:
SET(CPACK_STRIP_FILES TRUE)
# Applications Component
SET(CPACK_COMPONENTS_ALL applications map libraries help)
SET(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "MMapper")
SET(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION "A mud mapper especially written for the mud MUME")
SET(CPACK_COMPONENT_APPLICATIONS_REQUIRED TRUE)

# Map Component
SET(CPACK_COMPONENT_MAP_DISPLAY_NAME "Included Map")
SET(CPACK_COMPONENT_MAP_DESCRIPTION "A base map of Arda")

# Libraries Component
SET(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Runtime Libraries")
SET(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Qt/MingW libraries for running MMapper")

# Help Component
SET(CPACK_COMPONENT_HELP_DISPLAY_NAME "Documentation")
SET(CPACK_COMPONENT_HELP_DESCRIPTION "Guide on how to set up your client")
SET(CPACK_COMPONENT_HELP_HIDDEN TRUE)

SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};applications;ALL;/")

# Source Package
SET(CPACK_SOURCE_IGNORE_FILES
    "~$"
    "/\\\\.git/"
    "/build/"
    "/winbuild/"
)
INCLUDE(CPack)
