
// explode_component.cpp

// source file of the explode component class

#include "explode_component.hpp"
#include <line_renderer.hpp>

#define _USE_MATH_DEFINES

#include <math.h>

using namespace atlas;
using namespace pantheon;

const float PI2 = float( M_PI ) * 2;

glm::vec2 vertices[4] =
	{ glm::vec2( -sin( PI2 * 0.0f ) * 5.0f, cos( PI2 * 0.0f ) * 5.0f )
	, glm::vec2( -sin( PI2 * 0.33f ) * 5.0f, cos( PI2 * 0.33f ) * 5.0f )
	, glm::vec2( 0.0f, 0.0f )
	, glm::vec2( -sin( PI2 * 0.66f ) * 5.0f, cos( PI2 * 0.66f ) * 5.0f ) };

void ExplodeComponent::render() {

	// get renderer and check it is compatible with player

	LineRenderer* const renderer = Game::GetRendererAs<LineRenderer>();
	if ( renderer == nullptr ) {

		return;
	}

	// create render message and queue to draw

	Transform2D transform = getOwner().getTransform2D();

	glm::vec3 col = glm::vec3( 1.0f, 0.8f, 0.6f ) * ((3.0f - m_timer) / 3.0f);
	float iTimer = 1.0f - (m_timer / 3.0f);
	LineRendererVertex lineVertices[6];
	for ( int i = 0; i < 4; i++ ) {

		glm::vec2 vertA = vertices[i];
		lineVertices[i] = LineRendererVertex{
			vertA[0], vertA[1], 0.0f, col[0], col[1], col[2]
		};
	}
	lineVertices[4] = LineRendererVertex::separator();
	lineVertices[5] = LineRendererVertex::separator();
	transform.position += m_velocity * 0.5f * m_timer;
	transform.scale += glm::vec2( 1.0f, 1.0f ) * (m_timer / 3.0f);
	LineRendererMessage message{ lineVertices, lineVertices + 6, transform.findMatrix() };
	renderer->queueDraw( message );
}