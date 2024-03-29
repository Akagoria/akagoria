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

#include <gf/Id.h>
#include <gf/Path.h>
#include <gf/ResourceManager.h>

#include <agate.h>

namespace akgr {
  struct Akagoria;

  struct WorldData;
  struct WorldState;

  struct CharacterState;

  class Script {
  public:
    Script(gf::ResourceManager& resources, Akagoria& game);
    ~Script();

    Script(const Script&) = delete;
    Script& operator=(const Script&) = delete;

    Script(Script&&) = delete;
    Script& operator=(Script&&) = delete;

    void bind();

    const WorldData& getData();
    WorldState& getState();

    const char *loadModule(gf::Path path);

    void initialize();
    void start();
    void onMessage(const std::string& name);
    void onDialog(const std::string& name);

    void onMessageDefered(const std::string& name);
    void handleDeferedMessages();

  public:
    static void notImplemented(AgateVM* vm);

    /*
     * hero
     */

    static void moveHero(AgateVM* vm);
    static void moveHeroDown(AgateVM* vm);
    static void moveHeroUp(AgateVM* vm);


    /*
     * notifications
     */

    static void postNotification(AgateVM* vm);

    /*
     * requirements
     */

    static void addRequirement(AgateVM* vm);
    static void removeRequirement(AgateVM* vm);

    /*
     * items
     */

    static void addItem(AgateVM* vm);
    static void addItemToInventory(AgateVM* vm);

    /*
     * characters
     */

    static void addCharacter(AgateVM* vm);
    static void setCharacterMood(AgateVM* vm);

    /*
     * dialog
     */

    static void startDialog(AgateVM* vm);
    static void attachDialogToCharacter(AgateVM* vm);

  private:
    static const WorldData& getData(AgateVM* vm);
    static WorldState& getState(AgateVM* vm);
    static Akagoria& getGame(AgateVM* vm);

    static CharacterState *getCharacter(AgateVM* vm, gf::Id id);

  private:
    gf::ResourceManager& m_resources;
    Akagoria& m_game;

    AgateVM *m_vm;
    AgateHandle *m_classAdventure;
    AgateHandle *m_methodInitialize;
    AgateHandle *m_methodStart;
    AgateHandle *m_methodOnMessage;
    AgateHandle *m_methodOnDialog;

    std::queue<std::string> m_messages;

    std::vector<std::string> m_sources;
  };

}

#endif // AKGR_SCRIPT_H
