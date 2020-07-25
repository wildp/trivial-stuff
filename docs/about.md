--
layout: page
title: "About | Trivial Stuff"
permalink: /
--

# What's here?
A small collection of simple games and other programs.

1. [Bitmap Displayer](#bitmap-displayer)
1. [Console Minesweeper](#console-minesweeper)
1. [Lightcycles](#lightcycles)
1. [Lightcycles CMD](#lightcycles-cmd)
1. [Lightcycles++](#lightcyclesplusplus)
1. [Pong Clone](#pong-clone)
1. [Snake](#snake-cmd)
1. [Wiki Hangman](#wiki-hangman)


## Bitmap Displayer <a name="bitmap-displayer"></a>
![Bitmap Displayer](docs/images/bitmap-displayer-1.png?raw=true "Bitmap Displayer")

A program to display bitmap images on the command-line.
It only supports bitmaps with the `BITMAPINFOHEADER` bitmap header (40 byte header), and requires a console
which supports ANSI escape sequences.

For Windows 10, the program requires
[Windows Insiders build 14931](https://blogs.msdn.microsoft.com/commandline/2016/09/22/24-bit-color-in-the-windows-console/)
or later.

## Console Minesweeper <a name="console-minesweeper"></a>

![Console Minesweeper](images/console-minesweeper-3.png?raw=true "Console Minesweeper")

A text based minesweeper game, originally for the windows command prompt. For instructions, see [this help page][console-minesweeper-help].

## Lightcycles <a name="lightcycles"></a>

A local multiplayer game like Snake, written in Python with Pygame. See [this page][lightcycles-collection]
for more details and comparison of different versions.

## Lightcycles CMD <a name="lightcycles-cmd"></a>

An edited version of [Snake](#snake-cmd) with some improvements to rendering on the console.
Only works on Windows.

## Lightcycles++ <a name="lightcyclesplusplus"></a>

A remake of [Lightcycles](#lightcycles), written in C++ with SDL2. See [this page][lightcycles-collection] for more details.

## Pong Clone <a name="pong-clone"></a>

![Pong Clone](images/pong-clone-2.png?raw=true "Pong Clone")

A two player pong clone written in Python with Pygame. See [this page][pong-help] for more details.

## Snake <a name="snake-cmd"></a>
![Snake](images/snake-1.png?raw=true "Snake")

**Snake** is a game in which the player tries to eat items to increase the length of their snake.
Runs on the Windows Command Prompt  (or Powershell).


# Wiki Hangman <a name="wiki-hangman"></a>

```
      ┌────┐ 
      │    O 
      │   /█\ 
      │   / \ 
  ────┴─────── 
```
Console-based Hangman with random words sourced from Wiktionary, written in Python 3.




[lightcycles-collection]:lightcycles-collection
[console-minesweeper-help]:minesweeper-help
[pong-help]:pong-help
