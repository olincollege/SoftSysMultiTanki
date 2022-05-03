# SoftSysMultiTanki

![multitanki.gif](multitanki.gif)

MultiTanki is a multiplayer game written in C using SDL2. It is inspired by the Tanks! game from the Wii Play, but unlike the Tanks! it is fully made of top-down 2D graphics instead of 3D. It communicates through a simple UDP P2P connection between computers to achieve the multiplayer feature.

## Demo

[Youtube](https://youtu.be/P3Cs8kipk8w)

## Building

To install dependencies on Ubuntu:

    $ sudo apt-get install libsdl2-dev
    $ sudo apt-get install libsdl2-image-dev
    $ sudo apt-get install libsdl2-mixer-dev
    $ sudo apt-get install libsdl2-ttf-dev

To compile:

    $ make

To run:

    $ ./multitanki

## Instructions

* <kbd>w</kbd> <kbd>a</kbd> <kbd>s</kbd> <kbd>d</kbd> : Move the tank
* <kbd>Left Mouse Button</kbd> : Shoot bullet to current cursor position
* <kbd>SPACE</kbd> : Start/restart game on title/gameover screen
* <kbd>ESC</kbd> : End game on title/gameover screen

## Known Issues

- If no sound plays or there is a big delay, try re-starting the game
