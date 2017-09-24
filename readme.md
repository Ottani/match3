# Match 3

Simple game of matching 3 or more gems in a row or column.
This project was created for learning SMFL and practicing new features of C++11, 14 and 17.

## Getting Started

Select adjacent cells to swap them, if the gems match in a row or column of 3 or more of the same color, they will disappear, creating more when the top row is free.
Press 'Q' to show a possible move.
Every 50 points the game will spawn a bomb, click on the bomb to clear that area.

### Building and running

Requires cmake, c++17 compiler and SFML, then it is just a matter of building it:

```
cmake .
make
```

## Built With

* [SFML](https://www.sfml-dev.org/) - Simple and Fast Multimedia Library
* [CMake](https://cmake.org/) - Build tool
* [GCC](https://gcc.gnu.org/) - GCC, the GNU Compiler Collection v7.1.1, on Linux
* [MinGW64](https://mingw-w64.org/doku.php) - GCC for Windows 64 & 32 bits, on windows

## Authors

* [Paulo Assis](https://github.com/Ottani)

## License

MIT License

## Acknowledgments

* Inspired on [FamTrinli](https://www.youtube.com/channel/UCC7qpnId5RIQruKDJOt2exw)'s Youtube video [Let's make 16 games in C++: Bejeweled (Match-3)](https://youtu.be/YNXrFOynalE).
* Gems images by [GameArtForge](https://opengameart.org/users/gameartforge) at [OpenGameArt.org](https://opengameart.org/content/gems-set-01).
* Bomb images by [SpriteAttack](https://opengameart.org/users/spriteattack) at (OpenGameArt.org)(https://opengameart.org/content/emotional-explosives)
* Background image from [MontanaDiva](https://pixabay.com/en/users/MontanaDiva-1981003/) at [PixaBay](https://pixabay.com/en/background-orange-blue-green-1247931/).
* Sansation font by [Bernd Montag](https://www.fontsquirrel.com/fonts/list/foundry/bernd-montag) at [Font Squirrel](https://www.fontsquirrel.com/fonts/sansation)
