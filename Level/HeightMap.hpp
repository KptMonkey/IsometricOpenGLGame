#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "VertexArray.hpp"
#include "IGlobalRenderable.hpp"

class HeightMap : public IGlobalRenderable
{
public:

   HeightMap(std::string path);

   bool
   loadHeightMapFromImage(std::string const & path);

   float
   getHeight(float x, float y);

   float
   barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);

   void
   createIndicies();

   void
   drawShadow(Shader & shader, RenderContext & rctx) override;


   void
   draw(Camera const & camera, RenderContext & rctx) override;

   glm::mat4 const
   getLightSpace() {
       return m_LightProjection * m_LightView;
   }

   void
   setDepthTexture(Texture & dt) {
       m_DepthTexture = std::make_unique<Texture>(dt);
   }

   std::vector<VertexT> m_Vertices;
   std::vector<unsigned int> m_Indices;
   std::vector<glm::vec3> m_GrasPosition;

private:

   int m_Rows;
   int m_Columns;
   std::vector< std::vector<float> > m_Heights;

   VertexArray m_GrasArray;
   VertexArray m_VertexArray;
   Shader m_GrasShader;
   Shader m_Shader;

   glm::mat4 m_ModelMatrix;
   glm::mat4 m_LightProjection;
   glm::mat4 m_LightView;
   glm::vec3 m_LightPosition;

   Texture m_GrasTexture;
   Texture m_StoneTexture;
   Texture m_GrasPatchTexutre;
   std::unique_ptr<Texture> m_DepthTexture;
};
