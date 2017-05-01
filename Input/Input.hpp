#pragma once
#include "Camera/Camera.hpp"
#include "Player.hpp"
#include "Level/HeightMap.hpp"
#include <SDL2/SDL.h>

class Input {

public:

Input();
void
updateInput(Camera & camera, Player & player, bool & running, HeightMap & map);

private:

void
updatePlayer(Player & player);

void
moveCamera(Camera & camera);

void
zoomCamera(Camera & camera);

void
updateView(Camera & camera);

void
movePlayer(Player & player, Camera const & camera);

void
shootPlayer(Player & player, Camera const & camera, HeightMap &map);

glm::vec3
intersectWithMap(Camera const & camera);

SDL_Event m_Event;


};
