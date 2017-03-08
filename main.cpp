#include "RenderAbstraction.hpp"
#include "Controller/Controller.hpp"
#include "Level/HeightMap.hpp"

int main() {

    //TODO: Create player, level classes which draw them selves
    RenderContext rctx;
    Controller playerInput;
    HeightMap terrain;

    terrain.LoadHeightMapFromImage("./media/level/heightmap.r16");
    // GlTypes::Float is even more text than GL_Float
    VertexArray terrainObject;
    terrainObject.createIndexBuffer(terrain.vertices, terrain.indices);
    terrainObject.describeVertexArray(0, 3, GlTypes::Float, 5, GlBool::False, 0);
    terrainObject.describeVertexArray(1, 2, GlTypes::Float, 5, GlBool::False, 3);

    Shader terrainShader;
    terrainShader.bindShader("./shader/VertexShader.vert");
    terrainShader.bindShader("./shader/FragmentShader.frag");
    //The load functions feels shitty
    Texture texture("./media/level/colormap.bmp");
    texture.load();
    bool running = true;
    glm::vec3 moveClick(0.0f);
    while ( running ) {
        playerInput.cameraIsometric(moveClick, running);
        rctx.clearColor(0.1f, 0.3f, 0.3f, 1.0f);
        rctx.clearColorBuffer();
        rctx.clearDepthBuffer();
        //This is way to much code to render a single object
        terrainShader.activate();
        terrainShader["view"] = playerInput.getView();
        terrainShader["projection"] = playerInput.getProjection();
        glm::mat4 model(1.0f);
        terrainShader["model"] = model;
        terrainObject.bindVertexArray();
        terrainShader["terrain"]=0;
        texture.activate(0);
        rctx.drawIndex(terrainShader,terrain.indices.size());
        rctx.swapBuffers();
    }


}
