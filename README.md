Asteroids Reimagined RC 4.39.119 Released.

![Asteroids Reimagined RC 4](https://github.com/user-attachments/assets/d57901d3-3caf-47cb-bf0e-4c03a182b9f0)

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
UFO was not getting hit by Enemy Missile fixed.
Fixed Fighter had no VFX but had audio when hit, Fighter pair had VFX, but no audio when hit. Removed VFX from fighter pair hit, fixed audio.
Fixed UFO shot not getting destroyed when player hit/shield hit.
Fixed player homing mines chasing Death Star no matter how far away it was.
Fixed Enemy firing many missiles at once suddenly.
Fixed Boss did not spawn for wave 5, game in unable to complete.
Fixed Enemy Two that drops mines, after wave ten would drop a mine every frame after the first time.
Fixed Death Star flicker on spawn. Was starting on opposite side of screen.
Fixed High Score instruction text.
Fixed if Player hit by Fighter Pair, Player lost two lives.
Fixed Double Shot and Plasma shot not hitting Boss after shield down.
Fixed all weapons giving the player points after the game is over.

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
UFO is now able fire more than two shots at once. Time between shots is greater but lessons as more UFOs spawn and each wave.
It is now possible to have more than two UFOs active at once. Spawn times shortens by wave/spawn amount.
Secondary weapon selection and ammo now stay at bottom of ship.
Player now gets points for taking down the Boss Shields, and for each Turret.
UFO Changes direction more often in later waves.
Missile Enemy now can shot more than one at a time. Starts out with more delay than before, but lessons each wave.
Mine Enemy mine drop more often, starts out less often than before.
Missile now has sound when active.
PowerUp time is now additive.
Removed Player homing mines going after fighter/fighter pair.
Increased the range and speed of player homing mines.
Enemy One that fires missiles, now can spawn more than one at a time in later waves.
Death Star now spawns at random location, direction.
Enemy Two that lays mines, now can spawn more than one at a time in later waves.
Changed Fighter/Fighter Pairs behavior.
Total rework of Drop chance of Power Ups. Now way more drops on first wave. Fewer drops over all past that, and fewer Purple/Full Power Ups.
Purple PowerUp is limited after wave Five. All Power Drops are limited after wave five as well.]
New life score amount now 2 * Wave * 10000 after the first new life.
UFO no longer shoots if player hit, and game is not over. It wont shoot again after player spawns until shot timer expires.
Player Mines are much more effective now as they move faster than before. They can catch up to any enemy now.
Made another change on how player homing mines work on deciding what enemy to go after if there is more than one, goes after closest.
Boss on wave 10+ now has guided missiles, on 15+ drops mines.
Player homing mines no longer go after Death Star.
Fighter gets faster on each later wave.
Enemies now fire shots as well. If they hit a rock, it spawns more than usual, but not as many as the UFO.
Ammo bar for secondary weapon is now gray instead of white.
Boss chases player after all turrets are destroyed.
Only the UFO does not hit small rocks.
Player can no longer force spawn into something.
UFO now can shot small rock, but still wont run into them.
Plasma Shot now also destroys all enemy shots.
Deathstar now appears at the first wave, if there are less than three rocks. After wave 10, if there are less than ten rocks it will spawn.

Internal Changes:
Knightmare Engine Update, Made every entity an Entity in Entity Manager.
Added math_operators.h to Knightmare Engine.