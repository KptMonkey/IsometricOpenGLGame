#pragma once
#include "RenderAbstraction/RenderAbstraction.hpp"
#include "Level/HeightMap.hpp"
#include <vector>

class Bullet : public IGlobalRenderable {
public:
    Bullet(glm::vec3 const & shootDirection, glm::vec3 const  playerPos);

    void
    drawShadow(Shader &shader, RenderContext &rctx) override;

    void
    draw(Camera const & camera, RenderContext & rctx) override;

    void
    updatePosition();

    bool
    canBeRemoved();

    glm::vec3 const &
    getPosition() const;

private:
    glm::vec3
    sphereEquation(float u, float v, float r);

    void
    createBulletMesh(std::vector<glm::vec3> & model, int uline, int vline);

    Shader      m_Shader;
    glm::vec3   m_BulletDirection;
    glm::vec3   m_Position;
    glm::mat4   m_ModelMatrix;
    VertexArray m_VertexArray;
    int         m_DeadCounter;
    std::vector<glm::vec3> m_Model;
};
