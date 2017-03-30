#include "Bullet.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Bullet::Bullet(glm::vec3 const & shootDirection, glm::vec3 const  playerPos)
    : m_BulletPos(playerPos.x, playerPos.y, playerPos.z)
     ,m_BulletDirection(glm::normalize(glm::vec3(shootDirection.x, shootDirection.y,0.0f) -glm::vec3(playerPos.x, playerPos.y, 0.0f)))
{
    std::cout << "Bullet: " <<shootDirection.x << " " << shootDirection.y <<std::endl;
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
//    m_BulletPos += (glm::vec3(1.0f,0.0f,0.0f)) * 0.1f;
    m_ModelMatrix = glm::translate(m_ModelMatrix, m_BulletPos);
//    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(0.5f));
}

void
Bullet::draw(Shader &shader, RenderContext &rctx) {
    shader["model"]=m_ModelMatrix;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}

void
Bullet::drawBullet(RenderContext & rctx, glm::mat4 const & view, glm::mat4 const & projection) {
    m_Shader.activate();
    m_Shader["model"] = m_ModelMatrix;
    m_Shader["view"] = view;
    m_Shader["projection"] = projection;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}
