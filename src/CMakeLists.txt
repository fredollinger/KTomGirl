cmake_minimum_required(VERSION 2.8)
project(ktomgirl)

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

kde4_add_ui_files(ktomgirl_SRCS 
)

kde4_add_kcfg_files(ktomgirl_SRCS ktgconfig.kcfgc)
install(FILES ktgconfig.kcfg DESTINATION ${KCFG_INSTALL_DIR})


set(ktomgirl_SRCS
gnote.cpp
main.cpp
)

kde4_add_executable(ktomgirl ${ktomgirl_SRCS})

target_link_libraries(ktomgirl ${KDE4_KDEUI_LIBS}
${KDE4_KIO_LIBS}
${KDE4_KDE3SUPPORT_LIBS}
${KDE4_KTEXTEDITOR_LIBS})

install(TARGETS ktomgirl  DESTINATION ${BIN_INSTALL_DIR})
install(FILES ktomgirl-kde4.rc DESTINATION ${DATA_INSTALL_DIR}/ktomgirl-kde4)
install(FILES ktomgirl_config DESTINATION ${BIN_INSTALL_DIR})
# Fri Feb 10 09:20:49 PST 2012
