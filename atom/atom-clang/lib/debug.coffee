#
# Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
#

assert = console.assert.bind console
error = console.error.bind console
info = if atom.inDevMode() then console.info.bind console else ->
log = if atom.inDevMode() then console.log.bind console else ->
time = if atom.inDevMode() then console.time.bind console else ->
timeEnd = if atom.inDevMode() then console.timeEnd.bind console else ->
trace = if atom.inDevMode() then console.trace.bind console else ->
warn = if atom.inDevMode() then console.warn.bind console else ->

module.exports = {
  assert
  error
  info
  log
  time
  timeEnd
  trace
  warn
}
