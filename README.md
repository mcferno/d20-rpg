# D20 Game

A cross-platform C++ game based on the [D20 System](http://en.wikipedia.org/wiki/D20_System) from Dungeons and Dragons.

![Gameplay Screenshot](https://mcferno.com/content/media/2d-indie-rpg-screenshot.png "Gameplay Screenshot")

## Introduction

This project started as a university school project by [Patrick McFern](https://mcferno.com) and
[Christopher Karpyszyn](http://www.chriskarpyszyn.com/) in 2007. The project assignment asked that we implement the
D20 rules in a turn-based RPG using C++. The rest was up to us.

Despite the hundreds of hours it took to get this project together, the code sat on a dusty hard drive for years. In
2015, we decided to open source as much of the project as we could as it benefits no one being hidden out of sight.
More about licensing below.

We're putting this code into the wild. Feel free to fork it, remix it and make it your own. We had a lot of fun assembling
this project and we hope it inspires you.

## Libraries

The code depends on the following libraries:

 * [Simple DirectMedia Layer](http://www.libsdl.org/) (SDL) v1.2.x (LGPL licensed)
 * [SDL image](https://www.libsdl.org/projects/SDL_image/) v1.2.x (zlib licensed)
 * [SDL mixer](https://www.libsdl.org/projects/SDL_mixer/) v1.2.x (zlib licensed)
 * [SDL ttf](http://www.libsdl.org/projects/SDL_ttf/) v2.0.x (zlib licensed)

## Project Status

The code was recently revived in order to compile against the C++11 standard. It has been tested on OSX 10.9 and Windows
7, and should generally work on any platform supported by SDL 1.2.

Here's some of the known issues at this time:

### Dependence on console window output

The console window output details some of the mechanics of the game during gameplay. In particular, the outcome of
combat with AI characters are only shown here. This was a requirement for the original project and has not yet been
improved. In the future, the console output should be moved into the graphical portion of the game.

### OSX support

The game will build and run under OSX, but there are some known issues.

* No console window. Combat outcome is hidden from the player.
* Map editor does not save changes.

## Licensing

This project uses media which is not open sourced. Thankfully, the C++ code is our own property and we've made that
available under the MIT license.

Please read COPYRIGHTS.txt and LICENSE.txt for a detailed overview of the licensing.
