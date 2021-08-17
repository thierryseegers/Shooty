Shooty
======

This simple game is a reimplementation of the game presented in the book "[SFML Game Development](https://www.sfml-dev.org/learn.php)".

As such, there's nothing novel here except... for the *very* opinionated approach I took to C++-ify the code as much I liked.
I probably went too far in some cases with the class hierarchy and the templatization (e.g. qualifying an aircarft being friend or foe through its type) and I expect some seasoned game developers would chastize me for it but it was a fun exercice nonetheless.

One extra feature I added was the use of `sf::Joystick` functionalities to play the game with my original [SNES game controller](https://en.wikipedia.org/wiki/Super_Nintendo_Entertainment_System#Peripherals)!