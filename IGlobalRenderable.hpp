#pragma once
#include "RenderAbstraction/RenderAbstraction.hpp"
#include "Camera/Camera.hpp"

class IGlobalRenderable {
public:
    virtual void
    drawShadow(Shader & shader, RenderContext & rctx)=0;

    virtual void
    draw(Camera const & camera, RenderContext & rctx)=0;

    virtual ~IGlobalRenderable(){}
};
