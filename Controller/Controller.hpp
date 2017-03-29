#pragma once
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

class Controller {
public:
    Controller();

    void
    cameraIsometric(glm::vec3 & moveClick, bool & running );

    void
    cameraFPS( bool & running);

    void
    updateView();

    glm::mat4
    getView() { return m_View;}

    glm::mat4
    getProjection() { return m_Projection; }

    glm::vec3 m_CameraPosition;
private:

    glm::vec3 m_Forward;
    glm::vec3 m_Sideward;
    float     m_Pitch;
    float     m_Yaw;
    glm::mat4 m_View;
    SDL_Event m_Event;
    glm::mat4 m_Projection;
    glm::vec3 m_LookAt;
};
