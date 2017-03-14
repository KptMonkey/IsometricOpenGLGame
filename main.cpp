#include "RenderAbstraction.hpp"
#include "Controller/Controller.hpp"
#include "Level/HeightMap.hpp"
#include "Player.hpp"

int main() {

    //TODO: Create player, level classes which draw them selves
    RenderContext rctx;
    Controller playerInput;
    HeightMap terrain;

    terrain.loadHeightMapFromImage("./media/level/firstheightmap.jpg");
    // GlTypes::Float is even more text than GL_Float
    VertexArray terrainObject;
    terrainObject.createIndexBuffer(terrain.m_Vertices, terrain.m_Indices);
    terrainObject.describeVertexArray(0, 3, GlTypes::Float, 8, GlBool::False, 0);
    terrainObject.describeVertexArray(1, 2, GlTypes::Float, 8, GlBool::False, 3);
    terrainObject.describeVertexArray(2, 3, GlTypes::Float, 8, GlBool::False, 5);

    Shader terrainShader;
    terrainShader.bindShader("./shader/VertexShader.vert");
    terrainShader.bindShader("./shader/FragmentShader.frag");
    //The load functions feels shitty
    Texture texture("./media/level/firstheightmap.jpg");
    texture.load();
    bool running = true;
    glm::vec3 moveClick(0.0f);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    rctx.enableDepthTest();
    Player player;
    player.init();
//    SDL_ShowCursor(SDL_ENABLE);
//    SDL_SetRelativeMouseMode(SDL_FALSE);
    glCullFace(GL_BACK);
    while ( running ) {
//        playerInput.cameraIsometric(moveClick, running);
        playerInput.cameraFPS(running);
        playerInput.m_CameraPosition.z = terrain.getHeight(playerInput.m_CameraPosition.x, playerInput.m_CameraPosition.y);
        playerInput.updateView();
        rctx.clearColor(0.1f, 0.3f, 0.3f, 1.0f);
        rctx.clearColorBuffer();
        rctx.clearDepthBuffer();
        //This is way to much code to render a single object
        terrainShader.activate();
        terrainShader["view"] = playerInput.getView();
        terrainShader["projection"] = playerInput.getProjection();
        glm::mat4 model(1.0f);
        terrainShader["model"] = model;
        terrainShader["ViewPos"] = playerInput.m_CameraPosition;
        terrainObject.bindVertexArray();
        terrainShader["terrain"]=0;
        texture.activate(0);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        rctx.drawIndex(terrainShader,terrain.m_Indices.size());
//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//        player.render(terrain, moveClick, rctx, playerInput);

        rctx.swapBuffers();
    }


}
