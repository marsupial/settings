#
# Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
#

#
# parse/reparse is expensive. we need to coalesce parse/reparse calls.
# if a reparse is pending (awaiting some parse or reparse to finish),
# and a full parse request comes in, we would want to replace the pending
# reparse with a full parse. the opposite can be said if a parse is pending,
# and a reparse comes in, we would just continue to leave the parse pending.
#

class ParseCoalescer
  constructor: (@_parse, @_reparse) ->
    @_active = null
    @_pending = {
      func: null
      args: null
      promise: null
    }

  parse: (args...) ->
    return @_invokeActive @_parse, args if not @_active?

    # there is an active promise... need queue up this parse (override any pending @_reparse)
    @_pending.func = @_parse
    @_pending.args = args

    # if there is already a promise, just return that
    return @_pending.promise if @_pending.promise?

    # we need to make a new promise
    @_pending.promise = new Promise (fulfill, reject) =>
      @_active
        .then => @_invokePending fulfill, reject

  reparse: (args...) ->
    return @_invokeActive @_reparse, args if not @_active?

    # if nothing is pending yet, set the pending func
    @_pending.func = @_reparse if @_pending.func is null

    # there is an active promise... if pending is also a reparse, update args, otherwise, leave parse alone
    @_pending.args = args if @_pending.func is @_reparse

    # if there is already a promise, just return that
    return @_pending.promise if @_pending.promise?

    # we need to make a new promise
    @_pending.promise = new Promise (fulfill, reject) =>
      @_active
        .then => @_invokePending fulfill, reject

  _invokeActive: (func, args) ->
    @_active = new Promise (fulfill, reject) =>
      func.apply @this, args
        .then (val) =>
          @_active = null
          fulfill val
        .catch (err) => # is this catch and try/catch even needed?
          @_active = null
          reject err

  _invokePending: (fulfill, reject) ->
    @_active = @_pending.func.apply @this, @_pending.args
    @_pending.func = null
    @_pending.args = null
    @_pending.promise = null
    @_active
      .then (val) =>
        @_active = null
        fulfill val
      .catch (err) =>
        @_active = null
        reject err

module.exports = {
  ParseCoalescer
}
