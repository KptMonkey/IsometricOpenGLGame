#include "Player.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void
Player::init() {

    m_PlayerVA.createVertexArray(m_Model);
    m_PlayerVA.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
    m_PlayerVA.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);

    m_PlayerShader.bindShader("./shader/VertexShader.vert");
    m_PlayerShader.bindShader("./shader/FragmentShader.frag");

}

void
Player::render(HeightMap & height, glm::vec3 clickPos, RenderContext & ctx, Controller & control) {
    m_PlayerShader.activate();
    glm::mat4 model(1.0f);
    m_PlayerPos += (clickPos - m_PlayerPos) * 0.01f;
    m_PlayerPos.z = height.getHeight(m_PlayerPos.x, m_PlayerPos.y) + 0.50f; // Half size of the cube
    control.m_CameraPosition = glm::vec3(m_PlayerPos.x, m_PlayerPos.y, m_PlayerPos.z + 60.0f);
    control.updateView();
//    std::cout << m_PlayerPos.z << "\n";
//    std::cout <<"X: " << m_PlayerPos.x << "\n";
//    std::cout <<"Y: " << m_PlayerPos.y << "\n";
    model = glm::translate(model, m_PlayerPos);
    m_PlayerShader["model"] = model;
    m_PlayerShader["uColor"] = glm::vec4(0.0f, 1.0f, 0.5f, 1.0f);
    m_PlayerShader["projection"] = control.getProjection();
    m_PlayerShader["view"] = control.getView();
    m_PlayerVA.bindVertexArray();
    ctx.draw(m_PlayerVA, PrimitiveType::Triangles);
}
