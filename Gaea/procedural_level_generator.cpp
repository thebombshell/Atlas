
// procedural_level_generator.cpp

#include "procedural_level_generator.hpp"

#include "blocks.hpp"
#include "player_prefab.hpp"

#include <sprite_renderer.hpp>

#define _USE_MATH_DEFINES

#include <math.h>

using namespace pantheon;
using namespace gaea;

ProceduralLevelGenerator::ProceduralLevelGenerator( pthn::ConstructComponentPermit& t_permit, pthn::Actor& t_owner )
	: IActorComponent( t_permit, t_owner ) {

	m_player = &Game::GetScene().createPrefab<PlayerPrefab>();
	m_backTrackLimit = -12.0f;
	m_player->getTransform().position = { 0.0f, 1.0f, 0.0f };
	m_ground = &Game::GetScene().createPrefab<BlockPrefab>( glm::vec2( 100.0f, 1.0f ) );
	m_wall = &Game::GetScene().createPrefab<BlockPrefab>( glm::vec2( 1.0f, 16.0f ) );
	m_wall->getTransform().position = { m_backTrackLimit - 8.0f, 8.0f, 0.0f };
	std::vector<glm::vec2> cullVertices
		{ { -0.5f, -0.5f }
		, { -0.5f, 0.5f }
		, { 0.5f, 0.5f }
		, { 0.5f, -0.5f } };
	m_cullBarrier = new ConvexHull( m_ground->getTransform(), cullVertices.begin(), cullVertices.end() );

	m_predictedJumpTime = (PLAYER_JUMP_STRENGTH / WORLD_GRAVITY_STRENGTH) * 2.0f;
	m_predictedJumpDistance = m_predictedJumpTime * PLAYER_TOP_SPEED;
	m_predictedJumpHeight = (PLAYER_JUMP_STRENGTH * PLAYER_JUMP_STRENGTH) / (WORLD_GRAVITY_STRENGTH * 2);

	printf( ("predicted jump time: " + std::to_string( m_predictedJumpTime ) + "s \n").c_str() );
	printf( ("predicted jump distance: " + std::to_string( m_predictedJumpDistance ) + "f \n").c_str() );
	printf( ("predicted jump height: " + std::to_string( m_predictedJumpHeight ) + "f \n").c_str() );

	m_minLocation = { 0.0f, 0.0f };
	m_minLocation = { 12.0f, 0.0f };
}
ProceduralLevelGenerator::~ProceduralLevelGenerator() {

	delete m_cullBarrier;
}

void ProceduralLevelGenerator::update( float t_delta ) {

	handleTracking();
	handleCulling();
	handleProcedure();
}

void ProceduralLevelGenerator::render() {

	SpriteRenderer& renderer = *Game::GetRendererAs<SpriteRenderer>();
	
	float viewPosX = m_player->getTransform().position[0] < m_backTrackLimit ?
		m_backTrackLimit : m_player->getTransform().position[0];
	renderer.setViewPosition( { viewPosX, 3.5f } );
}

float ProceduralLevelGenerator::getPredictedY( float t_alpha ) {

	return sin( t_alpha * static_cast<float>(M_PI) ) * m_predictedJumpHeight;
}

float ProceduralLevelGenerator::getPredictedX( float t_alpha ) {

	return m_predictedJumpDistance * t_alpha;
}

void ProceduralLevelGenerator::handleTracking() {

	if ( m_player->getTransform().position[0] > m_backTrackLimit + 12.0f ) {

		float tickCheck = m_backTrackLimit;
		m_backTrackLimit = m_player->getTransform().position[0] - 12.0f;
		m_wall->getTransform().position = { m_backTrackLimit - 8.0f, 8.0f, 0.0f };
	}
	if ( m_player->getTransform().position[0] > 24.0f ) {

		m_player->getTransform().position[0] -= 48.0f;
		m_wall->getTransform().position[0] -= 48.0f;
		m_backTrackLimit -= 48.0f;
		m_minLocation[0] -= 48.0f;
		m_maxLocation[0] -= 48.0f;
		for ( auto actor : m_levelActors ) {
			
			actor->getTransform().position[0] -= 48.0f;
		}
	}
}

void ProceduralLevelGenerator::handleCulling() {

	std::vector<Actor*> cullList;
	for ( auto& actor : m_levelActors ) {

		if ( actor->getTransform().position[0] < m_backTrackLimit - 12.0f ) {

			cullList.push_back( actor );
		}
	}

	for ( auto& actor : cullList ) {

		m_levelActors.erase( std::find( m_levelActors.begin(), m_levelActors.end(), actor ) );
		Game::GetScene().destroyActor( actor );
	}
}

float random() {

	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

void ProceduralLevelGenerator::handleProcedure() {

	if ( m_maxLocation[0] > m_player->getTransform().position[0] + 12.0f ) {

		return;
	}

	float sampleAlpha = random();
	glm::vec2 jumpPosition = glm::floor( m_minLocation * (1.0f - sampleAlpha) + m_maxLocation * sampleAlpha );
	float jumpAlpha = random();
	float destX = getPredictedX( jumpAlpha );
	float destY = getPredictedY( jumpAlpha * 2.0f );
	if ( jumpAlpha > 0.5f ) {

		destY = -jumpPosition[1] * (1.0f - jumpAlpha) * 2.0f;
	}
	if ( jumpPosition[1] + destY < 0.0001f ) {

		m_minLocation = { jumpPosition[0] + destX + 1.0f, 0.0f };
		m_maxLocation = { jumpPosition[0] + destX + 4.0f, 0.0f };
		return;
	}
	glm::vec2 jumpDestination = jumpPosition + glm::vec2( destX, destY );

	Actor* actor = &Game::GetScene().createPrefab<BlockPrefab>( glm::vec2( 4.0f, 1.0f ) );
	actor->getTransform().position = glm::vec3( jumpDestination, 0.0f ) + glm::vec3( 2.0f, 0.0f, 0.0f );
	m_minLocation = jumpDestination + glm::vec2( 1.0f, 0.0f );
	m_maxLocation = jumpDestination + glm::vec2( 4.0f, 0.0f );

	m_levelActors.push_back( actor );
}