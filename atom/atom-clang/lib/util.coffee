#
# Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
#

# if an error occurs, show it to the user..
showError = (result) ->
  (err) ->
    console.error err
    atom.notifications.addError "Atom-Clang: #{err.name}", detail: err.stack
    result

module.exports = {
  showError
}
