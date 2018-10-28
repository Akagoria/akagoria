
set(AKAGORIA_DATABASE_FILES
  raw/database/characters.json
  raw/database/dialogs.json
  raw/database/notifications.json
  raw/database/ui.json
)

set(AKAGORIA_MAP_FILES
  raw/akagoria.tmx
)

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/akagoria.dat"
  DEPENDS
    akagoria_datacompile
    ${AKAGORIA_DATABASE_FILES}
    ${AKAGORIA_MAP_FILES}
  COMMAND
    akagoria_datacompile "${CMAKE_CURRENT_SOURCE_DIR}/raw" "${CMAKE_CURRENT_BINARY_DIR}/akagoria.dat"
)

add_custom_target(akagoria.dat
  ALL
  DEPENDS
    "${CMAKE_CURRENT_BINARY_DIR}/akagoria.dat"
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/akagoria"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/raw/tilesets"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games/akagoria"
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/raw/pictures"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games/akagoria"
)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/akagoria.dat"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games/akagoria"
)
