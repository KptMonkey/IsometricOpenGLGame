#include "HeightMap.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <random>
#include <fstream>


bool
HeightMap::loadHeightMapFromImage(std::string path) {

    auto image = IMG_Load(path.c_str());
    m_Rows = image->h;
    m_Columns = image->w;

    m_Heights.resize(m_Rows);
    for ( int i = 0; i < m_Rows; ++i) {
        for (int j = 0; j<m_Columns; ++j) {

            m_Heights[i].resize(m_Columns);
            float halfWidth = (float)(m_Columns-1.0f)/2.0f;
            float halfHeight = (float)(m_Rows-1.0f)/2.0f;

            int lineOffSet = i * (image->pitch/4);
            Uint32 pixel = ((Uint32*)image->pixels)[lineOffSet + j];
            Uint8 r, g ,b;
            SDL_GetRGB(pixel,image->format,&r, &g, &b);
            float m_pHeight = static_cast<float>(g/15.0f);
            VertexT temp ;
            m_Heights[i][j] = m_pHeight;
            std::cout << "i, j: " << i << " " << j << " " << m_pHeight << "\n";
            temp.Position = glm::vec3(j-halfWidth, i-halfHeight, m_pHeight);
            temp.TexPosition = glm::vec2((float)j /(m_Rows-1), (float)i/ (m_Columns-1));
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
    float terrainX = x+(m_Columns-1)/2;
    float terrainY = (m_Rows-1)/2 - y;
    int gridX = static_cast<int>(terrainX);
    int gridY = static_cast<int>(terrainY);
    std::cout << "Height:" << m_Heights[gridX][gridY] << "\n";
    std::cout << "Grid: " << gridX << " " << gridY << "\n";
    if( gridX >= m_Columns || gridY >=m_Rows || gridX < 0 || gridY < 0) return 0.0f;

    float squarePosX = std::abs(std::fmod(terrainX,1.0f));
    float squarePosY = std::abs(std::fmod(terrainY,1.0f));
//    std::cout << "squarePos: " << squarePosX << " " << squarePosY << "\n";
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
