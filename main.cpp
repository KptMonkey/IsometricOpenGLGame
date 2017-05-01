#include "RenderAbstraction.hpp"
#include "Level/HeightMap.hpp"
#include "Player.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Collectable.hpp"
#include "Enemy.hpp"
#include "Camera/Camera.hpp"
#include "Input/Input.hpp"

int main() {
    RenderContext rctx;
    Input input;
    Camera camera(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 30.0f, 0.0f));

    HeightMap terrain("./media/level/firstheightmap.jpg"); // name sucks
    Player player(terrain);
    Collectable colTest(terrain);
    Enemy testEnemy(terrain, glm::vec2(15.0f, 0.0f));

    //DepthMap and DepthMap visualization
    Shader depthShader;
    depthShader.bindShader("./shader/SimpleDepth.vert");
    depthShader.bindShader("./shader/SimpleDepth.frag");

    Shader debug;
    debug.bindShader("./shader/DebugDepth.vert");
    debug.bindShader("./shader/DebugDepth.frag");


    Texture depth;
    depth.createDepthTexture(2048, 2048);
    terrain.setDepthTexture(depth);

    FrameBuffer depthFBO;
    depthFBO.createDepthFrameBuffer(depth);

    bool running = true;

    std::vector<IGlobalRenderable*> world;
    world.emplace_back(&terrain);
    world.emplace_back(&player);
    world.emplace_back(&colTest);
    world.emplace_back(&testEnemy);


    rctx.enableDepthTest();
    while ( running ) {
        player.updatePosition();
        input.updateInput(camera, player, running, terrain);

        glViewport(0,0,2024,2024);
        depthShader.activate();
        depthShader["lightSpace"]=terrain.getLightSpace();

        depthFBO.bind();
        rctx.clearDepthBuffer(); // fucking wichtig
        for (auto & e : world) {
            e->drawShadow(depthShader, rctx);
        }
        depthFBO.unbind();

        glViewport(0,0,800,600);
//        rctx.drawDepthMap(debug, depth);
        rctx.clearColor(0.1f, 0.3f, 0.3f, 1.0f);
        rctx.clearColorBuffer();
        rctx.clearDepthBuffer();

        for ( auto & e : world) {
            e->draw(camera, rctx);
        }
        colTest.intersect(player); // A scene manager should manage this stuff
        testEnemy.intersectWithBullet(player.getBullets());
        testEnemy.lookForPlayer(player);

        rctx.swapBuffers();
    }
}
