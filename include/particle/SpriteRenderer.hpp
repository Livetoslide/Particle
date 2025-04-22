#pragma once
#include "IRenderer.hpp"
#include <glm.hpp>
#include <vector>
#include <string>


namespace particle {

	class SpriteRenderer : public IRenderer {
	public:
		SpriteRenderer(const std::string& texturePath);
		~SpriteRenderer();

		void draw(const std::vector<IParticle*>& pts) override;
		unsigned int getShader() { return shader_; };

		void setBlendMode(BlendMode mode) override {
			blendMode_ = mode;
		}

	private:
		unsigned int shader_, vao_, vbo_, ebo_, texture_;
		BlendMode blendMode_ = BlendMode::Normal;
		void initShaders();
		void initBuffers();
		unsigned loadTexture(const std::string& path);
	};
}