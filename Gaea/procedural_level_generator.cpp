
// procedural_level_generator.cpp

#include "procedural_level_generator.hpp"

#include "blocks.hpp"
#include "player_prefab.hpp"
#include "text_helper.hpp"

#include <sprite_renderer.hpp>

#define _USE_MATH_DEFINES

#include <math.h>

using namespace pantheon;
using namespace gaea;

glm::vec2 worldToHomo( const glm::vec2& t_vec ) {

	SpriteRenderer& renderer = *Game::GetRendererAs<SpriteRenderer>();
	glm::vec2 vec = t_vec - renderer.getPosition();
	glm::vec2 win = getWindowSize();
	float dist = renderer.getViewDistance();
	glm::vec2 world = glm::vec2( dist * (win[0] / win[1]), dist );
	return (vec / world) + glm::vec2( 0.5f, 0.5f );
}

glm::vec2 screenToHomo(const glm::vec2& t_vec) {

	return t_vec / getWindowSize();
}

glm::vec2 homoToWorld( const glm::vec2& t_vec ) {

	SpriteRenderer& renderer = *Game::GetRendererAs<SpriteRenderer>();
	glm::vec2 vec = t_vec - glm::vec2( 0.5f, 0.5f );
	glm::vec2 win = getWindowSize();
	float dist = renderer.getViewDistance();
	glm::vec2 world = glm::vec2( dist * (win[0] / win[1]), dist );
	return vec * world + renderer.getPosition();
}

glm::vec2 homoToScreen( const glm::vec2& t_vec ) {

	return t_vec * getWindowSize();
}

///////////////////////////////////////////////////////////////////////////////
// ToggleTagOption
///////////////////////////////////////////////////////////////////////////////

ToggleTagOption::ToggleTagOption( int t_index, std::string t_name )
	: m_index{ t_index }, m_name{ t_name } {

	m_transform = { homoToWorld( { 0.0f, 1.0f } ) - (static_cast<float>(t_index + 1) * glm::vec2( 0.0f, 0.5f )), { 0.5f, 0.5f }, 0.0f };
	m_min = worldToHomo( m_transform.position );
	m_max = worldToHomo( m_transform.position + glm::vec2( t_name.size() * 0.5f, 0.5f ) );
	m_isEnabled = false;
}

ToggleTagOption::~ToggleTagOption() {

}

float ToggleTagOption::getValue() {

	return m_isEnabled ? 1.0f : 0.0f;
}

void ToggleTagOption::update() {

	const Input& input = Game::GetInput();
	glm::vec2 mousePosition = screenToHomo( input.getMousePosition() );
	mousePosition[1] = 1.0f - mousePosition[1];
	if ( mousePosition[0] < m_max[0] && mousePosition[0] > m_min[0]
		&& mousePosition[1] < m_max[1] && mousePosition[1] > m_min[1] ) {

		m_isMouseOver = true;
	}
	else {

		m_isMouseOver = false;
	}
	bool isMouseDown = input.getAxisValue( "mouse_button1" ) != 0.0f;
	if ( m_isMouseOver && isMouseDown && !m_wasMouseDown ) {

		m_isEnabled = !m_isEnabled;
	}
	m_wasMouseDown = isMouseDown;
}

void ToggleTagOption::render() {

	SpriteRenderer& renderer = *Game::GetRendererAs<SpriteRenderer>();

	m_transform.position = homoToWorld( { 0.0f, 1.0f } ) - (static_cast<float>(m_index + 1)	* glm::vec2( 0.0f, 0.5f ));
	drawText( m_name + (m_isEnabled ? " enabled" : " disabled")
		, m_isMouseOver ? glm::vec3( 999.0f, 999.0f, 999.0f ) : glm::vec3( 1.0f, 1.0f, 1.0f )
		, m_transform.findMatrix() );

}

///////////////////////////////////////////////////////////////////////////////
// ProceduralLevelGenerator
///////////////////////////////////////////////////////////////////////////////

ProceduralLevelGenerator::ProceduralLevelGenerator( pthn::ConstructComponentPermit& t_permit
	, pthn::Actor& t_owner ) : IActorComponent( t_permit, t_owner ) {

	m_player = &Game::GetScene().createPrefab<PlayerPrefab>();
	m_backTrackLimit = -12.0f;
	m_player->getTransform().position = { 0.0f, 1.0f, 0.0f };
	m_ground = &Game::GetScene().createPrefab<BlockPrefab>( glm::vec2( 100.0f, 1.0f ) );
	m_ground->getTransform().position = { -50.0f, 0.0f, 0.0f };
	m_ground->getComponent<BlockPrefab>().setupMessages();
	m_wall = &Game::GetScene().createPrefab<BlockPrefab>( glm::vec2( 1.0f, 32.0f ) );
	m_wall->getTransform().position = { m_backTrackLimit - 4.0f, 0.0f, 0.0f };
	m_wall->getComponent<BlockPrefab>().setupMessages();

	m_predictedJumpTime = (PLAYER_JUMP_STRENGTH / WORLD_GRAVITY_STRENGTH) * 2.0f;
	m_predictedJumpDistance = m_predictedJumpTime * PLAYER_TOP_SPEED;
	m_predictedJumpHeight 
		= (PLAYER_JUMP_STRENGTH * PLAYER_JUMP_STRENGTH) / (WORLD_GRAVITY_STRENGTH * 2);

	printf( ("predicted jump time: " 
		+ std::to_string( m_predictedJumpTime ) + "s \n").c_str() );
	printf( ("predicted jump distance: " 
		+ std::to_string( m_predictedJumpDistance ) + "f \n").c_str() );
	printf( ("predicted jump height: " 
		+ std::to_string( m_predictedJumpHeight ) + "f \n").c_str() );

	m_minLocation = { 0.0f, 0.0f };
	m_minLocation = { 12.0f, 0.0f };

	m_qualityControl = new ToggleTagOption( 0, "quality control" );
	m_snapToGrid = new ToggleTagOption( 0, "snap to grid" );
	m_heightLimit = new ToggleTagOption( 1, "height limit" );
	m_minimumJump = new ToggleTagOption( 2, "minimum jump" );
	m_variablePlatforms = new ToggleTagOption( 3, "variable platforms" );
	m_weightedVerticality = new ToggleTagOption( 4, "weighted verticality" );
	m_options.push_back( m_snapToGrid );
	m_options.push_back( m_heightLimit );
	m_options.push_back( m_minimumJump );
	m_options.push_back( m_variablePlatforms );
	m_options.push_back( m_weightedVerticality );
}
ProceduralLevelGenerator::~ProceduralLevelGenerator() {

	for ( auto option : m_options ) {

		delete option;
	}
}

void ProceduralLevelGenerator::update( float t_delta ) {

	handleTracking();
	handleCulling();
	handleProcedure();

	for ( auto option : m_options ) {

		option->update();
	}
}

void ProceduralLevelGenerator::render() {

	SpriteRenderer& renderer = *Game::GetRendererAs<SpriteRenderer>();
	
	float viewPosX = m_player->getTransform().position[0] < m_backTrackLimit ?
		m_backTrackLimit : m_player->getTransform().position[0];
	renderer.setViewPosition( { viewPosX, 3.5f } );
	renderer.setViewSize( 12.0f );

	for ( auto option : m_options ) {

		option->render();
	}
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
		m_wall->getTransform().position = { m_backTrackLimit - 4.0f, 0.0f, 0.0f };
		m_wall->getComponent<BlockPrefab>().setupMessages();
	}
	if ( m_player->getTransform().position[0] > 24.0f ) {

		m_player->getTransform().position[0] -= 48.0f;
		m_wall->getTransform().position[0] -= 48.0f;
		m_wall->getComponent<BlockPrefab>().setupMessages();
		m_backTrackLimit -= 48.0f;
		m_minLocation[0] -= 48.0f;
		m_maxLocation[0] -= 48.0f;
		for ( auto actor : m_levelActors ) {
			
			actor->getTransform().position[0] -= 48.0f;
			if ( actor->hasComponent<BlockPrefab>() ) {

				actor->getComponent<BlockPrefab>().setupMessages();
			}
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
	if ( m_minimumJump->getValue() > 0.001f ) {

		jumpAlpha = jumpAlpha * 0.5f + 0.25f;
	}
	float destX = getPredictedX( jumpAlpha );
	float destY = getPredictedY( jumpAlpha * 2.0f );
	if ( m_weightedVerticality->getValue() > 0.001f ) {

		destY = getPredictedY( jumpAlpha );
		float weight = m_maxLocation[1] / 6.0f;
		if ( random() <= weight ) {

			destY = -jumpPosition[1] * (1.0f - jumpAlpha);
		}
	}
	else {
	
		if ( jumpAlpha > 0.5f ) {

			destY = -jumpPosition[1] * (1.0f - jumpAlpha) * 2.0f;
		}
	}
	if ( jumpPosition[1] + destY < 0.0001f ) {

		m_minLocation = { jumpPosition[0] + destX + 1.0f, 0.0f };
		m_maxLocation = { jumpPosition[0] + destX + 4.0f, 0.0f };
		return;
	}
	glm::vec2 jumpDestination = jumpPosition + glm::vec2( destX, destY );
	if ( m_snapToGrid->getValue() > 0.001f ) {

		jumpDestination = jumpPosition + glm::vec2( floor(destX), floor(destY) );
	}
	if ( m_heightLimit->getValue() > 0.001f ) {

		jumpDestination[1] = jumpDestination[1] > 6.0f ? 6.0f : jumpDestination[1];
	}
	float scale = 4.0f;
	if ( m_variablePlatforms->getValue() > 0.001f ) {

		scale = 1.0f + floor( random() * 8.0f );
	}
	Actor* actor = &Game::GetScene().createPrefab<BlockPrefab>( glm::vec2( scale, 1.0f ) );
	actor->getTransform().position = glm::vec3( jumpDestination, 0.0f ) + glm::vec3( 2.0f, 0.0f, 0.0f );
	actor->getComponent<BlockPrefab>().setupMessages();

	m_minLocation = jumpDestination + glm::vec2( 1.0f, 0.0f );
	m_maxLocation = jumpDestination + glm::vec2( scale, 0.0f );

	m_levelActors.push_back( actor );
}