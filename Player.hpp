#pragma once
#include "RenderAbstraction/RenderAbstraction.hpp"
#include "Level/HeightMap.hpp"
#include "Bullet.hpp"
#include <list>

enum class PowerUpAttribute {
    GROW
};

class Player : public IGlobalRenderable {
public:

    Player(HeightMap & height, int x, int y);

    void
    drawShadow(Shader &shader, RenderContext &rctx) override;

    void
    draw(Camera const & camera, RenderContext & rctx) override;

    void
    updatePosition();

    void
    shoot(glm::vec3 const & shootDirection);

    bool const
    isHit();

    void
    hit();

    glm::vec3 const &
    getPosition() const;

    float const
    getHitBox();

    glm::vec3
    setNextPosition(glm::vec3 const & nextPosition);

    std::list<Bullet> const &
    getBullets();

    void
    setPowerUp(PowerUpAttribute powerUp);

    void
    clearBullets();


private:
    glm::vec3
    sphereEquation(float u, float v, float r);

    void
    createPlayerMesh(std::vector<glm::vec3> & model, int uline, int vline);

    PowerUpAttribute    m_PowerUp;
    std::list<Bullet>   m_Bullets;
    Shader              m_Shader;
    VertexArray         m_VertexArray;
    glm::mat4           m_ModelMatrix;
    bool                m_Hit;
    float               m_HitBox;
    HeightMap&          m_Map;
    glm::vec3           m_Position;
    glm::vec3           m_NextPosition;
    std::vector<glm::vec3>  m_Model;
};
