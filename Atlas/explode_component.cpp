
// explode_component.cpp

// source file of the explode component class

#include "explode_component.hpp"
#include <line_renderer.hpp>

using namespace atlas;
using namespace pantheon;

void ExplodeComponent::render() {

	// get renderer and check it is compatible with player

	LineRenderer* const renderer = Game::GetRendererAs<LineRenderer>();
	if ( renderer == nullptr ) {

		return;
	}

	// create render message and queue to draw

	Transform& transform = getOwner().getTransform();

	glm::vec3 col = glm::vec3( 1.0f, 0.8f, 0.6f ) * ((3.0f - m_timer) / 3.0f);
	glm::vec3 pos = transform.position + glm::vec3(m_velocity, 0.0f) * (m_timer / 3.0f);
	glm::vec2 vel = m_velocity * 0.16f;
	float sca = m_scale * 5.0f * (0.16f + m_timer);
	float tim = m_timer;
	LineRendererVertex vertices[15]
		{ { cos( 1.25f ) * sca + pos[0] + vel[0], sin( 1.25f ) * sca + pos[1] + vel[1], 0.0f, col[0], col[1], col[2] }
		, { cos( 1.25f ) * (sca + tim) + pos[0] + vel[0] * 2.0f, sin( 1.25f ) * (sca + tim) + pos[1] + vel[1] * 2.0f, 0.0f, col[0], col[1], col[2] }
		, LineRendererVertex::separator()
		, { cos( 2.5f ) * sca + pos[0] + vel[0], sin( 2.5f ) * sca + pos[1] + vel[1], 0.0f, col[0], col[1], col[2] }
		, { cos( 2.5f ) * (sca + tim) + pos[0] + vel[0] * 2.0f, sin( 2.5f ) * (sca + tim) + pos[1] + vel[1] * 2.0f, 0.0f, col[0], col[1], col[2] }
		, LineRendererVertex::separator()
		, { cos( 3.75f ) * sca + pos[0] + vel[0], sin( 3.75f ) * sca + pos[1] + vel[1], 0.0f, col[0], col[1], col[2] }
		, { cos( 3.75f ) * (sca + tim) + pos[0] + vel[0] * 2.0f, sin( 3.75f ) * (sca + tim) + pos[1] + vel[1] * 2.0f, 0.0f, col[0], col[1], col[2] }
		, LineRendererVertex::separator()
		, { cos( 5.0f ) * sca + pos[0] + vel[0], sin( 5.0f ) * sca + pos[1] + vel[1], 0.0f, col[0], col[1], col[2] }
		, { cos( 5.0f ) * (sca + tim) + pos[0] + vel[0] * 2.0f, sin( 5.0f ) * (sca + tim) + pos[1] + vel[1] * 2.0f, 0.0f, col[0], col[1], col[2] }
		, LineRendererVertex::separator()
		, { cos( 6.25f ) * sca + pos[0] + vel[0], sin( 6.25f ) * sca + pos[1] + vel[1], 0.0f, col[0], col[1], col[2] }
		, { cos( 6.25f ) * (sca + tim) + pos[0] + vel[0] * 2.0f, sin( 6.25f ) * (sca + tim) + pos[1] + vel[1] * 2.0f, 0.0f, col[0], col[1], col[2] }
		, LineRendererVertex::separator() };
	LineRendererMessage message{ vertices, vertices + 15 };
	renderer->queueDraw( message );
}