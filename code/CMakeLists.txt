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
  akagoria_s.cc

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
  bits/Opening.cc
  bits/OpeningAct.cc
  bits/OpeningBaseScene.cc
  bits/OpeningMenuScene.cc
  bits/OpeningOptionsScene.cc
  bits/OpeningSelectorScene.cc
  bits/OptionsRenderer.cc
  bits/OptionsScenery.cc
  bits/PhysicsRuntime.cc
  bits/PhysicsState.cc
  bits/ProgressionState.cc
  bits/Root.cc
  bits/RootData.cc
  bits/Script.cc
  bits/SlotSelectorRenderer.cc
  bits/SlotSelectorScenery.cc
  bits/StartMenuRenderer.cc
  bits/StartMenuScenery.cc
  bits/UIData.cc
  bits/VfxRenderer.cc
  bits/VfxScenery.cc
  bits/WeaponState.cc
  bits/World.cc
  bits/WorldAct.cc
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
    ${BOX2D_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(akagoria
  PRIVATE
    gf::gf0
    Threads::Threads
    wren
    Boost::boost
    Boost::locale
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
  bits/RootData.cc
  bits/WorldData.cc
)

target_include_directories(akagoria_datacompile
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/json/single_include"
)

target_link_libraries(akagoria_datacompile
  PRIVATE
    gf::gf0
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
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(akagoria_dataview
  PRIVATE
    gf::gf0
    Boost::boost
    Boost::locale
    fmt-header-only
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
  PRIVATE
    gf::gf0
    Boost::boost
    Boost::locale
    ${BOX2D_LIBRARIES}
    fmt-header-only
)


add_executable(akagoria_uimessagesview
  akagoria_uimessagesview.cc
  bits/Fmt.cc
  bits/RootData.cc
)

target_include_directories(akagoria_uimessagesview
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(akagoria_uimessagesview
  PRIVATE
    gf::gf0
    Boost::boost
    Boost::locale
    fmt-header-only
)
