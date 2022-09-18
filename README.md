# Rook frenzy test project

## Introduction
This is an implementation of a test task. The task is as follows:

``` text
5 rooks are placed randomly on an 8x8 chessboard.
Rooks start moving simultaneously to random positions on the board (either vertically or horizontally. The move is considered to be performed instantly).
Every other move rook pauses for 200-300 ms. If another figure is blocking the way, rook waits until path clears. If path is blocked for 5 seconds another move is chosen randomly.
Movement ends when each rook makes 50 moves.
Output initial and final rook positions on the chessboard.

The solution should work under Ubuntu x86_64. Use stdout for IO.
```

## Installation

``` text
git clone https://github.com/tamanrasset/rook-frenzy.git
cd rook-frenzy
cmake
make
./RookFrenzy
```

If done correctly, the output should be similar to the following:
``` text
Initial position:
|__|__|__|__|__|__|R3|__|
|__|__|__|__|__|__|__|__|
|__|__|__|__|__|__|__|__|
|__|__|__|__|__|__|__|__|
|__|__|R0|__|__|__|R2|__|
|__|__|__|__|__|__|__|__|
|__|__|__|__|R4|__|__|__|
|__|__|__|__|__|__|__|R1|

Simulation started. Please wait until done...

Final position:
|__|__|__|__|__|__|__|__|
|__|__|__|__|__|__|__|__|
|__|__|R2|__|__|__|__|R1|
|__|R0|__|__|__|__|__|__|
|__|__|__|__|__|__|__|__|
|__|__|__|__|__|__|__|__|
|R3|__|__|__|__|R4|__|__|
|__|__|__|__|__|__|__|__|

```

## Enable diagnostic logging

Since it's a real-time simulation, the process might take a while. You can enable logging to console by uncommenting the following line in CMakeLists.txt:

```text
add_definitions(-DENABLE_LOGGING)
```
then recomplie and re-run the app.
Now the output should contain some diagnostic info i.e.:
```text
...
1663517810231 Rook4: (step 42) moved from 30 to 38
1663517810231 Rook4: Position 54 is blocked
1663517812863 Rook3: Failed to move to 62
1663517812863 Rook3: (step 23) moved from 57 to 49
1663517812863 Rook2: (step 24) moved from 59 to 57
1663517812863 Rook2: Position 41 is blocked
1663517813068 Rook3: (step 24) moved from 49 to 25
1663517813068 Rook3: (step 25) moved from 25 to 49
1663517813094 Rook1: Failed to move to 57
1663517813094 Rook1: (step 26) moved from 61 to 5
...
```