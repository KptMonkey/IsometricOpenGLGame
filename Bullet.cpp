#include "Bullet.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Bullet::Bullet(glm::vec3 const & shootDirection, glm::vec3 const  playerPos)
    : m_BulletPos(playerPos.x, playerPos.y, playerPos.z)
     ,m_BulletDirection(glm::normalize(shootDirection - playerPos))
     ,m_DeadCounter(0)
{
    m_VertexArray.createVertexArray(m_Model);
    m_VertexArray.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
    m_VertexArray.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);

    m_Shader.bindShader("./shader/VertexShader.vert");
    m_Shader.bindShader("./shader/FragmentShader.frag");
}

void
Bullet::updatePosition() {
    m_ModelMatrix=glm::mat4(1.0f);
    m_BulletPos += (m_BulletDirection) * 0.1f;
    m_ModelMatrix = glm::translate(m_ModelMatrix, m_BulletPos);
    ++m_DeadCounter;
}

void
Bullet::drawShadow(Shader &shader, RenderContext &rctx) {
    shader["model"]=m_ModelMatrix;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}

void
Bullet::draw(Camera const & camera, RenderContext & rctx) {
    m_Shader.activate();
    m_Shader["model"] = m_ModelMatrix;
    m_Shader["view"] = camera.View;
    m_Shader["projection"] = camera.Projection;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}

bool
Bullet::canBeRemoved() {
    return m_DeadCounter > 480;
}
