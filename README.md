Asteroids Reimagined Beta 4.24.13 Released.

Instructions:
AWD to rotate and move. Use mouse to aim turret. Right mouse for shield. Left mouse to fire. Gun heats up as you fire, and slowly cools when you're not. Using shield drains its power, and slowly regains when not in use. E or Middle Mouse button for secondary weapon (Red, Yellow, Orange and Violet.) Mouse Wheel or F to switch type if you have more than one.

Power ups, blue one boost your shields, ice blue ones give your gun cool boost, they are both cumulative. Purple is a super power up that keeps the shield on, and gun cool, and is timed. Red is the Big Shot you get a limited number. Yellow is Double Shot, you get a limited number. Orange is Homing Mine, they seek out enemy ships, you get a limited number. Violet is Plasma Shot, it grows as it goes, destroying everything in its path. Ends at edge of screen.

Enter key is now used for Forced Spawn when player is waiting for rocks to clear spawn area.

Game also has controller support.

Made with raylib and my Knightmare arcade engine in C++.

To try it out, unzip this to your desktop or documents, or somewhere without write protection for saving the high score. Download the file Asteroids Reimagined.zip or the Update zip file if you already did the previous one.

Bug Fixes:
Player homing mine went to center of screen when Death Star or fighters active. Fixed.
High Score is not displayed when game started. Fixed.
Enemy guided missile chased player if player not active. Fixed
Player Ship sometimes visible for a frame in middle of window after being hit before spawning. Fixed.
Fixed end of game, first letter of new high score is A.
Fixed bug that allowed user to select secondary weapon that was not available.
Fixed bug that power ups headed to player after player hit, again.

Changes:
When UFO hits rock, 5 to number of rocks by wave spawn if not a small rock.
After every five waves, more than one more rocks spawn at the start of the wave. That number increases every five waves.
Player shot (Turret and secondary) now effected by ship velocity.
Big Shot now takes out multiple rocks(5), enemies(2).
Player can change active secondary weapon if they have more than one. UI for active secondary weapon added.
Remade icon files. Remade player ship model.
Enemy/UFOs don't hit small rocks. UFOs don't shoot other enemies, but they can hit each other.
All twenty high score names are now shown on high score list.
Secondary weapon power ups are accumulative now.
When secondary weapon runs out of ammo, Only one with ammo or None is selected if multiple have ammo, or none do.
Added F button for secondary weapon selection.
If only secondary weapon power up, it is selected.
Added controller support for new features.