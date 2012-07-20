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

QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCCS})

set (CMAKE_CXX_FLAGS "-fexceptions -lboost_filesystem-mt -lpcrecpp")

set(ktomgirl_SRCS
main.cpp
)

kde4_add_executable(ktomgirl ${ktomgirl_SRCS} ${QtApp_RCC_SRCS})

#target_link_libraries(yourapp ${SIGC2_LIBRARIES})
# target_link_libraries(ktomgirl ${LibXML_LIBRARIES} ${SIGC2_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KDE3SUPPORT_LIBS} ${KDE4_KTEXTEDITOR_LIBS})

target_link_libraries(ktomgirl ${KDEPIMLIBS_KCAL_LIBS} kdepim ${KDEPIMLIBS_KRESOURCES_LIBS} ${KDE4_KIO_LIBS}  ${KDE4_KDNSSD_LIBS}
${LibXML_LIBRARIES}  ${SIGC2_LIBRARIES} 
${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KDE3SUPPORT_LIBS} ${KDE4_KTEXTEDITOR_LIBS})

install(TARGETS ktomgirl  DESTINATION ${BIN_INSTALL_DIR})
# Fri Feb 10 09:20:49 PST 2012
