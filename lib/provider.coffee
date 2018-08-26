#
# Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
#
debug = require './debug'
scope = require './scope'
util = require './util'

# prefix match, want to trigger completions on . -> :: and words matching minimum word length
# FWIW: testing showed regex.exec(string) faster than string.match(regex)...

symbol_prefix = /(\.|->|::)\w*$/
word_prefix = /\w+$/

#
# https://github.com/atom/autocomplete-plus/wiki/Provider-API
#

module.exports = {
  selector: scope.selectors()
  inclusionPriority: 1
  suggestionPriority: 1
  excludeLowerPriority: true
  filterSuggestions: true

  getSuggestions: ({ editor, bufferPosition, scopeDescriptor, prefix, activatedManually }) ->
    return null unless editor.clang?

    line = editor.getTextInRange [[bufferPosition.row, 0], bufferPosition]
    triggered = symbol_prefix.exec(line)?
    prefix = word_prefix.exec(line)?[0] or ''
    minimumWordLength = atom.config.get 'autocomplete-plus.minimumWordLength', scope: editor.getRootScopeDescriptor()
    deprecated = atom.config.get 'atom-clang.includeDeprecated', scope: editor.getRootScopeDescriptor()

    debug.log \
      'completions',
      editor.clang,
      bufferPosition,
      triggered,
      "[#{prefix}]",
      minimumWordLength,
      deprecated,
      activatedManually

    # only attempt completions if activated manually or prefix > minimum setting
    return null if not activatedManually and not triggered and prefix.length < minimumWordLength

    completions = editor.clang.completions \
      deprecated,
      prefix,
      bufferPosition.row,
      bufferPosition.column - prefix.length

    debug.log 'completions', completions
    completions
}
