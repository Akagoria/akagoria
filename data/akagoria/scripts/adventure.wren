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

import "world" for World
import "chapter1" for Chapter1

class Adventure {

  /*
   * Interfaces
   */

  static initialize() {
    __messages = { }
    __dialogs = { }

    // special messages

    Adventure.addMessageCallback("MoveDown") {
      System.print("[adventure.wren] MoveDown")
      World.moveHeroDown()
    }

    Adventure.addMessageCallback("MoveUp") {
      System.print("[adventure.wren] MoveUp")
      World.moveHeroUp()
    }

    // chapter initialization

    Chapter1.initialize(Adventure, World)
  }

  static start() {
    World.moveHero("Start")
    World.addRequirement("IntroDialogReq")
    World.addCharacter("Shagir", "Shagir")
    World.attachDialogToCharacter("ShagirConversation0", "Shagir")
    World.postNotification("Welcome")

    World.addItem("GemRuby", "Ruby #1")
  }

  static onMessage(message) {
    var callback = __messages[message]

    if (callback) {
      System.print("[adventure.wren] Message '%(message)' sent")
      callback.call()
    } else {
      System.print("[adventure.wren] No callback for message '%(message)'")
    }
  }

  static onDialog(dialog) {
    var callback = __dialogs[dialog]

    if (callback) {
      System.print("[adventure.wren] Dialog '%(dialog)' finished")
      callback.call()
    } else {
      System.print("[adventure.wren] No callback for dialog '%(dialog)'")
    }
  }

  /*
   * Others
   */

  static addMessageCallback(message, fn) {
    __messages[message] = fn
  }

  static addDialogCallback(dialog, fn) {
    __dialogs[dialog] = fn
  }
}
