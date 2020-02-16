
set(AKAGORIA_DATABASE_FILES
  raw/database/atlases.json
  raw/database/characters.json
  raw/database/dialogs.json
  raw/database/items.json
  raw/database/notifications.json
  raw/database/spells.json
  raw/database/ui.json
  raw/database/weapons.json
)

set(AKAGORIA_MAP_FILES
  raw/akagoria.tmx
)

add_custom_command(
  OUTPUT
    "${CMAKE_CURRENT_BINARY_DIR}/akagoria.dat"
    "${CMAKE_CURRENT_BINARY_DIR}/root.dat"
    "${CMAKE_CURRENT_BINARY_DIR}/strings.cc"
  DEPENDS
    akagoria_datacompile
    ${AKAGORIA_DATABASE_FILES}
    ${AKAGORIA_MAP_FILES}
  COMMAND
    akagoria_datacompile "${CMAKE_CURRENT_SOURCE_DIR}/raw" "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_target(akagoria.dat
  ALL
  DEPENDS
    "${CMAKE_CURRENT_BINARY_DIR}/akagoria.dat"
)

add_subdirectory(raw/i18n)

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
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/akagoria.dat"
    "${CMAKE_CURRENT_BINARY_DIR}/root.dat"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games/akagoria"
)
