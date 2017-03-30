#include "RenderAbstraction.hpp"
#include "Controller/Controller.hpp"
#include "Level/HeightMap.hpp"
#include "Player.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Collectable.hpp"

int main() {

    //TODO: Create player, level classes which draw them selves
    RenderContext rctx;
    Controller playerInput;
    Player player;
    HeightMap terrain("./media/level/firstheightmap.jpg"); // name sucks
    player.init();
    Collectable colTest;
    colTest.init(terrain); // height, view and projection need a global access

    //DepthMap and DepthMap visualization
    Shader depthShader;
    depthShader.bindShader("./shader/SimpleDepth.vert");
    depthShader.bindShader("./shader/SimpleDepth.frag");

    Shader debug;
    debug.bindShader("./shader/DebugDepth.vert");
    debug.bindShader("./shader/DebugDepth.frag");


    Texture depth;
    depth.createDepthTexture(2024, 2024);

    FrameBuffer depthFBO;
    depthFBO.createDepthFrameBuffer(depth);

    bool running = true;
    glm::vec3 moveClick(0.0f);

    std::vector<std::unique_ptr<IGlobalRenderable>> world;
    world.emplace_back(&terrain);
    world.emplace_back(&player);
    world.emplace_back(&colTest);


    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
//    glCullFace(GL_BACK);
    int i = 0;
    rctx.enableDepthTest();
    while ( running ) {
        player.updatePosition(terrain,moveClick);
        //Light should have its own struct or so..
        glm::vec3 lightPos=glm::vec3(0.0f, 160.0f, 120.0f);
        ++i;
        glm::mat4 lightProjection = glm::ortho(-160.0f, 160.0f, -166.0f, 158.0f, 120.1f, 380.0f);
        glm::mat4 lightView = glm::lookAt( lightPos,
                                           glm::vec3(0.0f, 20.0f, 0.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f) );

        glm::mat4 lightSpace = lightProjection * lightView;

        playerInput.cameraIsometric(moveClick, running,player);
//        playerInput.cameraFPS(running);
//        playerInput.m_CameraPosition.z = terrain.getHeight(playerInput.m_CameraPosition.x, playerInput.m_CameraPosition.y) + 1.5f;
//        playerInput.updateView();

        glViewport(0,0,2024,2024);
        depthShader.activate();
        depthShader["lightSpace"]=lightSpace;

        depthFBO.bind();
        rctx.clearDepthBuffer(); // fucking wichtig
        for (auto & e : world) {
            e->draw(depthShader, rctx);
        }
        depthFBO.unbind();

        glViewport(0,0,800,600);
//        rctx.drawDepthMap(debug, depth);


        rctx.clearColor(0.1f, 0.3f, 0.3f, 1.0f);
        rctx.clearColorBuffer();
        rctx.clearDepthBuffer();

        terrain.drawTerrain(rctx, depth, playerInput.getView(),playerInput.getProjection(), lightSpace, lightPos,playerInput.m_CameraPosition); // The scene manager would be here nice too
        player.drawPlayer(rctx, playerInput.getView(), playerInput.getProjection());
        colTest.drawCollectable(rctx,playerInput);
        colTest.intersect(player); // A scene manager should manage this stuff

        rctx.swapBuffers();
    }
}
