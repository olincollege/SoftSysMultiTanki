## Notes

## UDP

How to deal with UDP multiplayer?

Right now I considered only the player input, naively thinking that no packet loss will happen - which is not the UDP way.

We need little more data, which is the tank's position + bullet's position. 

In this way, since the tank's position will be corrected even after packet loss, two players will be looking at the same screen most of the time. 

Even if packet loss happens 1 time in every 100 communications, it will not be noticeable. 

This way, player with index 0 will be the host computer, who checks for tank's collision with bullets. 

Whenever host computer detects death of other players, the data sent will be accounted by all the other players even if their screen did not show their death (which is unlikely)

So prob need death data/death frame to also be sent through communication.

## Title

Make the title little more active - right now it just looks like a photo.

## Restarting the game

Need to free all the memory before finishing the game. 

Also have to add quit game option or something