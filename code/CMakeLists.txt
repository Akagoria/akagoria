configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

#
# wren
#

add_library(wren STATIC
  vendor/wren/src/vm/wren_compiler.c
  vendor/wren/src/vm/wren_core.c
  vendor/wren/src/vm/wren_debug.c
  vendor/wren/src/vm/wren_primitive.c
  vendor/wren/src/vm/wren_utils.c
  vendor/wren/src/vm/wren_value.c
  vendor/wren/src/vm/wren_vm.c
)

target_include_directories(wren
  PUBLIC
    vendor/wren/src/include
)

target_compile_definitions(wren
  PRIVATE
    WREN_OPT_META=0
    WREN_OPT_RANDOM=0
)

#
# akagoria
#

add_executable(akagoria
  akagoria.cc
  bits/AreaRenderer.cc
  bits/AttributesRenderer.cc
  bits/CharacterRenderer.cc
  bits/Commands.cc
  bits/CommandsHelperRenderer.cc
  bits/DialogRenderer.cc
  bits/GameMenuRenderer.cc
  bits/GameMenuScenery.cc
  bits/HeroRenderer.cc
  bits/InventoryRenderer.cc
  bits/ItemRenderer.cc
  bits/LogoRenderer.cc
  bits/MapRenderer.cc
  bits/Menu.cc
  bits/MiniMapRenderer.cc
  bits/NotificationRenderer.cc
  bits/OpeningDriver.cc
  bits/OpeningProcessor.cc
  bits/OptionsRenderer.cc
  bits/OptionsScenery.cc
  bits/PhysicsRuntime.cc
  bits/PhysicsState.cc
  bits/Script.cc
  bits/ShrineRenderer.cc
  bits/SlotSelectorRenderer.cc
  bits/SlotSelectorScenery.cc
  bits/StartMenuRenderer.cc
  bits/StartMenuScenery.cc
  bits/UIData.cc
  bits/WorldData.cc
  bits/WorldDriver.cc
  bits/WorldProcessor.cc
  bits/WorldScenery.cc
  bits/WorldState.cc

  bits/ui/Common.cc
  bits/ui/Models.cc
  bits/ui/Theme.cc
  bits/ui/Widget.cc
  bits/ui/Widgets.cc
)

target_include_directories(akagoria
  PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${BOX2D_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(akagoria
  gf::gf0
  wren
  ${Boost_LIBRARIES}
  ${BOX2D_LIBRARIES}
)

install(
  TARGETS akagoria
  RUNTIME DESTINATION games
)

#
# tools
#

add_subdirectory(vendor/fmt EXCLUDE_FROM_ALL)

add_executable(akagoria_datacompile
  akagoria_datacompile.cc
  bits/UIData.cc
  bits/WorldData.cc
)

target_include_directories(akagoria_datacompile
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/json/single_include"
)

target_link_libraries(akagoria_datacompile
  gf::gf0
  ${Boost_LIBRARIES}
)


add_executable(akagoria_dataview
  akagoria_dataview.cc
  bits/Fmt.cc
  bits/WorldData.cc
)

target_include_directories(akagoria_dataview
  PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(akagoria_dataview
  gf::gf0
  fmt-header-only
  ${Boost_LIBRARIES}
)


add_executable(akagoria_stateview
  akagoria_stateview.cc
  bits/Fmt.cc
  bits/PhysicsState.cc
  bits/WorldState.cc
)

target_include_directories(akagoria_stateview
  PRIVATE
    ${BOX2D_INCLUDE_DIRS}
)

target_link_libraries(akagoria_stateview
  gf::gf0
  ${BOX2D_LIBRARIES}
  fmt-header-only
)


add_executable(akagoria_uimessagesview
  akagoria_uimessagesview.cc
  bits/Fmt.cc
  bits/UIData.cc
)

target_include_directories(akagoria_uimessagesview
  PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(akagoria_uimessagesview
  gf::gf0
  fmt-header-only
  ${Boost_LIBRARIES}
)
