#
# Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
#
contextMenu = null

activate = ->
  generatedMenus = []
  contextMenu = atom.contextMenu.add 'atom-text-editor': [label: 'Clang', submenu: generatedMenus]

deactivate = ->
  contextMenu.dispose()
  contextMenu = null

module.exports = {
  activate
  deactivate
}
