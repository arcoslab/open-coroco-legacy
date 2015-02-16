# Install script for directory: /tmp/buildd/kicad-0.20120526+bzr3261/library/modules

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "resources")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/kicad/modules" TYPE FILE FILES
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/affich.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/display.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/miscellaneous.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/connect2.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/muonde.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/conn_HExx.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/dip_sockets.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/libcms-bga.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/led.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/pga_sockets.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/dip_sockets_special.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/discret1.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/pin_array.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/sockets_miscellaneous.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/conn_DBxx.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/libcms.brd"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/connect1.brd"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "resources")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "resources")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/kicad/modules" TYPE FILE FILES
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_capa_0805.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_passive.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_resistor_1206.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/standard-trans-diodes.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/miscellaneous.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_capa_pol.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_capa_1206.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_trans-diodes.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/ci.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/device.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_active.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/capa_pol.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/devcms.equ"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_resistor_0805.equ"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "resources")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "resources")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/kicad/modules" TYPE FILE FILES
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/led.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/discret.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/divers.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_resistors.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/display.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/pga_sockets.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/muonde.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_capacitors.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/dip_sockets.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/libcms.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/iut.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/capacitors.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_transistors.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/sockets.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/valves.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/powerint.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/smd_dil.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/pin_array.mod"
    "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/connect.mod"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "resources")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "resources")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/kicad/modules" TYPE DIRECTORY FILES "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/packages3d" REGEX "/\\.svn$" EXCLUDE)
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "resources")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "resources")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/kicad/modules" TYPE DIRECTORY FILES "/tmp/buildd/kicad-0.20120526+bzr3261/library/modules/footprints_doc" REGEX "/\\.svn$" EXCLUDE)
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "resources")

