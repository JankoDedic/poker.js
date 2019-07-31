var addon = require('bindings')('addon');

// TODO write new tests

const table = new addon.Table({ ante: 0, smallBlind: 10, bigBlind: 20 })
console.log(table.forcedBets())
console.log(table.handPlayers())

// try {
//   console.log(table.handPlayers())
// } catch (e) {
//   console.log(e)
// }
