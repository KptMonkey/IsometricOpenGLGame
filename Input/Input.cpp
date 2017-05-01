#include "Input.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Input::Input() {
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

void
Input::updateInput(Camera &camera, Player &player, bool & running, HeightMap  & map) {
    while (SDL_PollEvent(&m_Event)) {
        switch (m_Event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                if (m_Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    running = false;

                moveCamera(camera);
                break;

            case SDL_MOUSEWHEEL:
                zoomCamera(camera);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if( m_Event.button.button == SDL_BUTTON_LEFT ) {
                    movePlayer(player, camera, map);
                }
                if (m_Event.button.button == SDL_BUTTON_RIGHT) {
                    shootPlayer(player, camera, map);
                }
            default: break;
        } //switch
    } // Pollevent
}

void
Input::moveCamera(Camera &camera) {
    float dx, dy;
    dx = dy = 0.0f;
    if (m_Event.key.keysym.scancode == SDL_SCANCODE_W){
        dy = 2.0f;
    }
    if (m_Event.key.keysym.scancode == SDL_SCANCODE_S){
        dy = -2.0f;
    }
    if (m_Event.key.keysym.scancode == SDL_SCANCODE_D){
        dx =  2.0f;
    }
    if (m_Event.key.keysym.scancode == SDL_SCANCODE_A){
        dx = -2.0f;
    }

    camera.Position.x += dx;
    camera.Position.y +=dy;
    camera.LookAt.x += dx;
    camera.LookAt.y += dy;
    updateView(camera);
}

void
Input::zoomCamera(Camera &camera) {
    glm::vec3 zoom = glm::vec3(camera.View[0][2], camera.View[1][2], camera.View[2][2]);
    auto tCameraPos = camera.Position + (static_cast<float>(m_Event.wheel.y) * zoom)*2.10f;
    if (tCameraPos.z >= 75.0f || tCameraPos.z <= 45.0f)
        return;
    camera.Position +=(static_cast<float>(m_Event.wheel.y) * zoom)*2.10f;
    updateView(camera);
}

void
Input::updateView(Camera &camera){

    camera.View = glm::lookAt(camera.Position,
                              camera.LookAt,
                              glm::vec3(0.0f, 1.0f, 0.0f));
}

void
Input::movePlayer(Player &player, Camera const & camera, HeightMap & map) {
    auto intersection = intersectWithMap(camera);
    if (map.getHeight(intersection.x, intersection.y) > 20.0f) return;
    player.setNextPosition(intersection);
}

void
Input::shootPlayer(Player &player, const Camera &camera, HeightMap &map) {
    auto shootDir = intersectWithMap(camera);
    shootDir.z = map.getHeight(shootDir.x, shootDir.y);
    player.shoot(shootDir);
}

glm::vec3
Input::intersectWithMap(const Camera &camera) {
    int x = m_Event.motion.x;
    int y = m_Event.motion.y;
    float ndcX = (x/800.0f - 0.5f) * 2.0f;
    float ndcY = (0.5f - y/600.0f) * 2.0f;
    glm::vec4 homo(ndcX, ndcY, -1.0f, 1.0f);
    glm::vec4 view = glm::inverse(camera.Projection) * homo;
    glm::vec4 world = glm::inverse(camera.View) * glm::vec4(view.x,
                                                            view.y,
                                                            -1.0f,
                                                            0.0f);
    glm::vec3 worldRay(world.x, world.y, world.z);
    worldRay = glm::normalize( worldRay );

    float t = -(glm::dot(camera.Position, glm::vec3(0.0f, 0.0f, 1.0f ) )/
              glm::dot(worldRay, glm::vec3( 0.0f, 0.0f, 1.0f)));
    float d = sqrt(d*d + camera.Position.z * camera.Position.z);
    return (camera.Position + t * worldRay);
}
