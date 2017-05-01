#include "Collectable.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Collectable::Collectable(HeightMap &height)
    :   m_Map(height),
        m_Hit(false) {

    m_VertexArray.createVertexArray(m_Model);
    m_VertexArray.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
    m_VertexArray.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);

    m_Shader.bindShader("./shader/VertexShader.vert");
    m_Shader.bindShader("./shader/FragmentShader.frag");

    m_CollectablePos =      glm::vec3(5.0f, 5.0f,0.0f);
    m_CollectablePos.z =    height.getHeight(m_CollectablePos.x, m_CollectablePos.y) + 2.5f; // Half size of the cube
    m_ModelMatrix =         glm::mat4(1.0f);
    m_ModelMatrix =         glm::translate(m_ModelMatrix, m_CollectablePos);
    m_ModelMatrix =         glm::scale(m_ModelMatrix, glm::vec3(2.0f,2.0,2.0f));
}

void
Collectable::intersect(Player & player) {
    if (std::abs(player.getPosition().x - 5.0f)< 2.5f && std::abs(player.getPosition().y - 5.0f)< 2.5f ) {
        player.setPowerUp(PowerUp::GROW);
        m_Hit = true;
    }
}

void
Collectable::drawShadow(Shader &shader, RenderContext &rctx) {
    if(m_Hit)
        return;

    shader["model"]=m_ModelMatrix;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}

void
Collectable::draw(Camera const & camera, RenderContext & rctx) {
    if (m_Hit)
        return;

    m_ModelMatrix = glm::rotate(m_ModelMatrix,0.01f,glm::vec3(0.0f,0.0f,1.0f));
    m_Shader.activate();
    m_Shader["model"] = m_ModelMatrix;
    m_Shader["view"] = camera.View;
    m_Shader["projection"] = camera.Projection;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}
