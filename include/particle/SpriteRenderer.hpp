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

	private:
		unsigned int shader_, vao_, vbo_, ebo_, texture_;
		void initShaders();
		void initBuffers();
		unsigned loadTexture(const std::string& path);
	};
}