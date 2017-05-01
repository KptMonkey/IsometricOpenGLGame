#include "Player.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Player::Player(HeightMap &height, int x, int y) :
    m_Hit(false),
    m_HitBox(3.0f),
    m_Map(height),
    m_Position(glm::vec3(x, y, m_Map.getHeight(x, y))),
    m_NextPosition(m_Position) {

    createPlayerMesh(m_Model,20,20);
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
    m_ModelMatrix = glm::rotate(m_ModelMatrix,0.01f,glm::vec3(0.0f,0.0f,1.0f));
    switch(m_PowerUp) {
        case PowerUpAttribute::GROW :
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
    clearBullets();
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

std::list<Bullet> const &
Player::getBullets() {
    return m_Bullets;
}

void
Player::setPowerUp(PowerUpAttribute powerUp) {
    m_PowerUp = powerUp;
}

void
Player::clearBullets() {
    if (m_Bullets.empty())
        return;

    if ( m_Bullets.front().canBeRemoved())
        m_Bullets.pop_front();
}

void
Player::createPlayerMesh(std::vector<glm::vec3> &model, int uline, int vline) {
    int startU=0;
    int startV=0;
    float endU=3.14f*2.0f;
    float endV=3.14f;
    float stepU=(endU-startU)/uline;
    float stepV=(endV-startV)/vline;
    for (int i=0;i<uline;i++) {
        for (int j=0;j<vline;j++) {
            float u=i*stepU+startU;
            float v=j*stepV+startV;
            float un=(i+1==uline) ? endU : (i+1)*stepU+startU;
            float vn=(j+1==vline) ? endV : (j+1)*stepV+startV;
            glm::vec3 p0=sphereEquation(u,v, 1.5f);
            glm::vec3 p1=sphereEquation(u, vn, 1.5f);
            glm::vec3 p2=sphereEquation(un, v, 1.5f);
            glm::vec3 p3=sphereEquation(un, vn, 1.5f);

             // Like a donkey....
             model.emplace_back(p0);
             model.emplace_back(glm::normalize(p0));
             model.emplace_back(p2);
             model.emplace_back(glm::normalize(p2));
             model.emplace_back(p1);
             model.emplace_back(glm::normalize(p1));

             model.emplace_back(p3);
             model.emplace_back(glm::normalize(p3));
             model.emplace_back(p1);
             model.emplace_back(glm::normalize(p1));
             model.emplace_back(p2);
             model.emplace_back(glm::normalize(p2));
         }
    }
}

glm::vec3
Player::sphereEquation(float u, float v, float r){
    return glm::vec3(std::cos(u)*std::sin(v)*r, std::cos(v)*r, std::sin(u)*std::sin(v)*r);
}

