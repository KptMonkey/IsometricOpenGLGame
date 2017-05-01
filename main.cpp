#include "RenderAbstraction.hpp"
#include "Level/HeightMap.hpp"
#include "Player.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <random>
#include "PowerUp.hpp"
#include "Enemy.hpp"
#include "Camera/Camera.hpp"
#include "Input/Input.hpp"

void
createEnemys(std::vector<Enemy> & enemys, HeightMap & map);

int main() {
    RenderContext rctx;
    Input input;
    Camera camera(glm::vec3(90.0f, -170.0f, 70.0f), glm::vec3(90.0f, -160.0f, 0.0f));

    HeightMap terrain("./media/level/test.jpg"); // name sucks
    Player player(terrain,90,-170);
    PowerUp powerUpGrow(terrain,-110, -225);
    std::vector<Enemy> enemys;
    createEnemys(enemys, terrain);
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
    world.emplace_back(&powerUpGrow);
    for (auto & e : enemys) {
        world.emplace_back(&e);
    }

    rctx.enableDepthTest();
    while ( running ) {
        player.updatePosition();
        input.updateInput(camera, player, running, terrain);

        glViewport(0,0,2048,2048);
        depthShader.activate();
        depthShader["lightSpace"]=terrain.getLightSpace();

        depthFBO.bind();
        rctx.clearDepthBuffer(); // fucking wichtig
        for (auto & e : world) {
            e->drawShadow(depthShader, rctx);
        }
        depthFBO.unbind();

        glViewport(0,0,800,600);
        rctx.drawDepthMap(debug, depth);
        rctx.clearColor(0.1f, 0.3f, 0.3f, 1.0f);
        rctx.clearColorBuffer();
        rctx.clearDepthBuffer();

        for ( auto & e : world) {
            e->draw(camera, rctx);
        }
        powerUpGrow.intersect(player); // A scene manager should manage this stuff
        for (auto & e : enemys) {
            e.intersectWithBullet(player.getBullets());
            e.lookForPlayer(player);
        }
        rctx.swapBuffers();
    }
}

void
createEnemys(std::vector<Enemy> & enemys, HeightMap & map) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for ( int i = -100; i<70; i +=10) {
        for (int j = -200; j<140; j+=20) {
            if (dist(mt)<0.35f)
                enemys.emplace_back(map,glm::vec2(i,j));
        }
    }
}
