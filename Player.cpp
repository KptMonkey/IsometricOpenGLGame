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
Player::updatePosition(HeightMap & height, glm::vec3 clickPos, Controller &control) {

    m_ModelMatrix=glm::mat4(1.0f);
    m_PlayerPos += (clickPos - m_PlayerPos) * 0.01f;
    m_PlayerPos.z = height.getHeight(m_PlayerPos.x, m_PlayerPos.y) + 5.0f; // Half size of the cube
    control.m_CameraPosition = glm::vec3(m_PlayerPos.x, m_PlayerPos.y, m_PlayerPos.z + 20.0f);
    control.updateView();
    m_ModelMatrix = glm::translate(m_ModelMatrix, m_PlayerPos);
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(3.0f));
}

void
Player::draw(Shader &shader, RenderContext &rctx) {
    shader["model"]=m_ModelMatrix;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}

void
Player::drawPlayer(RenderContext & rctx, Controller & input) {
    m_Shader.activate();
    m_Shader["model"] = m_ModelMatrix;
    m_Shader["view"] = input.getView();
    m_Shader["projection"] = input.getProjection();
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}
