#include "Player.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Player::Player(HeightMap &height) :
    m_Hit(false),
    m_HitBox(5.0f),
    m_Map(height) {

    m_VertexArray.createVertexArray(m_Model);
    m_VertexArray.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
    m_VertexArray.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);

    m_Shader.bindShader("./shader/VertexShader.vert");
    m_Shader.bindShader("./shader/FragmentShader.frag");
}

void
Player::updatePosition() {
    m_ModelMatrix=glm::mat4(1.0f);
    m_Position += (m_NextPosition - m_Position) * 0.01f;
    m_Position.z = m_Map.getHeight(m_Position.x, m_Position.y) + 2.5f; // Half size of the cube
    m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(3.0f));
    switch(m_PowerUp) {
        case PowerUp::GROW :
            m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(2.0f));
    }
}

void
Player::drawShadow(Shader &shader, RenderContext &rctx) {
    if (m_Hit) return;

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
    if (m_Hit) return;

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
    m_Bullets.emplace_back(shootDirection, m_Position);
}

bool const
Player::isHit() {
    return m_Hit;
}

void
Player::hit() {
    m_Hit = true;
}

glm::vec3 const &
Player::getPosition() const {
    return m_Position;
}

float const
Player::getHitBox() {
    return m_HitBox;
}

glm::vec3
Player::setNextPosition(glm::vec3 const & nextPosition) {
    m_NextPosition = nextPosition;
}

std::vector<Bullet> const &
Player::getBullets() {
    return m_Bullets;
}

void
Player::setPowerUp(PowerUp powerUp) {
    m_PowerUp = powerUp;
}
