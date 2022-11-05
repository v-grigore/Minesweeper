# Minesweeper
To run the game, use a command line compiler.

Running the game on Ubuntu:

Install g++ compiler, compile the source code and then run the game using the following commands:
```
$ sudo apt-get install g++

$ g++ minesweeper.cpp -o minesweeper

$ ./minesweeper
```
#
This is a CLI implementation of the popular game Minesweeper, with all its classic features and a scoring system that saves the highscores locally. To reset the data, simply delete the "scoreboard" file.

```

  __  __ _____ _   _ ______  _______          ________ ______ _____  ______ _____
 |  \/  |_   _| \ | |  ____|/ ____\ \        / /  ____|  ____|  __ \|  ____|  __ \
 | \  / | | | |  \| | |__  | (___  \ \  /\  / /| |__  | |__  | |__) | |__  | |__) |
 | |\/| | | | | . ` |  __|  \___ \  \ \/  \/ / |  __| |  __| |  ___/|  __| |  _  /
 | |  | |_| |_| |\  | |____ ____) |  \  /\  /  | |____| |____| |    | |____| | \ \
 |_|  |_|_____|_| \_|______|_____/    \/  \/   |______|______|_|    |______|_|  \_\



The board is divided into cells, with mines randomly distributed. To win, you need
to open all the cells. The number on a cell shows the number of mines adjacent to
it. Using this information, you can determine cells that are safe, and cells that
contain mines. Cells can be opened with the {open} or {o} command, or they can
be flagged/unflagged with the {flag} or {f} command (without brackets). Commands
have to be structured: [action] [y-coordonate] [x-coordonate].

Example: "flag 4 6" will flag the cell with the coordinates 4 6;

Game board example:
    ___________
 6 |1|*|*|1| | | <---Cell with no adjacent mines. Automatically uncovers all
 5 |1|2|2|1|1| |     adjacent cells when opened.
 4 | | | | |1|#| <---Flagged cell. Flagged cells can't be opened.
 3 | | | | |1|1| <---Numbered cells indicate the number of adjacent mines.
 2 |1|2|1|1|1|*| <---Mine. The game is lost when you open the first mine.
 1 |.|.|.|1|1|.| <---Covered cell.
    1   3   5
      2   4   6     3 mines

As you can see , the (3,6) cell has been flagged.

Winning a standard difficulty game will earn you a score based on difficulty
and time completion. The scoring system does not support custom difficulty at the
moment.

```
