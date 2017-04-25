#include "Player.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void
Player::init() {

    m_VertexArray.createVertexArray(m_Model);
    m_VertexArray.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
    m_VertexArray.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);

    m_Shader.bindShader("./shader/VertexShader.vert");
    m_Shader.bindShader("./shader/FragmentShader.frag");

}

void
Player::updatePosition(HeightMap & height, glm::vec3 clickPos) {

    m_ModelMatrix=glm::mat4(1.0f);
    m_PlayerPos += (clickPos - m_PlayerPos) * 0.01f;
    m_PlayerPos.z = height.getHeight(m_PlayerPos.x, m_PlayerPos.y) + 2.5f; // Half size of the cube
    m_ModelMatrix = glm::translate(m_ModelMatrix, m_PlayerPos);
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(3.0f));
    switch(m_powerUp) {
        case PowerUp::GROW :
            m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(2.0f));
    }
}

void
Player::drawShadow(Shader &shader, RenderContext &rctx) {
    shader["model"]=m_ModelMatrix;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
    for(auto  & bullet : m_Bullets) {
        bullet.updatePosition();
        bullet.drawShadow(shader,rctx);
    }

}

void
Player::draw(Camera const & camera, RenderContext & rctx) {
    m_Shader.activate();
    m_Shader["model"] = m_ModelMatrix;
    m_Shader["view"] = camera.View;
    m_Shader["projection"] = camera.Projection;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
    for(auto  & bullet : m_Bullets) {
        bullet.updatePosition();
        bullet.draw(camera, rctx);
    }
}

void
Player::shoot(const glm::vec3 &shootDirection) {
    m_Bullets.emplace_back(shootDirection, m_PlayerPos);
}
