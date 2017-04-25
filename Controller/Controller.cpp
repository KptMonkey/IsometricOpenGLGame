#include "Controller.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

Controller::Controller() :
    m_CameraPosition(glm::vec3(0.0f, 0.0f,30.0f)),
    m_Forward(glm::vec3(0.0f)),
    m_Sideward(glm::vec3(0.0f)),
    m_Pitch(0.0f),
    m_Yaw(0.0f),
    m_LookAt(0.0f, 30.0f,0.0f)

{
    m_View = glm::lookAt( m_CameraPosition,
                          m_LookAt,
                          glm::vec3(0.0f, 1.0f, 0.0f) );
    m_Projection = glm::perspective( 45.0f, 0.75f, 0.001f, 150.0f );
}

// This function handles to much stuff, has to be splitted
// Time to refactor stuffs get messy ;_;
void
Controller::cameraIsometric(glm::vec3 & moveClick, bool & running, Player &player, Camera & camera) {
    while (SDL_PollEvent(&m_Event)) {
        float dx = 0.0f;
        float dy = 0.0f;
        glm::vec3 shootDir;
        switch (m_Event.type) {

            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                if (m_Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                    running = false;
                }
                if (m_Event.key.keysym.scancode == SDL_SCANCODE_W){
                    dy += 2.0f;
                }

                if (m_Event.key.keysym.scancode == SDL_SCANCODE_S){
                    dy += -1.0f;
                }
                if (m_Event.key.keysym.scancode == SDL_SCANCODE_D){
                    dx =  +2.0f;
                }
                if (m_Event.key.keysym.scancode == SDL_SCANCODE_A){
                    dx = -2.0f;
                }

                camera.Position.x += dx;
                camera.Position.y +=dy;
                camera.LookAt.x += dx;
                camera.LookAt.y += dy;
                updateView(camera);
                break;

            case SDL_MOUSEWHEEL:
                m_Forward = glm::vec3(camera.View[0][2], camera.View[1][2], camera.View[2][2]);
                camera.Position +=(static_cast<float>(m_Event.wheel.y) * m_Forward)*2.10f;
                updateView(camera);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if( m_Event.button.button == SDL_BUTTON_LEFT ) {
                    int x = m_Event.motion.x;
                    int y = m_Event.motion.y;
                    float ndcX = ( x/800.0 - 0.5f ) * 2.0f;
                    float ndcY = ( 0.5f -y/600.0f ) * 2.0f;
                    glm::vec4 homo( ndcX, ndcY, -1.0f, 1.0f );
                    glm::vec4 viewS = glm::inverse(camera.Projection) * homo;
                    glm::vec4 world = glm::inverse(camera.View) * glm::vec4(viewS.x,
                                                                     viewS.y,
                                                                     -1.0f,
                                                                     0.0f);
                    glm::vec3 worldRay(world.x, world.y, world.z);
                    worldRay = glm::normalize( worldRay );

                    float t = -(glm::dot(camera.Position, glm::vec3(0.0f, 0.0f, 1.0f ) )/
                              glm::dot( worldRay , glm::vec3( 0.0f, 0.0f, 1.0f)));
                    float d = 1.0f;
                          d = sqrt(d*d + camera.Position.z * camera.Position.z);
                        auto pos = camera.Position + t * worldRay;
                        moveClick = glm::vec3(pos.x, pos.y, 0.0f);
                }
                if (m_Event.button.button == SDL_BUTTON_RIGHT) {
                    int x = m_Event.motion.x;
                    int y = m_Event.motion.y;
                    float ndcX = ( x/800.0 - 0.5f ) * 2.0f;
                    float ndcY = ( 0.5f -y/600.0f ) * 2.0f;
                    glm::vec4 homo( ndcX, ndcY, -1.0f, 1.0f );
                    glm::vec4 viewS = glm::inverse( camera.Projection) * homo;
                    glm::vec4 world = glm::inverse(camera.View) * glm::vec4(viewS.x,
                                                                     viewS.y,
                                                                     -1.0f,
                                                                     0.0f);
                    glm::vec3 worldRay(world.x, world.y, world.z);
                    worldRay = glm::normalize( worldRay );

                    float t = -(glm::dot(camera.Position, glm::vec3(0.0f, 0.0f, 1.0f ) )/
                              glm::dot( worldRay , glm::vec3( 0.0f, 0.0f, 1.0f)));
                    float d = 1.0f;
                          d = sqrt(d*d + camera.Position.z * camera.Position.z);
                        auto pos = camera.Position + t * worldRay;
                        shootDir = glm::vec3(pos.x, pos.y, 0.0f);
                        std::cout << shootDir.x << " " << shootDir.y <<std::endl;

                        // move stuff to an intersection function
                        player.shoot(shootDir);

                }
            default: break;
        } //switch

    } // Pollevent

}

void
Controller::cameraFPS( bool &running) {
    while (SDL_PollEvent(&m_Event)) {
        float dx = 0.0f;
        float dz = 0.0f;
        glm::mat4 invMat = glm::inverse(m_View);
        switch (m_Event.type) {

            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                if (m_Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                    running = false;
                }
                if (m_Event.key.keysym.scancode == SDL_SCANCODE_W){
                    dz = -5.0f;
                }

                if (m_Event.key.keysym.scancode == SDL_SCANCODE_S){
                    dz = 5.0f;
                }
                if (m_Event.key.keysym.scancode == SDL_SCANCODE_D){
                    dx =  +1.0f;
                }
                if (m_Event.key.keysym.scancode == SDL_SCANCODE_A){
                    dx = -1.0f;

                }
                m_Forward = glm::vec3(m_View[0][2], m_View[1][2], m_View[2][2]);
                m_Sideward = glm::vec3(m_View[0][0], m_View[1][0], m_View[2][0]);

                m_CameraPosition += (dz * m_Forward + dx *m_Sideward) * 0.10f;
//                updateView();

                 break;

                case SDL_MOUSEMOTION:
                    m_Yaw += 0.005f * m_Event.motion.xrel;
                    m_Pitch -= 0.005f * m_Event.motion.yrel;
//                    updateView();

            default: break;

        } //switch

    } // Pollevent
}

//void
//Controller::updateView(){

//    glm::quat qPitch = glm::angleAxis(-m_Pitch, glm::vec3(1,0,0));
//    glm::quat qYaw = glm::angleAxis(m_Yaw, glm::vec3(0,0,1));
////    glm::quat qRoll = glm::angleAxis( roll, glm::vec3(0,0,1));
//    glm::quat orientation = qPitch*qYaw;
//    orientation = glm::normalize( orientation );
//    glm::mat4 rotate = glm::mat4_cast(orientation);
//    glm::mat4 translate(1.0f);
//    translate = glm::translate(translate, -m_CameraPosition );

//     m_View = rotate * translate;
////     m_View = glm::lookAt( m_CameraPosition,
////                           glm::vec3(0.0f, 30.0f, 0.0f),
////                           glm::vec3(0.0f, 1.0f, 0.0f) );

//}

void
Controller::updateView(Camera &camera){

    camera.View = glm::lookAt( camera.Position,
                           camera.LookAt,
                           glm::vec3(0.0f, 1.0f, 0.0f) );
}
