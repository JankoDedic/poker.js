# poker.js

**poker.js** is a high-performance native Node module based on the [poker library](https://github.com/JankoDedic/poker) written in C++ for simulating poker gameplay. It can be used for running poker games with real players.

## Installation

This is a native Node module and needs to be compiled with a C++17 compiler.

For Windows: If you don't have Visual Studio installed, you can install just [Build Tools for Visual Studio 2019](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019).

For macOS/Linux: You will have to have GCC 8 or Clang 7 installed to compile the module.

You can install the package from this repository:

```
npm install poker
```

## Usage

The library provides a single class: `poker.Table`. The created table object represents a state machine and models a real-world poker table. Methods operating on it have documented preconditions. Precondition violation will lead to an error being thrown. Poker table state is not guaranteed to be valid after this happens. Common usage looks something like this:

```javascript
const poker = require('poker')

const table = new poker.Table({
    ante: 0,
    smallBlind: 10,
    bigBlind: 20
})

table.sitDown(0, 1000); // seat a player at seat 0 with 1000 chips buy-in
table.sitDown(2, 1500); // seat a player at seat 2 with 1500 chips buy-in
table.sitDown(5, 1700); // seat a player at seat 5 with 1700 chips buy-in

table.startHand()
while (table.isHandInProgress()) {
    while (table.isBettingRoundInProgress()) {
        table.actionTaken(getPlayerAction())
    }
    table.endBettingRound()
}
table.showdown()

// ad infinitum...
```

## API reference

### `poker.Table(forcedBets)`

Creates an instance of the poker table object.

Parameters:
- `[Object] forcedBets` - bet structure of the table
    - `[Number] ante` - ante
    - `[Number] smallBlind` - small blind
    - `[Number] bigBlind` - big blind

### `poker.Table.prototype.seats() => Array<Object>`

Returns the state of the players seated at the table.

### `poker.Table.prototype.forcedBets() => Object`

Returns the current bet structure at the table.

### `poker.Table.prototype.isHandInProgress() => Boolean`

Returns `true` if hand is in progress.

### `poker.Table.prototype.isBettingRoundInProgress() => Boolean`

Returns `true` if betting round is in progress.

### `poker.Table.prototype.areBettingRoundsCompleted() => Boolean`

Returns `true` if all betting rounds are completed.

### `poker.Table.prototype.handPlayers() => Array<Object>`

Returns the state of the players currently in the hand.

### `poker.Table.prototype.button() => Number`

Returns the seat index of the button.

### `poker.Table.prototype.playerToAct() => Number`

Returns the seat index of the player to act.

### `poker.Table.prototype.numActivePlayers() => Number`

Returns the number of active players in the active hand.

### `poker.Table.prototype.pots() => Array<Object>`

Returns the state of all pots in the active hand.

### `poker.Table.prototype.roundOfBetting() => String`

Returns the current round of betting (`preflop`|`flop`|`turn`|`river`).

### `poker.Table.prototype.communityCards() => Array<Object>`

Returns the community cards for the active hand.

### `poker.Table.prototype.automaticActions() => Array<Object>`

Returns the toggled automatic actions at the table.

### `poker.Table.prototype.canSetAutomaticActions(seatIndex) => Boolean`

Returns `true` if the player with a given `seatIndex` can set an automatic action.

Parameters:
- `[Number] seatIndex` - seat index of a player

### `poker.Table.prototype.legalAutomaticActions(seatIndex) => Array<String>`

Returns an array of legal automatic actions for a player with a given `seatIndex`.

Parameters:
- `[Number] seatIndex` - seat index of a player

### `poker.Table.prototype.setForcedBets(forcedBets)`

Modifies the bet structure of the table.

Parameters:
- `[Object] forcedBets` - bet structure of the table
    - `[Number] ante` - ante
    - `[Number] smallBlind` - small blind
    - `[Number] bigBlind` - big blind

### `poker.Table.prototype.sitDown(seatIndex, buyIn)`

Indicate that a player took a given seat with a given buy-in.

Parameters:
- `[Number] seatIndex` - index of an unoccupied seat
- `[Number] buyIn` - amount of chips the player sat down with

### `poker.Table.prototype.standUp(seatIndex)`

Indicate that a player has left the table.

Parameters:
- `[Number] seatIndex` - index of an occupied seat

### `poker.Table.prototype.startHand()`

Start a new hand by collecting ante, placing blinds and dealing cards.

### `poker.Table.prototype.actionTaken(action[, betSize])`

Indicate that the player to act has taken an action.

Parameters:
- `[String] action` - type of action (`fold`|`check`|`call`|`bet`|`raise`)
- `[Number] betSize` - the size of the bet (if action is aggressive)

### `poker.Table.prototype.endBettingRound()`

End the current betting round which is no longer in progress. Collect the bets and form the pots.

### `poker.Table.prototype.showdown()`

Perform a showdown. Evaluate the players' hands and pay the winners.

### `poker.Table.prototype.setAutomaticAction(seatIndex, automaticAction)`

Set the automatic action for a given player.

Parameters:
- `[Number] seatIndex` - seat index of a player
- `[String] automaticAction` - type of automatic action (`fold`|`check/fold`|`check`|`call`|`call any`|`all-in`)

## License

This project is licensed under the MIT license. See [LICENSE](LICENSE).
