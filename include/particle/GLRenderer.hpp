#pragma once
#include "IRenderer.hpp"
#include <vector>

namespace particle {

    class GLRenderer : public IRenderer {
        unsigned int shader_, vao_, vbo_;
        BlendMode blendMode_ = BlendMode::Normal;
    public:
        GLRenderer();
        ~GLRenderer();
        void draw(const std::vector<IParticle*>& pts) override;
        void setBlendMode(BlendMode mode) override {
            blendMode_ = mode;
        }
    };

} // namespace particle
