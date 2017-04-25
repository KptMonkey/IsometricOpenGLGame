#include "Enemy.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void
Enemy::init(HeightMap & height) {

    m_VertexArray.createVertexArray(m_Model);
    m_VertexArray.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
    m_VertexArray.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);

    m_Shader.bindShader("./shader/VertexShader.vert");
    m_Shader.bindShader("./shader/FragmentShader.frag");

    m_ModelMatrix=glm::mat4(1.0f);
    m_EnemyPos = glm::vec3(-5.0f, -2.0f,0.0f);
    m_EnemyPos.z = height.getHeight(m_EnemyPos.x, m_EnemyPos.y) + 2.5f; // Half size of the cube
    m_ModelMatrix = glm::translate(m_ModelMatrix, m_EnemyPos);
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(1.5,1.0,2.0f));
    m_Hit = false;

}
void
Enemy::intersect(std::vector<Bullet> &bullets) {
    for (auto & bullet : bullets) {
        if (std::abs(bullet.m_BulletPos.x - m_EnemyPos.x)< 1.0f && std::abs(bullet.m_BulletPos.y - m_EnemyPos.y)< 1.0f
                && std::abs(bullet.m_BulletPos.z - m_EnemyPos.z)< 2.0f) {
            m_Hit=true;
        }
    }
}

void
Enemy::drawShadow(Shader &shader, RenderContext &rctx) {
    if(m_Hit) return; // placeholder
    shader["model"]=m_ModelMatrix;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}

void
Enemy::draw(Camera const & camera, RenderContext & rctx) {
 if (m_Hit) return;
    m_Shader.activate();
    m_Shader["model"] = m_ModelMatrix;
    m_Shader["view"] = camera.View;
    m_Shader["projection"] = camera.Projection;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}
