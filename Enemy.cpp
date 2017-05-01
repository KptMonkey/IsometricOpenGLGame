#include "Enemy.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/component_wise.hpp>
Enemy::Enemy(HeightMap &height, glm::vec2 position) :
        m_MovementRadius(2.5f),
        m_Speed(0.005f),
        m_DeltaPosition(0.0f),
        m_Map(height),
        m_AggroRange(30.0f){

    m_VertexArray.createVertexArray(m_Model);
    m_VertexArray.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
    m_VertexArray.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);

    m_Shader.bindShader("./shader/VertexShader.vert");
    m_Shader.bindShader("./shader/FragmentShader.frag");

    m_ModelMatrix=glm::mat4(1.0f);
    m_Position = glm::vec3(position.x, position.y,height.getHeight(position.x, position.y)+2.5f);
    m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(1.5,1.0,2.0f));
    m_Hit = false;
}

void
Enemy::intersectWithBullet(std::list<Bullet> const & bullets) {
    for (auto & bullet : bullets) {
        if (std::abs(bullet.m_BulletPos.x - m_Position.x)< 1.0f && std::abs(bullet.m_BulletPos.y - m_Position.y)< 1.0f
                && std::abs(bullet.m_BulletPos.z - m_Position.z)< 2.0f) {
            m_Hit=true;
        }
    }
}

void
Enemy::drawShadow(Shader &shader, RenderContext &rctx) {
    if(m_Hit) return; // placeholder
    movement();
    shader["model"]=m_ModelMatrix;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}

void
Enemy::draw(Camera const & camera, RenderContext & rctx) {
 if (m_Hit) return;
    movement();
    m_Shader.activate();
    m_Shader["model"] = m_ModelMatrix;
    m_Shader["view"] = camera.View;
    m_Shader["projection"] = camera.Projection;
    m_VertexArray.bindVertexArray();
    rctx.draw(m_VertexArray, PrimitiveType::Triangles);
}

void
Enemy::lookForPlayer(Player & player) {
    if (m_Hit) return;
    float disToPlayer = glm::distance(player.getPosition(), m_Position);
    if (disToPlayer < player.getHitBox()) {
        player.hit();
        return;
    }
    else if (disToPlayer < m_AggroRange) {
        m_PlayerDirection = glm::normalize(player.getPosition() - m_Position);
    }
    else  {
        m_PlayerDirection = glm::vec3(0.0f);
    }
}

void
Enemy::movement() {
    if (glm::compAdd(m_PlayerDirection) != 0.0f)
        huntPlayer();
    else {
        m_Position.x +=     m_Speed;
        m_Position.z =      m_Map.getHeight(m_Position.x, m_Position.y)+0.5f;
        m_DeltaPosition +=  m_Speed;

        glm::mat4 mat(1.0f);
        m_ModelMatrix = glm::translate(mat, m_Position);

        if (std::abs(m_DeltaPosition) > m_MovementRadius )
            m_Speed = -m_Speed;
    }
}

void
Enemy::huntPlayer() {

    m_Position.x += 3 * std::abs(m_Speed) * m_PlayerDirection.x;
    m_Position.y += 3 * std::abs(m_Speed) * m_PlayerDirection.y;
    m_Position.z =  m_Map.getHeight(m_Position.x, m_Position.y)+0.5f;

    glm::mat4 mat(1.0f);
    m_ModelMatrix = glm::translate(mat, m_Position);
}

bool
Enemy::isHit() {
    return m_Hit;
}
