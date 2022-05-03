# Network

## Research:
- Head First C, Chapter 11: Sockets and networking
- Computer Systems, A Programmer's Perspective Chapter 11: Network Programming
- [Enet](https://github.com/zpl-c/enet) Thunderbolt via UDP
- [GeeksforGeeks](https://www.geeksforgeeks.org/socket-programming-cc/)
- [GeeksforGeeks UDP file transfer](https://www.geeksforgeeks.org/c-program-for-file-transfer-using-udp/)

Find port by something that isn't used already
`/etc/services`, decided on port 5200.

# Plan:

- P2P (peer to peer)
- UDP (rather than TCP), since it is simpler and faster, recommended for games

Both computers connect to a specific to a specific host. They pass their own IP addresses to this host.
This connection then drops down to the peers talking to each other, given the IPs provided by the host.

Moving from JSON file send and receive implementation. It is much slower than just sending bytes over
the network (memory to hard drive to JSON serialization).

In order to upload the executable to a server using ssh:

First get the file onto the server.
```rsync matchmaking_server user@server:matchmaking_server```

Then, use `tmux`.
Run `./matchmaking_server`, and it will continue to run in the background!
