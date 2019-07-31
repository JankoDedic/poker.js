# pokerjs

**pokerjs** is a high-performance native Node module based on the [poker library](https://github.com/JankoDedic/poker) written in C++ for simulating poker gameplay. It can be used for running poker games with real players.

## Installation

This is a native Node module and needs to be compiled with a C++17 compiler.

For Windows: If you don't have Visual Studio installed, you can install just [Build Tools for Visual Studio 2019](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019).

For macOS/Linux: You will have to have GCC 8 or Clang 7 installed to compile the module.

You can install the package from this repository:

```
npm install JankoDedic/pokerjs
```

## Usage

The library provides a single class: `poker.Table`. The created table object represents a state machine and models a real-world poker table. Methods operating on it have documented preconditions. Precondition violation will lead to an error being thrown. Poker table state is not guaranteed to be valid after this happens. Common usage looks something like this:

```javascript
const poker = require('pokerjs')

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

### poker.Table(forcedBets)

Creates an instance of the poker table object.

Parameters:
    * `[Object] forcedBets` - bet structure of the table
        * `[Number] ante` - ante
        * `[Number] smallBlind` - small blind
        * `[Number] bigBlind` - big blind

## License

This project is licensed under the MIT license. See [LICENSE](LICENSE).
