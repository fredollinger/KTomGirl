cmake_minimum_required(VERSION 3.0)
project(ktomgirl)

set (QT_QMAKE_EXECUTABLE "/usr/bin/qmake-qt4")
set (CMAKE_INSTALL_PREFIX "/usr")
set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.2.0")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SIGC2 REQUIRED sigc++-2.0)
include_directories(${SIGC2_INCLUDE_DIRS})

find_package(LibXml2 REQUIRED)
include_directories(${LibXML_INCLUDE_DIRS})
include_directories("/usr/include/libxml2")

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS 
    Core    # QCommandLineParser, QStringLiteral
    Widgets # QApplication 
)

#
#SET(QtApp_RCCS ui/ktomgirl.qrc)
#
## generate rules for building source files from the resources
#QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCCS})
#
#kde4_add_kcfg_files(ktomgirl_SRCS ktgconfigskel.kcfgc)
#
set (CMAKE_CXX_FLAGS "-L/usr/lib/x86_64-linux-gnu -lX11 -luuid -fexceptions -lboost_filesystem -lboost_date_time -lpcrecpp -lxml2 -lktomgirl -I.")

set(ktomgirl_SRCS
    ktgitem.cpp
)
#ktgmenu.cpp
#ktgsystray.cpp
#notesdialog.cpp
#notebooksdialog.cpp
#ktgconfig.cpp
#ktgconfigskel.cpp
#ktgitem.cpp
#ktgdateitem.cpp
#knote.cpp
#newnotebookdialog.cpp
#searchbar.cpp
#searchwindow.cpp
#resourcemanager.cpp
#knotesapp.cpp
#knoteedit.cpp
#main.cpp
#application.cpp
#)
#
#kde4_add_ui_files(ktomgirl_SRCS 
#ui/newnotebookdialog.ui
#ui/notebooksdialog.ui
#ui/notesdialog.ui
#ui/searchbar.ui
#ui/searchwindow.ui
#)
#
#kde4_add_executable(ktomgirl ${ktomgirl_SRCS} ${QtApp_RCC_SRCS})
add_executable(ktomgirl ${ktomgirl_SRCS})
#
#target_link_libraries(ktomgirl ${CMAKE_CXX_FLAGS} ${KDEPIMLIBS_KCAL_LIBS} ${KDEPIMLIBS_KRESOURCES_LIBS} ${KDE4_KIO_LIBS}  ${KDE4_KDNSSD_LIBS}
#${LibXML_LIBRARIES}  ${SIGC2_LIBRARIES} 
#${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KDE3SUPPORT_LIBS} ${KDE4_KTEXTEDITOR_LIBS})
#
#install( TARGETS ktomgirl  ${INSTALL_TARGETS_DEFAULT_ARGS} )
# Tue 25 Feb 2020 07:33:46 PM PST