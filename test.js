var addon = require('bindings')('addon');

// TODO write new tests

const table = new addon.Table({ ante: 0, smallBlind: 10, bigBlind: 20 })
console.log(table.forcedBets())

table.sitDown(0, 1000)
table.sitDown(1, 1000)

table.startHand()
console.log(table.legalActions())
