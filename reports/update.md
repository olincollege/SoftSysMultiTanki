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

- [What frameworks should I consider for online multiplayer?](https://www.reddit.com/r/gamedev/comments/4e5t3k/what_frameworks_should_i_consider_for_online/)
- Head First C, Chapter 11: Sockets and networking
- Computer Systems, A Programmer's Perspective Chapter 11: Network Programming
- [GeeksforGeeks Socket Programming in C](https://www.geeksforgeeks.org/socket-programming-cc/)
- [GeeksforGeeks Example for fork and pipe](https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/)
- [IBM's C Socket Documentation](https://www.ibm.com/docs/en/zos/2.3.0?topic=interfaces-c-socket-application-programming-interface)
- [GeeksforGeeks File Transfer using UDP](https://www.geeksforgeeks.org/c-program-for-file-transfer-using-udp/)

#### SDL

- [SDL Tutorials](http://wiki.libsdl.org/Tutorials)
- [SDL2 Game Tutorials](https://www.parallelrealities.co.uk/tutorials/)
- [Probably Worth Buying](https://parallelrealities.itch.io/sdl2-tutorials)
- [Lazyfoo SDL Tutorials](https://lazyfoo.net/SDL_tutorials/)
- [Modern SDL programming tips & tricks](https://www.reddit.com/r/gamedev/comments/re4zvh/modern_sdl_programming_tips_tricks/)
- [SDL 2.0 API by Category](https://wiki.libsdl.org/APIByCategory)

## Update

### Single-Player Game
*SeungU*

[![Update Demo](http://img.youtube.com/vi/JRxff37KnSA/sddefault.jpg)](https://youtu.be/JRxff37KnSA)

*click the image for demo video*

We made a good progress on implementing the basic game part. All the basic game features with the stretch goal is implemented, except for sound which will soon be added. This includes graphics, collisions, tank movement, receiving player inputs, bullets and reflection, lives, and special effects.

We had to learn a lot from external resources to achieve the current state. We have classified files in our src folder so that we can correctly attribute each work at the end of the project. 

- files in *imported* folder : imported files with minimal/no changes
- files in *modified* folder : imported files but added modification so the big portion of code is written by us.
- files in *original* folder: our original code written from scratch

### Multiplayer P2P UDP Connection
*Melissa*

One thing we have implemented is the matchmaking server with code as seen in `network/matchmaking_server.c`. The executable is running on a cloud server, so that it keeps a static ip (which is not possible on the Olin network). The matchmaking server connects to two different clients (and restarts the client pair when two clients have connected). For each client, the server receives the ip address of the client, and when its ready, sends the ip address of the other client. So, the clients don't need to have ip addresses entered by the user, as it gets the peer client's ip address from this host connection. They can then drop down from this host connection, in order to next form a P2P connection.

The peer code, `network/peer.c`, is currently written to access this matchmaking server based on its specific ip address, and wait for and receive the ip address of the other peer. It then disconnects from the matchmaking server, and forms a connection to the other peer with UDP protocol.

### Game and Network Integration
*SeungU and Melissa*

The player will be required to run the game client and the p2p client to play the multiplayer game. We are aiming to create a real-time multiplayer game, so we will utilize the low latency that we can achieve from UDP connection.

The game client will be recording the player's input and their tank's position, dx/dy, and angle to a JSON file every frame. The p2p client will try to request and update each player's JSON file real-time, so that the game client can modify the other player's position as real-time as possible. We will track the state of each tank with timestamp, so that we do not use the same data again. If there was lag/packet loss during the process and no update happened, the game client will assume that a same input was done by the other players and continue without modifying their positions. For the first integration, we will just try to save each other's input and see what happens.

We are also planning to implement the concept of host computer, where some important event like player's death is calculated just in one computer to prevent some occasions where events happen in one client but not in the other. 

### Current Tasks

- Melissa: Have peers send and receive data from each other
- Melissa and SeungU: Have peers send and receive changes to the game state JSON file
- SeungU: Implement sound feature to the game, fix some bugs/memory leaks, add additional feature like better title/gameover screen if possible.

