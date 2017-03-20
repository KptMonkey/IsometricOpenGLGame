#include "HeightMap.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <random>
#include <fstream>
#include <random>

HeightMap::HeightMap(std::string path) :
    m_ModelMatrix(glm::mat4(1.0f))

{
    loadHeightMapFromImage(path);

    m_VertexArray.createIndexBuffer(m_Vertices, m_Indices);
    m_VertexArray.describeVertexArray(0, 3, GlTypes::Float, 8, GlBool::False, 0);
    m_VertexArray.describeVertexArray(1, 2, GlTypes::Float, 8, GlBool::False, 3);
    m_VertexArray.describeVertexArray(2, 3, GlTypes::Float, 8, GlBool::False, 5);

    m_GrasArray.createVertexArray(m_GrasPosition);
    m_GrasArray.describeVertexArray(0, 3, GlTypes::Float,3, GlBool::False,0);

    GrasShader.bindShader("./shader/gras.vert");
    GrasShader.bindShader("./shader/gras.geom");
    GrasShader.bindShader("./shader/gras.frag");

    m_Shader.bindShader("./shader/shadowMapFloor.vert");
    m_Shader.bindShader("./shader/shadowMapFloor.frag");

    m_GrasTexture.load2DTexture("./media/level/gras.jpeg");
    m_StoneTexture.load2DTexture("./media/level/stone.jpeg");
}


bool
HeightMap::loadHeightMapFromImage(std::string const & path) {

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.3, 6.0);

    auto image = IMG_Load(path.c_str());
    m_Rows = image->h;
    m_Columns = image->w;
    float halfWidth = (float)(m_Columns)/2.0f;
    float halfHeight = (float)(m_Rows)/2.0f;

    m_Heights.resize(m_Rows);
    for ( int i = 0; i < m_Rows; ++i) {
        for (int j = 0; j<m_Columns; ++j) {

            m_Heights[j].reserve(m_Rows);

            int lineOffSet = i * (image->pitch/4);
            Uint32 pixel = ((Uint32*)image->pixels)[lineOffSet + j];
            Uint8 r, g ,b;
            SDL_GetRGB(pixel,image->format,&r, &g, &b);
            float m_pHeight = static_cast<float>(g/15.0f);
            VertexT temp ;
            m_Heights[j][m_Rows-i-1] = m_pHeight;
            if (m_pHeight < 4.3f)
                m_GrasPosition.push_back(glm::vec3(j-halfWidth, i-halfHeight, m_pHeight));
            else if( m_pHeight >4.3 && m_pHeight < 6 && i%5==0)
                m_GrasPosition.push_back(glm::vec3(j-halfWidth, i-halfHeight, m_pHeight));

            temp.Position = glm::vec3(j-halfWidth, i-halfHeight, m_pHeight);
            temp.TexPosition = glm::vec2(16.0f * (float)j/m_Rows, 16.0f * (float)i/m_Rows);
            auto pos1 = glm::vec3(j-halfWidth, i-halfHeight, m_pHeight);
            glm::vec3 pos2;
            glm::vec3 pos3;
            if( i == m_Rows || j == m_Columns ) {
                pos2 = glm::vec3((j)-halfWidth, (i-1)-halfHeight, m_pHeight);
                pos3 = glm::vec3((j-1)-halfWidth, (i)-halfHeight, m_pHeight);
            }
            else{
                pos2 = glm::vec3((j)-halfWidth, (1+i)-halfHeight, m_pHeight);
                pos3 = glm::vec3((j-1)-halfWidth, (i)-halfHeight, m_pHeight);
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
            for (int x = m_Columns-1; x >=0; x-- ) {

                m_Indices.push_back(x+(y*m_Columns));
                m_Indices.push_back(x+(y*m_Columns) + m_Columns);
             }
        }
    }
}

void
HeightMap::draw(Shader & shader, RenderContext & rctx) {
    //It would be possible to have more terrains......
    shader["model"] = m_ModelMatrix;
    m_VertexArray.bindVertexArray();
    rctx.drawIndex(PrimitiveType::TiangleStrip,m_Indices.size());
}

void
HeightMap::drawTerrain(RenderContext & rctx, Texture & depthTexture, Controller & input, const glm::mat4 &lightView, const glm::vec3 &lightPos) {
    m_Shader.activate();
    m_Shader["model"] = m_ModelMatrix;
    m_Shader["Color"] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    m_Shader["projection"] = input.getProjection();
    m_Shader["viewPos"] = input.m_CameraPosition;
    m_Shader["lightSpace"] = lightView;
    m_Shader["view"] = input.getView();
    m_Shader["lightPos"] = lightPos;

    // Don't forget that the order is important
    m_Shader["shadowMap"]=0;
    depthTexture.activate(0);
    m_Shader["ground"] = 1;
    m_GrasTexture.activate(1);
    m_Shader["groundNormal"] = 2;
    m_StoneTexture.activate(2);

    m_VertexArray.bindVertexArray();
    rctx.drawIndex(PrimitiveType::TiangleStrip,m_Indices.size());

}
