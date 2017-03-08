#include "HeightMap.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <random>
#include <fstream>


bool
HeightMap::LoadHeightMapFromImage( std::string path) {

    std::ifstream heightStream;
    heightStream.open( path, std::ios::binary );
    if ( heightStream.fail() )
    {
        std::cout <<  "Could not open height file.";
        return false;
    }

    // Get number of vertices
    heightStream.seekg( 0, std::ios::end );
    auto m_numVertices = heightStream.tellg();
    heightStream.seekg( 0, std::ios::beg );

    // Allocate memory and read the data
    auto m_pHeight = new unsigned char[m_numVertices];
    heightStream.read( (char *)m_pHeight, m_numVertices );
    heightStream.close();


    iCols = (int)std::sqrt((float)m_numVertices);
    iRows = (int)std::sqrt((float)m_numVertices);

    heightTable.resize(iRows);
    for ( int i = 0; i < iRows; ++i) {
        for (int j = 0; j<iCols; ++j) {

            heightTable[i].resize(iCols);
            float halfWidth = (float)(iCols-1.0f)/2.0f;
            float halfHeight = (float)(iRows-1.0f)/2.0f;

            VertexT temp ;
            heightTable[i][j] = m_pHeight[i*iRows+j]/255.0f;
            temp.Position = glm::vec3(j-halfWidth, i-halfHeight, m_pHeight[i*iRows+j]/255.0f);
            temp.TexPosition = glm::vec2((float)j /(iRows-1), (float)i/ (iCols-1));
            vertices.emplace_back(temp);
        }
    }

    for ( int y = 0; y < iRows -1; ++y) {

        if( y % 2 == 0 ) {
            int x;
            for ( x = 0; x<iCols; ++x) {

               indices.push_back(x+(y*iCols));
               indices.push_back(x+(y*iCols) + iCols);
            }

            if ( y != iRows -2 ) {
                indices.push_back( --x + ( y*iCols));
            }
        }
        else {
            int x;
            for (int x = iCols-1; x >=0; x-- ) {

                indices.push_back(x+(y*iCols));
                indices.push_back(x+(y*iCols) + iCols);
             }

             if ( y != iRows -2 ) {
                 indices.push_back( ++x + ( y*iCols));
             }
        }
    }
    std::cout << "done\n";
}

float
HeightMap::getHeight(float x, float y){
    float terrainX = x+(iCols-1)/2;
    float terrainY = y+(iRows-1)/2;
    int gridX = static_cast<int>(terrainX);
    int gridY = static_cast<int>(terrainY);
    if( gridX >= iCols || gridY >=iRows || gridX < 0 || gridY < 0) return 0.0f;

    float squarePosX = std::abs(std::fmod(terrainX,1.0f));
    float squarePosY = std::abs(std::fmod(terrainY,1.0f));

    float height;
    if (squarePosX <= 1 - squarePosY){
       height = barryCentric( glm::vec3(0, heightTable[gridX][gridY], 0), glm::vec3(1,
                      heightTable[gridX + 1][gridY], 0), glm::vec3(0,
                      heightTable[gridX][gridY + 1], 1), glm::vec2(squarePosX, squarePosY));
    }
    else {
       height = barryCentric(glm::vec3(1, heightTable[gridX + 1][gridY], 0), glm::vec3(1,
                     heightTable[gridX + 1][gridY + 1], 1), glm::vec3(0,
                     heightTable[gridX][gridY + 1], 1), glm::vec2(squarePosX, squarePosY));
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
