/*
 * Akagoria, the revenge of Kalista
 * a single-player RPG in an open world with a top-down view.
 *
 * Copyright (c) 2013-2018, Julien Bernard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Script.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>

#include <gf/Id.h>
#include <gf/Log.h>
#include <gf/Path.h>

#include "Akagoria.h"
#include "WorldAct.h"
#include "WorldData.h"
#include "WorldState.h"

using namespace gf::literals;

namespace akgr {

  namespace {
    constexpr const char *Unit = "Adventure";

    std::string loadFile(const gf::Path& filename) {
      std::ifstream file(filename.string());

      if (!file) {
        gf::Log::error("File for script does not exist: '%s'\n", filename.string().c_str());
        return "";
      }

      std::string content;

      for (std::string line; std::getline(file, line); ) {
        content.append(line);
        content.append(1, '\n');
      }

      gf::Log::debug("Script loaded from file: '%s'\n", filename.string().c_str());

      return content;
    }

    const char *vmUnitLoad(const char *name, void *user_data) {
      auto script = static_cast<Script *>(user_data);
      return script->loadModule(name);
    }

    AgateUnitHandler vmUnitHandler(AgateVM *vm, [[maybe_unused]] const char *name) {
      AgateUnitHandler handler;
      handler.load = vmUnitLoad;
      handler.release = nullptr;
      handler.user_data = agateGetUserData(vm);
      return handler;
    }

    AgateForeignMethodFunc vmForeignMethodHandler([[maybe_unused]] AgateVM *vm, [[maybe_unused]] const char *unit_name, [[maybe_unused]] const char *class_name, [[maybe_unused]] AgateForeignMethodKind kind, const char *signature) {
      assert(std::strcmp(unit_name, "world") == 0);
      assert(std::strcmp(class_name, "World") == 0);
      assert(kind == AGATE_FOREIGN_METHOD_CLASS);

      switch (gf::hash(signature)) {
        case "move_hero(_)"_id:
          return &Script::moveHero;
        case "move_hero_down()"_id:
          return &Script::moveHeroDown;
        case "move_hero_up()"_id:
          return &Script::moveHeroUp;
        case "post_notification(_)"_id:
          return &Script::postNotification;
        case "add_requirement(_)"_id:
          return &Script::addRequirement;
        case "remove_requirement(_)"_id:
          return &Script::removeRequirement;
        case "add_item(_,_)"_id:
          return &Script::addItem;
        case "add_item_to_inventory(_)"_id:
          return &Script::addItemToInventory;
        case "add_character(_,_)"_id:
          return &Script::addCharacter;
        case "set_character_mood(_,_)"_id:
          return &Script::setCharacterMood;
        case "start_dialog(_)"_id:
          return &Script::startDialog;
        case "attach_dialog_to_character(_,_)"_id:
          return &Script::attachDialogToCharacter;
      }

      return nullptr;
    }

    void vmPrint([[maybe_unused]] AgateVM* vm, const char* text) {
      std::printf("%s", text);
    }

    const char *vmErrorToString(AgateErrorKind kind) {
      switch (kind) {
        case AGATE_ERROR_COMPILE:
          return "Compile error";
        case AGATE_ERROR_RUNTIME:
          return "Runtime error";
        case AGATE_ERROR_STACKTRACE:
          return "Stack";
      }

      assert(false);
      return "???";
    }

    void vmError([[maybe_unused]] AgateVM *vm, AgateErrorKind kind, const char *unit_name, int line, const char *message) {
      gf::Log::error("%s:%d [%s] %s\n", unit_name, line, vmErrorToString(kind), message);
    }

    template<typename T>
    void checkRef(T *data, const char *name) {
      if (data == nullptr) {
        gf::Log::error("A binding failed. Reference was '%s'\n", name);
        std::abort();
      }
    }

  }

  Script::Script(gf::ResourceManager& resources, Akagoria& game)
  : m_resources(resources)
  , m_game(game)
  , m_vm(nullptr)
  , m_classAdventure(nullptr)
  , m_methodInitialize(nullptr)
  , m_methodStart(nullptr)
  , m_methodOnMessage(nullptr)
  , m_methodOnDialog(nullptr)
  {

  }

  Script::~Script() {
    for (auto handle : { m_methodOnDialog, m_methodOnMessage, m_methodStart, m_methodInitialize, m_classAdventure }) {
      if (handle != nullptr) {
        agateReleaseHandle(m_vm, handle);
      }
    }

    if (m_vm != nullptr) {
      agateDeleteVM(m_vm);
    }
  }

  void Script::bind() {
    gf::Path file = m_resources.getAbsolutePath("scripts/adventure.agate");
    std::string script = loadFile(file);
    m_sources.push_back(std::move(script));

    AgateConfig configuration;
    agateConfigInitialize(&configuration);

    configuration.unit_handler = vmUnitHandler;
    configuration.foreign_method_handler = vmForeignMethodHandler;

    configuration.print = vmPrint;
    configuration.error = vmError;

    configuration.user_data = this;

    m_vm = agateNewVM(&configuration);

    AgateStatus status = agateCallString(m_vm, Unit, m_sources.back().c_str());

    if (status != AGATE_STATUS_OK) {
      gf::Log::error("Could not load the main script.\n");
      return;
    }

    agateStackStart(m_vm);
    ptrdiff_t adventureSlot = agateSlotAllocate(m_vm);
    agateGetVariable(m_vm, Unit, "Adventure", adventureSlot);
    m_classAdventure = agateSlotGetHandle(m_vm, adventureSlot);
    agateStackFinish(m_vm);
    assert(m_classAdventure);

    m_methodInitialize = agateMakeCallHandle(m_vm, "initialize()");
    assert(m_methodInitialize);
    m_methodStart = agateMakeCallHandle(m_vm, "start()");
    assert(m_methodStart);
    m_methodOnMessage = agateMakeCallHandle(m_vm, "on_message(_)");
    assert(m_methodOnMessage);
    m_methodOnDialog = agateMakeCallHandle(m_vm, "on_dialog(_)");
    assert(m_methodOnDialog);
  }


  const char *Script::loadModule(gf::Path path) {
    gf::Path file = m_resources.getAbsolutePath("scripts" / path.replace_extension(".agate"));
    std::string script = loadFile(file);
    m_sources.push_back(std::move(script));
    return m_sources.back().c_str();
  }


  void Script::initialize() {
    agateStackStart(m_vm);
    ptrdiff_t arg0 = agateSlotAllocate(m_vm);
    agateSlotSetHandle(m_vm, arg0, m_classAdventure);
    AgateStatus result = agateCallHandle(m_vm, m_methodInitialize);
    agateStackFinish(m_vm);

    if (result != AGATE_STATUS_OK) {
      gf::Log::error("Could not execute 'Adventure.initialize()'\n");
    }
  }

  void Script::start() {
    agateStackStart(m_vm);
    ptrdiff_t arg0 = agateSlotAllocate(m_vm);
    agateSlotSetHandle(m_vm, arg0, m_classAdventure);
    AgateStatus result = agateCallHandle(m_vm, m_methodStart);
    agateStackFinish(m_vm);

    if (result != AGATE_STATUS_OK) {
      gf::Log::error("Could not execute 'Adventure.start()'\n");
    }
  }

  void Script::onMessage(const std::string& name) {
    agateStackStart(m_vm);
    ptrdiff_t arg0 = agateSlotAllocate(m_vm);
    agateSlotSetHandle(m_vm, arg0, m_classAdventure);
    ptrdiff_t arg1 = agateSlotAllocate(m_vm);
    agateSlotSetString(m_vm, arg1, name.c_str());
    AgateStatus result = agateCallHandle(m_vm, m_methodOnMessage);
    agateStackFinish(m_vm);

    if (result != AGATE_STATUS_OK) {
      gf::Log::error("Could not execute 'Adventure.on_message(_)'\n");
    }
  }

  void Script::onDialog(const std::string& name) {
    agateStackStart(m_vm);
    ptrdiff_t arg0 = agateSlotAllocate(m_vm);
    agateSlotSetHandle(m_vm, arg0, m_classAdventure);
    ptrdiff_t arg1 = agateSlotAllocate(m_vm);
    agateSlotSetString(m_vm, arg1, name.c_str());
    AgateStatus result = agateCallHandle(m_vm, m_methodOnDialog);
    agateStackFinish(m_vm);

    if (result != AGATE_STATUS_OK) {
      gf::Log::error("Could not execute 'Adventure.on_dialog(_)'\n");
    }
  }

  void Script::onMessageDefered(const std::string& name) {
    m_messages.push(name);
  }

  void Script::handleDeferedMessages() {
    while (!m_messages.empty()) {
      onMessage(m_messages.front());
      m_messages.pop();
    }
  }

  /*
   * bindings
   */

  void Script::notImplemented([[maybe_unused]] AgateVM* vm) {
    assert(false);
  }

  // move_hero(location)
  void Script::moveHero(AgateVM* vm) {
    const char *locationId = agateSlotGetString(vm, agateSlotForArg(vm, 1));

    DataRef<LocationData> locationRef;
    locationRef.id = gf::hash(locationId);
    locationRef.bind(getData(vm).locations);
    assert(locationRef.data);

    getState(vm).hero.physics.location = locationRef.data->location;
    getState(vm).hero.physics.pushLocation();

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }

  // move_hero_down()
  void Script::moveHeroDown(AgateVM* vm) {
    getState(vm).hero.physics.location.floor -= 2;
    getState(vm).hero.physics.pushFloor();

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }

  // move_hero_up()
  void Script::moveHeroUp(AgateVM* vm) {
    getState(vm).hero.physics.location.floor += 2;
    getState(vm).hero.physics.pushFloor();

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }

  // post_notification(notification)
  void Script::postNotification(AgateVM* vm) {
    const char *notificationId = agateSlotGetString(vm, agateSlotForArg(vm, 1));

    NotificationState notification;
    notification.ref.id = gf::hash(notificationId);
    notification.ref.bind(getData(vm).notifications);
    assert(notification.ref.data);

    getState(vm).notifications.push_back(notification);

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }

  // add_requirement(requirement)
  void Script::addRequirement(AgateVM* vm) {
    const char *requirementId = agateSlotGetString(vm, agateSlotForArg(vm, 1));
    getState(vm).hero.requirements.insert(gf::hash(requirementId));

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }

  // remove_requirement(requirement)
  void Script::removeRequirement(AgateVM* vm) {
    const char *requirementId = agateSlotGetString(vm, agateSlotForArg(vm, 1));
    getState(vm).hero.requirements.erase(gf::hash(requirementId));

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }

  // add_item(item, location)
  void Script::addItem(AgateVM* vm) {
    const char *itemId = agateSlotGetString(vm, agateSlotForArg(vm, 1));
    const char *locationId = agateSlotGetString(vm, agateSlotForArg(vm, 2));

    DataRef<LocationData> locationRef;
    locationRef.id = gf::hash(locationId);
    locationRef.bind(getData(vm).locations);
    checkRef(locationRef.data, locationId);

    ItemState item;
    item.ref.id = gf::hash(itemId);
    item.ref.bind(getData(vm).items);
    checkRef(item.ref.data, itemId);

    item.physics.location = locationRef.data->location;
    item.physics.angle = 0.0f; // TODO
    item.physics.body = getState(vm).physics.createItemBody(item.physics.location, item.physics.angle, item.ref().shape);

    getState(vm).items.push_back(std::move(item));

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }

  // add_item_to_inventory(item)
  void Script::addItemToInventory(AgateVM* vm) {
    const char *itemId = agateSlotGetString(vm, agateSlotForArg(vm, 1));

    DataRef<ItemData> ref;
    ref.id = gf::hash(itemId);
    ref.bind(getData(vm).items);
    checkRef(ref.data, itemId);

    getState(vm).hero.inventory.addItem(ref);

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }

  // add_character(character, location)
  void Script::addCharacter(AgateVM* vm) {
    const char *characterId = agateSlotGetString(vm, agateSlotForArg(vm, 1));
    const char *locationId = agateSlotGetString(vm, agateSlotForArg(vm, 2));

    DataRef<LocationData> locationRef;
    locationRef.id = gf::hash(locationId);
    locationRef.bind(getData(vm).locations);
    checkRef(locationRef.data, locationId);

    CharacterState character;
    character.ref.id = gf::hash(characterId);
    character.ref.bind(getData(vm).characters);
    checkRef(character.ref.data, characterId);

    character.physics.location = locationRef.data->location;
    character.physics.angle = 0.0f; // TODO
    character.physics.body = getState(vm).physics.createCharacterBody(character.physics.location, character.physics.angle);

    gf::Id weapon = character.ref.data->weapon.id;

    if (weapon != gf::InvalidId) {
      character.weapon.ref.id = weapon;
      character.weapon.ref.bind(getData(vm).weapons);
      checkRef(character.weapon.ref.data, "<<weapon>>");
    }

    getState(vm).characters.push_back(std::move(character));

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }

  // set_character_mood(character, mood)
  void Script::setCharacterMood(AgateVM* vm) {
    const char *characterId = agateSlotGetString(vm, agateSlotForArg(vm, 1));
    int mood = agateSlotGetInt(vm, agateSlotForArg(vm, 2));
    assert(mood == 0 || mood == 1);

    CharacterState *character = getCharacter(vm, gf::hash(characterId));
    assert(character != nullptr);
    character->mood = static_cast<CharacterMood>(mood);

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }

  // start_dialog(name)
  void Script::startDialog(AgateVM* vm) {
    const char *dialogId = agateSlotGetString(vm, agateSlotForArg(vm, 1));

    auto& dialog = getState(vm).hero.dialog;
    dialog.ref.id = gf::hash(dialogId);
    dialog.ref.bind(getData(vm).dialogs);
    checkRef(dialog.ref.data, dialogId);

    dialog.currentLine = 0;

    getGame(vm).replaceScene(getGame(vm).worldAct->dialog);

    gf::Log::debug("Dialog '%s' started\n", dialogId);

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }

  // attach_dialog_to_character(dialog, name)
  void Script::attachDialogToCharacter(AgateVM* vm) {
    const char *dialogId = agateSlotGetString(vm, agateSlotForArg(vm, 1));
    const char *characterId = agateSlotGetString(vm, agateSlotForArg(vm, 2));

    gf::Id id = gf::hash(characterId);

    for (auto& character : getState(vm).characters) {
      if (character.ref.id == id) {
        character.dialog.id = gf::hash(dialogId);
        character.dialog.bind(getData(vm).dialogs);
        checkRef(character.dialog.data, dialogId);

        gf::Log::debug("Dialog '%s' attached to '%s'\n", dialogId, characterId);
        break;
      }
    }

    agateSlotSetNil(vm, agateSlotForReturn(vm));
  }


  /*
   * utils
   */
  const WorldData& Script::getData() {
    return m_game.world.data;
  }

  WorldState& Script::getState() {
    return m_game.world.state;
  }

  const WorldData& Script::getData(AgateVM* vm) {
    auto script = static_cast<Script *>(agateGetUserData(vm));
    return script->m_game.world.data;
  }

  WorldState& Script::getState(AgateVM* vm) {
    auto script = static_cast<Script *>(agateGetUserData(vm));
    return script->m_game.world.state;
  }

  Akagoria& Script::getGame(AgateVM* vm) {
    auto script = static_cast<Script *>(agateGetUserData(vm));
    return script->m_game;
  }

  CharacterState *Script::getCharacter(AgateVM* vm, gf::Id id) {
    auto& state = getState(vm);

    for (auto& character : state.characters) {
      if (character.ref.id == id) {
        return &character;
      }
    }

    return nullptr;
  }

}

