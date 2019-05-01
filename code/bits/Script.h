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
#ifndef AKGR_SCRIPT_H
#define AKGR_SCRIPT_H

#include <string>
#include <queue>

#include <gf/Path.h>
#include <gf/ResourceManager.h>

#include <wren.hpp>

namespace akgr {

  struct WorldData;
  struct WorldState;

  class Script {
  public:
    Script(gf::ResourceManager& resources, const akgr::WorldData& data, akgr::WorldState& state);
    ~Script();

    Script(const Script&) = delete;
    Script& operator=(const Script&) = delete;

    Script(Script&&) = delete;
    Script& operator=(Script&&) = delete;

    void bind();

    const akgr::WorldData& getData() {
      return m_data;
    }

    akgr::WorldState& getState() {
      return m_state;
    }

    char *loadModule(gf::Path path);

    void initialize();
    void start();
    void onMessage(const std::string& name);
    void onDialog(const std::string& name);

    void onMessageDefered(const std::string& name);
    void handleDeferedMessages();

  public:
    static void notImplemented(WrenVM* vm);

    /*
     * hero
     */

    static void moveHero(WrenVM* vm);
    static void moveHeroDown(WrenVM* vm);
    static void moveHeroUp(WrenVM* vm);


    /*
     * notifications
     */

    static void postNotification(WrenVM* vm);

    /*
     * requirements
     */

    static void addRequirement(WrenVM* vm);
    static void removeRequirement(WrenVM* vm);

    /*
     * items
     */

    static void addItem(WrenVM* vm);
    static void addItemToInventory(WrenVM* vm);

    /*
     * characters
     */

    static void addCharacter(WrenVM* vm);

    /*
     * dialog
     */

    static void startDialog(WrenVM* vm);
    static void attachDialogToCharacter(WrenVM* vm);

  private:
    static const akgr::WorldData& getData(WrenVM* vm);
    static akgr::WorldState& getState(WrenVM* vm);

  private:
    gf::ResourceManager& m_resources;
    const akgr::WorldData& m_data;
    akgr::WorldState& m_state;

    WrenVM *m_vm;
    WrenHandle *m_classAdventure;
    WrenHandle *m_methodInitialize;
    WrenHandle *m_methodStart;
    WrenHandle *m_methodOnMessage;
    WrenHandle *m_methodOnDialog;

    std::queue<std::string> m_messages;
  };

}

#endif // AKGR_SCRIPT_H
