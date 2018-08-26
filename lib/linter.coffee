#
# Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
#
debug = require './debug'
scope = require './scope'
util = require './util'

module.exports = {
  name: 'Atom-Clang'
  grammarScopes: scope.grammarScopes
  scope: 'file'
  lintOnFly: false

  lint: (editor) ->
    return [] unless editor.clang?
    return [] unless atom.config.get 'atom-clang.linterEnabled',  scope: editor.getRootScopeDescriptor()

    editor.clang.coalescer.reparse()
      .then (diagnostics) ->
        debug.log 'diagnostics', editor.clang, diagnostics
        diagnostics
      .catch util.showError []
}
