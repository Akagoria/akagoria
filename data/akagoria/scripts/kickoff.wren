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
import "constants" for Mood

class Kickoff {

  static initialize(W) {
    W.moveHero("Start")
    W.addRequirement("IntroDialogReq")
    W.addCharacter("Shagir", "Shagir")
    W.attachDialogToCharacter("ShagirConversation0", "Shagir")
    // W.postNotification("Welcome")

    W.addCharacter("Ling", "Ling")
    W.setCharacterMood("Ling", Mood.Angry)

    W.addItem("GemRuby", "Ruby #1")
    W.addItem("GemEmerald", "Emerald #1")
    W.addItemToInventory("GemEmerald")
    W.addItemToInventory("GemRuby")
  }

}
