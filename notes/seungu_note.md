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

## Tank death effect

I think I can just change the sprite of the tank itself whenever it is dead.

Make a variable like, tankDeathFrame = 160, and say if it's over 140 show sprite 1, if over 100 show sprite 2....

During the death the tank should not be moved or collide with anything. 

## Title

Make the title little more active - right now it just looks like a photo.

## Font

Try to mix controlling fonts and different font choices so that logo can also just be font? If I have extra time.