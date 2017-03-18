#include "RenderAbstraction.hpp"
#include "Controller/Controller.hpp"
#include "Level/HeightMap.hpp"
#include "Player.hpp"
#include <glm/gtc/matrix_transform.hpp>
int main() {

    //TODO: Create player, level classes which draw them selves
    RenderContext rctx;
    Controller playerInput;
    HeightMap terrain("./media/level/firstheightmap.jpg");
    Player player;
    player.init();

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

    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    glCullFace(GL_BACK);

    int i = 0;
    rctx.enableDepthTest();
    while ( running ) {
        player.updatePosition(terrain,moveClick, playerInput);
        //Light should have its own struct or so..
        glm::vec3 lightPos=glm::vec3(std::cos(i*0.01f)*70.0f, 160.0f, 140.0f);
        ++i;
        glm::mat4 lightProjection = glm::ortho(-128.0f, 128.0f, -66.0f, 58.0f, 100.1f, 400.0f);
        glm::mat4 lightView = glm::lookAt( lightPos,
                                           glm::vec3(0.0f, 0.0f, 0.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f) );

        glm::mat4 lightSpace = lightProjection * lightView;

        playerInput.cameraIsometric(moveClick, running);
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

//        rctx.drawDepthMap(debug, depth);

        glViewport(0,0,800,600);

        rctx.clearColor(0.1f, 0.3f, 0.3f, 1.0f);
        rctx.clearColorBuffer();
        rctx.clearDepthBuffer();

        terrain.drawTerrain(rctx, depth, playerInput, lightSpace, lightPos);
        player.drawPlayer(rctx, playerInput);
        rctx.swapBuffers();
    }
}
