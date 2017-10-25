
// explode_component.cpp

// source file of the explode component class

#include "explode_component.hpp"
#include <line_renderer.hpp>

#define _USE_MATH_DEFINES

#include <math.h>

using namespace atlas;
using namespace pantheon;

const float PI2 = float( M_PI ) * 2;

void ExplodeComponent::setupVertices() {

	// find triangle points and center point

	glm::vec2 vertices[4] =
		{ glm::vec2( -sin( PI2 * 0.0f ) * 5.0f, cos( PI2 * 0.0f ) * 5.0f )
		, glm::vec2( -sin( PI2 * 0.33f ) * 5.0f, cos( PI2 * 0.33f ) * 5.0f )
		, glm::vec2( 0.0f, 0.0f )
		, glm::vec2( -sin( PI2 * 0.66f ) * 5.0f, cos( PI2 * 0.66f ) * 5.0f ) };
	glm::vec2 normals[4];
	for ( int i = 0; i < 4; ++i ) {

		glm::vec2 temp = glm::normalize( vertices[i == 3 ? 0 : i + 1] - vertices[i] );
		normals[i] = glm::vec2( temp[1], -temp[0] );
	}

	std::copy( vertices, vertices + 4, m_vertices );
	std::copy( normals, normals + 4, m_normals );
}

void ExplodeComponent::render() {

	// get renderer and check it is compatible with player

	LineRenderer* const renderer = Game::GetRendererAs<LineRenderer>();
	if ( renderer == nullptr ) {

		return;
	}

	// create render message and queue to draw

	Transform2D transform = getOwner().getTransform2D();

	glm::vec3 col = glm::vec3( 1.0f, 0.8f, 0.6f ) * ((3.0f - m_timer) / 3.0f);
	float iTimer = 1.0f + 1.0f - (m_timer / 3.0f);
	LineRendererVertex vertices[12];
	for ( int i = 0; i < 4; i++ ) {

		int off = i * 3;
		glm::vec2 vertA = m_vertices[i];
		glm::vec2 vertB = m_vertices[i == 3 ? 0 : i + 1];
		glm::vec2 vecoff = m_normals[i] * m_timer * 3.0f + m_velocity * m_timer * iTimer * 0.5f + glm::normalize( (vertA + vertB) * 0.5f ) * (m_timer + 1.0f);
		vertices[off + 0] = LineRendererVertex{
			vertA[0] + vecoff[0], vertA[1] + vecoff[1], 0.0f, col[0], col[1], col[2]
		};
		vertices[off + 1] = LineRendererVertex{
			vertB[0] + vecoff[0], vertB[1] + vecoff[1], 0.0f, col[0], col[1], col[2]
		};
		vertices[off + 2] = LineRendererVertex::separator();
	}
	LineRendererMessage message{ vertices, vertices + 12, transform.findMatrix() };
	renderer->queueDraw( message );
}