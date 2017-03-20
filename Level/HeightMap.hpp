#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "VertexArray.hpp"
#include "IGlobalRenderable.hpp"
#include "Controller/Controller.hpp"

class HeightMap : public IGlobalRenderable
{
public:

   HeightMap(std::string path);

   bool loadHeightMapFromImage(std::string const & path);

   float
   getHeight(float x, float y);

   float
   barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);

   void
   createIndicies();

   void
   draw(Shader & shader, RenderContext & rctx) override;

   void
   drawTerrain(RenderContext & rctx,
               Texture &depthTexture,
               Controller & input,
               glm::mat4 const & lightView,
               glm::vec3 const & lightPos);

   std::vector<VertexT> m_Vertices;
   std::vector<unsigned int> m_Indices;
   std::vector<glm::vec3> m_GrasPosition;
   VertexArray m_GrasArray;
   Shader GrasShader;

private:

   int m_Rows;
   int m_Columns;
   std::vector< std::vector<float> > m_Heights;
   VertexArray m_VertexArray;
   Shader m_Shader;
   glm::mat4 m_ModelMatrix;
   Texture m_GrasTexture;
   Texture m_StoneTexture;

};
