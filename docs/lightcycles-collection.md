--
layout: page
title: "Lightcycles Collection | Trivial Stuff"
permalink: /lightcycles-collection/
--

# Lightcycles Collection
A basic multiplayer game similar to snake.

## Comparison between versions

**Version** | **[Lightcycles](#lightcycles)** | **[Lightcycles++](#lightcyclesplusplus)** | **[Command-line <br> Lightcycles](#lightcycles-cmd)**
:---|:---|:---|:---|
Number of players: | 2 or 4 | 2 or 4 | 2 or 4 |
Control scheme: | WASD<br>Arrow Keys<br>Numpad 8456<br>UHJK | WASD<br>Arrow Keys<br>Numpad 8456<br>UHJK |WASD<br>Arrow Keys<br>Numpad 8456<br>UHJK |
Game can be paused? | No | Yes | No |
Fullscreen mode? | No | Yes | Window can be <br>maximised manually | 
Post game scoreboard? | No | Yes, but only when <br> window is square | No |
Game does not start instantly? | No | Yes | Yes |
Dark colour scheme? | Yes | Yes | No |
Light colour scheme? | Yes, but why would<br>you want to use it? | No | No |
Method of changing settings: | Editing variables in<br>Lightcycles.py | Command line<br>arguments | Actual settings <br> menu |
Language written in: | Python | C++ | C++ |
Dependencies: | Pygame | SDL2<br>SDL_ttf | Windows |
Relative file size: | Quite Small | Quite Large | Smallish |

## [Lightcycles](../lightcycles "Go to source") <a name="lightcycles"></a>
Written in Python with [Pygame], this is the original version.

```python
    controls  = [[100 , 97, 119, 115], #WASD        - Blue
                 [275, 276, 273, 274], #ARROW KEYS  - Orange
                 [262, 260, 264, 261], #NUMPAD 8456 - Green
                 [107, 104, 117, 106]] #UHJK        - Purple
```
To change settings, edit Lightcycles.py

```python
    ### Game settings     ###
    player_count  = 4       # Normally 2 but could be 4. 3 is unfair, and 1 is snake without food.
    style_name    = "dark"  # Can be either "dark" or "lght".
    destroy_trail = False   # Can be either True or False.
    scale         = 8       # Default = 8    NOTE: scale * grid should be smaller than the monitor resolution.
    grid          = 100     # Default = 100  NOTE: Ideally an even number.
    midway        = grid/2  # Default = 50   NOTE: Should be the midpoint of grid.
    tick_speed    = 30      # Default = 30   NOTE: Larger grids should have higher tick_speed.
    ### Game settings END ###
```
The infinite launch script exists to automatically reopen lightcyles after a game has been won.

[Pygame]:https://www.pygame.org/wiki/GettingStarted

## [Lightcycles++](../lightcyclesplusplus "Go to source") <a name="lightcyclesplusplus"></a>
The remake of Lightcycles.py, in C++ using [SDL2] and [SDL_ttf].

### Features
* Supports 2 or 4 players (3 players is possible but not fair).
* Changeable settings via command line arguments for advanced users.
* Scoreboard for users of square windows.
* Can be run fullscreen if set to do so.

Note: a reasonably sized keyboard will be required.

### Help
```
General Controls

SPACEBAR  to start/continue
BACKSPACE to pause/unpause
ESCAPE    to exit (same as closing with red X)

Player Controls

PLAYER 1 (blue)   use     W A S D
PLAYER 2 (orange) use     ARROW KEYS
PLAYER 3 (green)  use     NUMPAD 8 4 5 6
PLAYER 4 (purple) use     U H J K


Use @ in Command Line Arguments to enable fullscreen, and set a suitable grid size and
scale for the game. This overrides any custom grid sizes, scales and window modes.


Command Line Arguments (most prefixed with -)

To set grid dimensions:
Use -x[uint] to set the width of the grid  (default 100)
    -y[uint] to set the height of the grid (default 100)
    -z[uint] to set both the width and height of the grid at the same time.

To set grid scale:
Use -g[uint] set pixels per square in the grid (default 8)

To set game speed:
Use -s[uint] to set delay between each frame (in ms) (default 30)

To set number of games to play to:
Use -w[uint] to set number of games to play to	(default 3)

To set number of players:
Use -p[uint] (max 4, min 1, default 4)
Note: only 2 or 4 players should be used

To enable flash upon player death:
Use -f[uint] to set number of frames to display flash for (default 0)


To enable certain modes: (all default false)
Use -n[bool] to reduce menus. 				
Use -d[bool] to delete trails upon player death
Use -l[bool] to relaunch the game on closing

To set window mode:
Use -m[uint] (default 0)
Where the uint entered corresponds to a window mode
    0 Normal Window
    1 Borderless Window
    2 Fullscreen

To set target grid scale for fullscreen:
Use -a[uint] (default 10)
Note: The target scale will not always be used, but the actual scale always will be less
```


[SDL2]:https://www.libsdl.org/index.php
[SDL_ttf]:https://www.libsdl.org/projects/SDL_ttf/

## [Command-line Lightcycles](../lightcycles-cmd "Go to source") <a name="lightcycles-cmd"></a>

A version of lightcycles made from recycling code from
[Snake](https://github.com/wildp/trivial-stuff/blob/master/README.md#snake-)
and the collision detection from Lightcycles++. It runs on Windows only,
and uses the same controls as the other games

