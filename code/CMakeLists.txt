configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

#
# akagoria
#

add_executable(akagoria
  akagoria.cc

  bits/Akagoria.cc
  bits/AreaRenderer.cc
  bits/Aspect.cc
  bits/AspectState.cc
  bits/AspectRenderer.cc
  bits/AttributeState.cc
  bits/CharacterRenderer.cc
  bits/Commands.cc
  bits/CommandsHelperRenderer.cc
  bits/DialogRenderer.cc
  bits/FloorRenderer.cc
  bits/GameMenuRenderer.cc
  bits/GameMenuScenery.cc
  bits/GamepadControl.cc
  bits/HeroRenderer.cc
  bits/InventoryRenderer.cc
  bits/ItemRenderer.cc
  bits/LogoRenderer.cc
  bits/MapRenderer.cc
  bits/Menu.cc
  bits/MiniMapRenderer.cc
  bits/NotificationRenderer.cc
  bits/OpeningAct.cc
  bits/OpeningBaseScene.cc
  bits/OpeningMenuScene.cc
  bits/OpeningOptionsScene.cc
  bits/OpeningSelectorScene.cc
  bits/OpeningWaitingScene.cc
  bits/OptionsRenderer.cc
  bits/OptionsScenery.cc
  bits/PhysicsRuntime.cc
  bits/PhysicsState.cc
  bits/ProgressionState.cc
  bits/Root.cc
  bits/RootData.cc
  bits/Script.cc
  bits/SlotManager.cc
  bits/SlotSelectorRenderer.cc
  bits/SlotSelectorScenery.cc
  bits/StartMenuRenderer.cc
  bits/StartMenuScenery.cc
  bits/UIData.cc
  bits/UniverseRenderer.cc
  bits/VfxRenderer.cc
  bits/WaitingRenderer.cc
  bits/WeaponState.cc
  bits/World.cc
  bits/WorldAct.cc
  bits/WorldBaseScene.cc
  bits/WorldDialogScene.cc
  bits/WorldData.cc
  bits/WorldInventoryScene.cc
  bits/WorldMenuScene.cc
  bits/WorldOptionsScene.cc
  bits/WorldScenery.cc
  bits/WorldSelectorScene.cc
  bits/WorldState.cc
  bits/WorldTravelScene.cc

  bits/ui/Common.cc
  bits/ui/Models.cc
  bits/ui/Theme.cc
  bits/ui/Widget.cc
  bits/ui/Widgets.cc

  vendor/gf-box2d/gfb2d/PhysicsModel.cc
  vendor/gf-box2d/gfb2d/PhysicsDebugger.cc

  vendor/agate/agate.c
)

target_include_directories(akagoria
  PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/gf-box2d"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/agate"
)

target_link_libraries(akagoria
  PRIVATE
    gf::graphics
    Threads::Threads
    Boost::boost
    Boost::locale
    box2d::box2d
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
  bits/RootData.cc
  bits/WorldData.cc
)

target_include_directories(akagoria_datacompile
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/json/single_include"
)

target_link_libraries(akagoria_datacompile
  PRIVATE
    gf::graphics
    Boost::boost
    Boost::locale
)


add_executable(akagoria_dataview
  akagoria_dataview.cc
  bits/Fmt.cc
  bits/WorldData.cc
)

target_include_directories(akagoria_dataview
  PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(akagoria_dataview
  PRIVATE
    gf::graphics
    Boost::boost
    Boost::locale
    fmt-header-only
)


add_executable(akagoria_stateview
  akagoria_stateview.cc
  bits/Fmt.cc
  bits/PhysicsState.cc
  bits/WorldState.cc

  vendor/gf-box2d/gfb2d/PhysicsModel.cc
)

target_include_directories(akagoria_stateview
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/gf-box2d"
)

target_link_libraries(akagoria_stateview
  PRIVATE
    gf::graphics
    Boost::boost
    Boost::locale
    box2d::box2d
    fmt-header-only
)


add_executable(akagoria_rootdataview
  akagoria_rootdataview.cc
  bits/Fmt.cc
  bits/RootData.cc
)

target_include_directories(akagoria_rootdataview
  PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(akagoria_rootdataview
  PRIVATE
    gf::graphics
    Boost::boost
    Boost::locale
    fmt-header-only
)
