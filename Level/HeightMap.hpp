#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "VertexArray.hpp"

class HeightMap
{
public:

   HeightMap(){}

   bool loadHeightMapFromImage(std::string path);

   float
   getHeight(float x, float y);

   float
   barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);

   void
   createIndicies();

   std::vector<VertexT> m_Vertices;
   std::vector<unsigned int> m_Indices;

private:

   int m_Rows;
   int m_Columns;
   std::vector< std::vector<float> > m_Heights;
};
