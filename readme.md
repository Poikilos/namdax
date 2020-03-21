# namdax
This is an experimental game using
[bonefinity](https://github.com/poikilos/bonefinity).


## Developer Notes:
### Movement (in chronological order):
- In your CPP file containing main (such as main.cpp or dxman.cpp):
  - In Gamemain, regardless of iGameState, if is controlled by player, a,s,d,w (SDLK_a etc) or other method sets a bit in u32Pressing to mark the key as pressed
  - In Gamemain, GAMESTATE_RUN
    - If is controlled by player and a bit of u32Pressing that matches a direction is on, Move method (such as for object hero, hero->Move) is called
    - Entity is moved using Move method
    - All entities are refreshed (such as hero->Refresh). Then in Refresh method of Entity (in entity.cpp):
      - Movement
      - Physics
      - Limits location to maximums stored in m3dEnt (such as m3dEnt.yMax)
- negative 90 degrees (negative Y) moves away from camera. The ground is an inverted cartesian system, similar to a computer screen.
