#include "Bullet.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Bullet::Bullet(glm::vec3 const & shootDirection, glm::vec3 const  playerPos)
    : m_Position(playerPos.x, playerPos.y, playerPos.z)
     ,m_BulletDirection(glm::normalize(shootDirection - playerPos))
     ,m_DeadCounter(0) {
    createBulletMesh(m_Model,10,10);
    m_VertexArray.createVertexArray(m_Model);
    m_VertexArray.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
    m_VertexArray.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);

    m_Shader.bindShader("./shader/VertexShader.vert");
    m_Shader.bindShader("./shader/FragmentShader.frag");
}

void
Bullet::updatePosition() {
    m_ModelMatrix=glm::mat4(1.0f);
    m_Position += (m_BulletDirection) * 0.1f;
    m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(0.5f, 1.0f, 0.5f));
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

void
Bullet::createBulletMesh(std::vector<glm::vec3> &model, int uline, int vline) {
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
            glm::vec3 p0=sphereEquation(u,v, 0.3f);
            glm::vec3 p1=sphereEquation(u, vn, 0.3f);
            glm::vec3 p2=sphereEquation(un, v, 0.3f);
            glm::vec3 p3=sphereEquation(un, vn, 0.3f);

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

glm::vec3 const &
Bullet::getPosition() const {
    return m_Position;}

glm::vec3
Bullet::sphereEquation(float u, float v, float r){
    return glm::vec3(std::cos(u)*std::sin(v)*r, std::cos(v)*r, std::sin(u)*std::sin(v)*r);
}

