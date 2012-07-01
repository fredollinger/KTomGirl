cmake_minimum_required(VERSION 2.8)
project(ktomgirl)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SIGC2 REQUIRED sigc++-2.0)
include_directories(${SIGC2_INCLUDE_DIRS})

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})
find_package(LibXml2 REQUIRED)
include_directories(${LibXML_INCLUDE_DIRS})
include_directories("/usr/include/libxml2")

kde4_add_ui_files(ktomgirl_SRCS 
)

kde4_add_kcfg_files(ktomgirl_SRCS ktgconfigskel.kcfgc)
install(FILES ktgconfigskel.kcfg DESTINATION ${KCFG_INSTALL_DIR})

set (CMAKE_CXX_FLAGS "-fexceptions -lboost_filesystem-mt -lboost_date_time-mt -lpcrecpp -lktomgirl")

set(ktomgirl_SRCS
notesdialog.cpp
notebooksdialog.cpp
ktgconfig.cpp
ktgconfigskel.cpp
ktgitem.cpp
knote.cpp
searchwindow.cpp
resourcemanager.cpp
knotesapp.cpp
knoteedit.cpp
main.cpp
application.cpp
../libtomgirl/actionmanager.cpp
../libtomgirl/datetime.cpp
../libtomgirl/debug.cpp
../libtomgirl/gnote.cpp
../libtomgirl/files.cpp
../libtomgirl/ktglib.cpp
../libtomgirl/note.cpp
../libtomgirl/notedata.cpp
../libtomgirl/notebuffer.cpp
../libtomgirl/notemanager.cpp
../libtomgirl/preferences.cpp
../libtomgirl/sharp.cpp
../libtomgirl/string.cpp
../libtomgirl/tag.cpp
../libtomgirl/xml.cpp
../libtomgirl/xmlconvert.cpp
../libtomgirl/xmlreader.cpp
../libtomgirl/xmlwriter.cpp
)

kde4_add_executable(ktomgirl ${ktomgirl_SRCS})

#target_link_libraries(yourapp ${SIGC2_LIBRARIES})
# target_link_libraries(ktomgirl ${LibXML_LIBRARIES} ${SIGC2_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KDE3SUPPORT_LIBS} ${KDE4_KTEXTEDITOR_LIBS})

target_link_libraries(ktomgirl ${KDEPIMLIBS_KCAL_LIBS} kdepim ${KDEPIMLIBS_KRESOURCES_LIBS} ${KDE4_KIO_LIBS}  ${KDE4_KDNSSD_LIBS}
${LibXML_LIBRARIES}  ${SIGC2_LIBRARIES} 
${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KDE3SUPPORT_LIBS} ${KDE4_KTEXTEDITOR_LIBS})

install(TARGETS ktomgirl  DESTINATION ${BIN_INSTALL_DIR})
install(FILES ui/ktomgirl.rc DESTINATION ${DATA_INSTALL_DIR}/ktomgirl)
install(FILES DESTINATION ${BIN_INSTALL_DIR})
# Fri Feb 10 09:20:49 PST 2012
