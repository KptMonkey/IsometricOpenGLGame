#include "HeightMap.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <random>
#include <fstream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>


HeightMap::HeightMap(std::string path) :
    m_ModelMatrix(glm::mat4(1.0f))
  , m_LightPosition(glm::vec3(-160.0f, 160.0f, 300.0f))
  , m_LightProjection(glm::ortho(-320.0f, 320.0f, -320.0f, 320.0f, 250.0f, 700.0f))

{
    loadHeightMapFromImage(path);

    m_VertexArray.createIndexBuffer(m_Vertices, m_Indices);
    m_VertexArray.describeVertexArray(0, 3, GlTypes::Float, 8, GlBool::False, 0);
    m_VertexArray.describeVertexArray(1, 2, GlTypes::Float, 8, GlBool::False, 3);
    m_VertexArray.describeVertexArray(2, 3, GlTypes::Float, 8, GlBool::False, 5);

    m_GrasArray.createVertexArray(m_GrasPosition);
    m_GrasArray.describeVertexArray(0, 3, GlTypes::Float,3, GlBool::False,0);

    m_GrasShader.bindShader("./shader/gras.vert");
    m_GrasShader.bindShader("./shader/gras.geom");
    m_GrasShader.bindShader("./shader/gras.frag");

    m_Shader.bindShader("./shader/shadowMapFloor.vert");
    m_Shader.bindShader("./shader/shadowMapFloor.frag");

    m_GrasTexture.load2DTexture("./media/level/gras.jpeg");
    m_StoneTexture.load2DTexture("./media/level/stone.jpeg");
    m_GrasPatchTexutre.load2DTextureAlpha("./media/level/gras.png");
    m_LightView = glm::lookAt(m_LightPosition,
                              glm::vec3(0.0f, 0.0f, 0.0f),
                              glm::vec3(0.0f, 1.0f, 0.0f));

}


bool
HeightMap::loadHeightMapFromImage(std::string const & path) {

    auto image = IMG_Load(path.c_str());
    m_Rows = image->h;
    m_Columns = image->w;
    float halfWidth = m_Columns * 0.5f;
    float halfHeight = m_Rows * 0.5f;
    m_Heights.resize(m_Rows);

    for (int i = 0; i < m_Rows; ++i) {
        for (int j = 0; j < m_Columns; ++j) {

            m_Heights[j].reserve(m_Rows);

            int lineOffSet = i*(image->pitch/4);
            uint32_t pixel = static_cast<uint32_t*>(image->pixels)[lineOffSet + j];
            uint8_t r, g ,b;
            SDL_GetRGB(pixel,image->format,&r, &g, &b);

            float height = r;
            m_Heights[j][m_Rows-i-1] = height;
            // Set the gras positions
            if (height < 6.0f)
                m_GrasPosition.emplace_back(j-halfWidth, i-halfHeight, height);
            else if (height > 6.0f && height < 10.0f && i%5==0)
                m_GrasPosition.emplace_back(j-halfWidth, i-halfHeight, height);

            VertexT temp ;
            temp.Position = glm::vec3(j-halfWidth, i-halfHeight, height);
            temp.TexPosition = glm::vec2(16.0f * (float)j/m_Rows, 16.0f * (float)i/m_Rows);

            //Calculate normals in an ugly way... FEM would be nicer
            auto pos1 = glm::vec3(j-halfWidth, i-halfHeight, height);
            glm::vec3 pos2;
            glm::vec3 pos3;
            if ( i == m_Rows || j == m_Columns ) {
                pos2 = glm::vec3((j)-halfWidth, (i-1)-halfHeight, height);
                pos3 = glm::vec3((j-1)-halfWidth, (i)-halfHeight, height);
            }
            else {
                pos2 = glm::vec3((j)-halfWidth, (1+i)-halfHeight, height);
                pos3 = glm::vec3((j-1)-halfWidth, (i)-halfHeight, height);
            }
            temp.Normal = glm::cross((pos1-pos2),(pos1-pos3));

            m_Vertices.emplace_back(temp);
        } // Columns
    } // Rows
    createIndicies();
}

float
HeightMap::getHeight(float x, float y) {
    float terrainX = x+(m_Columns)/2;
    float terrainY = (m_Rows)/2 - y;
    int gridX = static_cast<int>(terrainX);
    int gridY = static_cast<int>(terrainY);
    if( gridX >= m_Columns || gridY >=m_Rows || gridX < 0 || gridY < 0) return 0.0f;
    float squarePosX = std::abs(std::fmod(terrainX,1.0f));
    float squarePosY = std::abs(std::fmod(terrainY,1.0f));
    float height;
    if (squarePosX <= 1.0f - squarePosY){
       height = barryCentric( glm::vec3(0, m_Heights[gridX][gridY], 0), glm::vec3(1,
                      m_Heights[gridX + 1][gridY], 0), glm::vec3(0,
                      m_Heights[gridX][gridY + 1], 1), glm::vec2(squarePosX, squarePosY));
    }
    else {
       height = barryCentric(glm::vec3(1, m_Heights[gridX + 1][gridY], 0), glm::vec3(1,
                     m_Heights[gridX + 1][gridY + 1], 1), glm::vec3(0,
                     m_Heights[gridX][gridY + 1], 1), glm::vec2(squarePosX, squarePosY));
    }

    return height;
}

float
HeightMap::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    float l3 = 1.0f - l1 - l2;
    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

void
HeightMap::createIndicies() {
    for ( int y = 0; y < m_Rows -1; ++y) {
        if( y % 2 == 0 ) {
            for ( int x = 0; x<m_Columns; ++x) {
               m_Indices.push_back(x+(y*m_Columns));
               m_Indices.push_back(x+(y*m_Columns) + m_Columns);
            }
        }
        else {
            for (int x = m_Columns-1; x >=0; x--) {
                m_Indices.push_back(x+(y*m_Columns));
                m_Indices.push_back(x+(y*m_Columns) + m_Columns);
             }
        }
    }
}

void
HeightMap::drawShadow(Shader & shader, RenderContext & rctx) {
    shader["model"] = m_ModelMatrix;
    m_VertexArray.bindVertexArray();
    rctx.drawIndex(PrimitiveType::TiangleStrip,m_Indices.size());
}

void
HeightMap::draw(Camera const & camera, RenderContext & rctx) {
    m_Shader.activate();
    m_Shader["model"] = m_ModelMatrix;
    m_Shader["Color"] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    m_Shader["projection"] = camera.Projection;
    m_Shader["viewPos"] = camera.Position;
    m_Shader["lightSpace"] = m_LightProjection* m_LightView;
    m_Shader["view"] = camera.View;
    m_Shader["lightPos"] = m_LightPosition;

    // Don't forget that the order is important
    m_Shader["shadowMap"]=0;
    m_DepthTexture->activate(0);
    m_Shader["ground"] = 1;
    m_GrasTexture.activate(1);
    m_Shader["groundNormal"] = 2;
    m_StoneTexture.activate(2);

    m_VertexArray.bindVertexArray();
    rctx.drawIndex(PrimitiveType::TiangleStrip,m_Indices.size());

    m_GrasShader.activate();
    m_GrasShader["wind"]=0.8f;
    m_GrasShader["model"]=m_ModelMatrix;
    m_GrasShader["view"]=camera.View;
    m_GrasShader["projection"]=camera.Projection;
    m_GrasShader["gras"]=0;
    m_GrasPatchTexutre.activate(0);
    m_GrasArray.bindVertexArray();
    rctx.draw(m_GrasArray, PrimitiveType::Points);
}
