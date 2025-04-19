#pragma once
#include "IRenderer.hpp"
#include <vector>

namespace particle {

    class GLRenderer : public IRenderer {
        unsigned int shader_, vao_, vbo_;
    public:
        GLRenderer();
        ~GLRenderer();
        void draw(const std::vector<IParticle*>& pts) override;
    };

} // namespace particle
