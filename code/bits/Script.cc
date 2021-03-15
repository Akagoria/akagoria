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
#include <gf/Unused.h>

#include "Akagoria.h"
#include "WorldAct.h"
#include "WorldData.h"
#include "WorldState.h"

using namespace gf::literals;

namespace akgr {

  namespace {
    constexpr const char *Module = "Adventure";

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

    WrenLoadModuleResult vmLoadModule(WrenVM* vm, const char* name) {
      auto script = static_cast<Script *>(wrenGetUserData(vm));
      WrenLoadModuleResult res;
      res.source = script->loadModule(name);
      res.onComplete = nullptr;
      res.userData = nullptr;
      return res;
    }

    WrenForeignMethodFn vmBindForeignMethod(WrenVM* vm, const char* module, const char* className, bool isStatic, const char* signature) {
      gf::unused(vm, module, className, isStatic);
      assert(std::strcmp(module, "world") == 0);
      assert(std::strcmp(className, "World") == 0);
      assert(isStatic);

      switch (gf::hash(signature)) {
        case "moveHero(_)"_id:
          return &Script::moveHero;
        case "moveHeroDown()"_id:
          return &Script::moveHeroDown;
        case "moveHeroUp()"_id:
          return &Script::moveHeroUp;
        case "postNotification(_)"_id:
          return &Script::postNotification;
        case "addRequirement(_)"_id:
          return &Script::addRequirement;
        case "removeRequirement(_)"_id:
          return &Script::removeRequirement;
        case "addItem(_,_)"_id:
          return &Script::addItem;
        case "addItemToInventory(_)"_id:
          return &Script::addItemToInventory;
        case "addCharacter(_,_)"_id:
          return &Script::addCharacter;
        case "setCharacterMood(_,_)"_id:
          return &Script::setCharacterMood;
        case "startDialog(_)"_id:
          return &Script::startDialog;
        case "attachDialogToCharacter(_,_)"_id:
          return &Script::attachDialogToCharacter;
      }

      return nullptr;
    }

    void vmWrite(WrenVM* vm, const char* text) {
      gf::unused(vm);
      std::printf("%s", text);
    }

    const char *vmErrorToString(WrenErrorType type) {
      switch (type) {
        case WREN_ERROR_COMPILE:
          return "Compile error";
        case WREN_ERROR_RUNTIME:
          return "Runtime error";
        case WREN_ERROR_STACK_TRACE:
          return "Stack";
      }

      assert(false);
      return "???";
    }

    void vmError(WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message) {
      gf::unused(vm);
      gf::Log::error("%s:%d [%s] %s\n", module, line, vmErrorToString(type), message);
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
        wrenReleaseHandle(m_vm, handle);
      }
    }

    if (m_vm != nullptr) {
      wrenFreeVM(m_vm);
    }
  }

  void Script::bind() {
    gf::Path file = m_resources.getAbsolutePath("scripts/adventure.wren");
    std::string script = loadFile(file);
    m_sources.push_back(std::move(script));

    WrenConfiguration configuration;
    wrenInitConfiguration(&configuration);

    configuration.loadModuleFn = vmLoadModule;
    configuration.bindForeignMethodFn = vmBindForeignMethod;

    configuration.writeFn = vmWrite;
    configuration.errorFn = vmError;

    m_vm = wrenNewVM(&configuration);

    wrenSetUserData(m_vm, this);

    WrenInterpretResult result = wrenInterpret(m_vm, Module, m_sources.back().c_str());

    if (result != WREN_RESULT_SUCCESS) {
      gf::Log::error("Could not load the main script.\n");
      return;
    }

    wrenEnsureSlots(m_vm, 1);
    wrenGetVariable(m_vm, Module, "Adventure", 0);
    m_classAdventure = wrenGetSlotHandle(m_vm, 0);
    assert(m_classAdventure);

    m_methodInitialize = wrenMakeCallHandle(m_vm, "initialize()");
    assert(m_methodInitialize);
    m_methodStart = wrenMakeCallHandle(m_vm, "start()");
    assert(m_methodStart);
    m_methodOnMessage = wrenMakeCallHandle(m_vm, "onMessage(_)");
    assert(m_methodOnMessage);
    m_methodOnDialog = wrenMakeCallHandle(m_vm, "onDialog(_)");
    assert(m_methodOnDialog);
  }


  const char *Script::loadModule(gf::Path path) {
    gf::Path file = m_resources.getAbsolutePath("scripts" / path.replace_extension(".wren"));
    std::string script = loadFile(file);
    m_sources.push_back(std::move(script));
    return m_sources.back().c_str();
  }


  void Script::initialize() {
    wrenEnsureSlots(m_vm, 1);
    wrenSetSlotHandle(m_vm, 0, m_classAdventure);
    WrenInterpretResult result = wrenCall(m_vm, m_methodInitialize);

    if (result != WREN_RESULT_SUCCESS) {
      gf::Log::error("Could not execute 'Adventure::initialize()'\n");
    }
  }

  void Script::start() {
    wrenEnsureSlots(m_vm, 1);
    wrenSetSlotHandle(m_vm, 0, m_classAdventure);
    WrenInterpretResult result = wrenCall(m_vm, m_methodStart);

    if (result != WREN_RESULT_SUCCESS) {
      gf::Log::error("Could not execute 'Adventure::start()'\n");
    }
  }

  void Script::onMessage(const std::string& name) {
    wrenEnsureSlots(m_vm, 2);
    wrenSetSlotHandle(m_vm, 0, m_classAdventure);
    wrenSetSlotString(m_vm, 1, name.c_str());
    WrenInterpretResult result = wrenCall(m_vm, m_methodOnMessage);

    if (result != WREN_RESULT_SUCCESS) {
      gf::Log::error("Could not execute 'Adventure::onMessage(_)'\n");
    }
  }

  void Script::onDialog(const std::string& name) {
    wrenEnsureSlots(m_vm, 2);
    wrenSetSlotHandle(m_vm, 0, m_classAdventure);
    wrenSetSlotString(m_vm, 1, name.c_str());
    WrenInterpretResult result = wrenCall(m_vm, m_methodOnDialog);

    if (result != WREN_RESULT_SUCCESS) {
      gf::Log::error("Could not execute 'Adventure::onDialog(_)'\n");
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

  void Script::notImplemented(WrenVM* vm) {
    assert(false);
    gf::unused(vm);
  }

  // moveHero(location)
  void Script::moveHero(WrenVM* vm) {
    const char *locationId = wrenGetSlotString(vm, 1);

    DataRef<LocationData> locationRef;
    locationRef.id = gf::hash(locationId);
    locationRef.bind(getData(vm).locations);
    assert(locationRef.data);

    getState(vm).hero.physics.location = locationRef.data->location;
    getState(vm).hero.physics.pushLocation();

    wrenSetSlotNull(vm, 0);
  }

  // moveHeroDown()
  void Script::moveHeroDown(WrenVM* vm) {
    getState(vm).hero.physics.location.floor -= 2;
    getState(vm).hero.physics.pushFloor();

    wrenSetSlotNull(vm, 0);
  }

  // moveHeroUp()
  void Script::moveHeroUp(WrenVM* vm) {
    getState(vm).hero.physics.location.floor += 2;
    getState(vm).hero.physics.pushFloor();

    wrenSetSlotNull(vm, 0);
  }

  // postNotification(notification)
  void Script::postNotification(WrenVM* vm) {
    const char *notificationId = wrenGetSlotString(vm, 1);

    NotificationState notification;
    notification.ref.id = gf::hash(notificationId);
    notification.ref.bind(getData(vm).notifications);
    assert(notification.ref.data);

    getState(vm).notifications.push_back(notification);

    wrenSetSlotNull(vm, 0);
  }

  // addRequirement(requirement)
  void Script::addRequirement(WrenVM* vm) {
    const char *requirementId = wrenGetSlotString(vm, 1);
    getState(vm).hero.requirements.insert(gf::hash(requirementId));

    wrenSetSlotNull(vm, 0);
  }

  // removeRequirement(requirement)
  void Script::removeRequirement(WrenVM* vm) {
    const char *requirementId = wrenGetSlotString(vm, 1);
    getState(vm).hero.requirements.erase(gf::hash(requirementId));

    wrenSetSlotNull(vm, 0);
  }

  // addItem(item, location)
  void Script::addItem(WrenVM* vm) {
    const char *itemId = wrenGetSlotString(vm, 1);
    const char *locationId = wrenGetSlotString(vm, 2);

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
    item.physics.body = getState(vm).physics.createItemBody(item.physics.location, item.physics.angle, item.ref.data->shape);

    getState(vm).items.push_back(std::move(item));

    wrenSetSlotNull(vm, 0);
  }

  // addItemToInventory(item)
  void Script::addItemToInventory(WrenVM* vm) {
    const char *itemId = wrenGetSlotString(vm, 1);

    DataRef<ItemData> ref;
    ref.id = gf::hash(itemId);
    ref.bind(getData(vm).items);
    checkRef(ref.data, itemId);

    getState(vm).hero.inventory.addItem(ref);

    wrenSetSlotNull(vm, 0);
  }

  // addCharacter(character, location)
  void Script::addCharacter(WrenVM* vm) {
    const char *characterId = wrenGetSlotString(vm, 1);
    const char *locationId = wrenGetSlotString(vm, 2);

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

    gf::Id weapon = character.ref.data->weapon;

    if (weapon != gf::InvalidId) {
      character.weapon.ref.id = weapon;
      character.weapon.ref.bind(getData(vm).weapons);
      checkRef(character.weapon.ref.data, "<<weapon>>");
    }

    getState(vm).characters.push_back(std::move(character));

    wrenSetSlotNull(vm, 0);
  }

  // setCharacterMood(character, mood)
  void Script::setCharacterMood(WrenVM* vm) {
    const char *characterId = wrenGetSlotString(vm, 1);
    int mood = wrenGetSlotDouble(vm, 2);
    assert(mood == 0 || mood == 1);

    CharacterState *character = getCharacter(vm, gf::hash(characterId));
    assert(character != nullptr);
    character->mood = static_cast<CharacterMood>(mood);

    wrenSetSlotNull(vm, 0);
  }

  // startDialog(name)
  void Script::startDialog(WrenVM* vm) {
    const char *dialogId = wrenGetSlotString(vm, 1);

    auto& dialog = getState(vm).hero.dialog;
    dialog.ref.id = gf::hash(dialogId);
    dialog.ref.bind(getData(vm).dialogs);
    checkRef(dialog.ref.data, dialogId);

    dialog.currentLine = 0;

    getGame(vm).replaceScene(getGame(vm).worldAct->dialog);

    gf::Log::debug("Dialog '%s' started\n", dialogId);

    wrenSetSlotNull(vm, 0);
  }

  // attachDialogToCharacter(dialog, name)
  void Script::attachDialogToCharacter(WrenVM* vm) {
    const char *dialogId = wrenGetSlotString(vm, 1);
    const char *characterId = wrenGetSlotString(vm, 2);

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

    wrenSetSlotNull(vm, 0);
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

  const WorldData& Script::getData(WrenVM* vm) {
    auto script = static_cast<Script *>(wrenGetUserData(vm));
    return script->m_game.world.data;
  }

  WorldState& Script::getState(WrenVM* vm) {
    auto script = static_cast<Script *>(wrenGetUserData(vm));
    return script->m_game.world.state;
  }

  Akagoria& Script::getGame(WrenVM* vm) {
    auto script = static_cast<Script *>(wrenGetUserData(vm));
    return script->m_game;
  }

  CharacterState *Script::getCharacter(WrenVM* vm, gf::Id id) {
    auto& state = getState(vm);

    for (auto& character : state.characters) {
      if (character.ref.id == id) {
        return &character;
      }
    }

    return nullptr;
  }

}

