#pragma once
#include "RenderAbstraction/RenderAbstraction.hpp"

class IGlobalRenderable {
public:
    virtual void
    draw(Shader & shader, RenderContext & rctx)=0;

    virtual ~IGlobalRenderable(){}
};
