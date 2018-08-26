crypto = require 'crypto'
os = require 'os'
path = require 'path'

commands = require './lib/commands'
config = require './lib/config'
events = require './lib/events'
linter = require './lib/linter'
provider = require './lib/provider'

{clangDebug, clangVersion} = require('bindings') 'clang.node'

# logging inside the C++ libclang node module is a pain
# generate a random log file in the OS's tmp directory
loggingUUID = crypto.randomBytes(4).toString 'hex'

loggingPath = ->
  path.resolve os.tmpdir(), "CLANG-#{loggingUUID}.log"

activate = ->
  require 'atom-package-deps'
    .install 'atom-clang'
    .then ->
      console.log 'atom-clang', clangVersion()
      if atom.inDevMode()
        console.log 'atom-clang', loggingPath()
        clangDebug loggingPath()
      commands.activate()
      events.activate()

deactivate = ->
  commands.deactivate()
  events.deactivate()

module.exports = {
  config
  provide: -> provider
  provideLinter: -> linter
  activate
  deactivate
}
