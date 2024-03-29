# MultiTanki
> Melissa Kazazic and SeungU Lyu


## Goals

We aim to create a multiplayer game like the [Wii Tanks](https://youtu.be/orLxrg51xL8) where each players can connect to the same session on their own computers. Unlike the link, we aim to create the game fully top-down 2D. 

### MVP

The game will have similar mechanics as Wii Tanks, with the ability to move the tank (with either keyboard input or mouse tracking), aim the shots, and kill the enemy with the collision detection. The game will be multiplayer, joining the session from separate computers. The MVP version of the game will be done with minimal graphics, not including extra details like effects or animations.

### Stretch Goal

The first stretch goal would be to have the shells bounce off the walls from the angle where it was originally shot from. Along with the working multiplayer tank game, we would like to apply random map generation. This is so that every time the game is started, new maps would show up, creating new fun experiences. Then, we would also like to apply extra explosion effects, graphics, music, sound effects, etc., so that we can create a full game if we can.

## Individual Learning Goals

### Melissa

- Learn how to make a game in C
- Learn how to get multiplayer connection for said game in C
- Learn how to do graphics in C (+ incl. libraries)

### SeungU

- Learn how to use libraries to build a game in C
- Learn how to implement multiplayer feature
- If possible, learn about algorithms behind random-map generation and implement it

## Getting Started

### Resources

#### General Design

- [Wii Tanks](https://youtu.be/orLxrg51xL8)
- [Kenney Free Assets](https://www.kenney.nl/assets)

#### Multiplayer

- [TCP Server-Client Implementation in C](https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/)
- [sdl_networking](https://www.reddit.com/r/gamedev/comments/b6j5dp/sdl_networking/)
- [What frameworks should I consider for online multiplayer?](https://www.reddit.com/r/gamedev/comments/4e5t3k/what_frameworks_should_i_consider_for_online/)
- [ENet](http://enet.bespin.org/index.html)

#### SDL

- [SDL Tutorials](http://wiki.libsdl.org/Tutorials)
- [SDL2 Game Tutorials](https://www.parallelrealities.co.uk/tutorials/)
- [Probably Worth Buying](https://parallelrealities.itch.io/sdl2-tutorials)
- [Lazyfoo SDL Tutorials](https://lazyfoo.net/SDL_tutorials/)
- [Modern SDL programming tips & tricks](https://www.reddit.com/r/gamedev/comments/re4zvh/modern_sdl_programming_tips_tricks/)
- [SDL 2.0 API by Category](https://wiki.libsdl.org/APIByCategory)

### First Tasks

- Research how to implement multiplayer feature in C
- Research how to implement graphics in C, finding the right libraries to use
- Implement inputs (WASD) and mouse-follower, based on what libraries we use for graphics