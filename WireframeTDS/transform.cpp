
// transform.cpp

// Source file for the transformation class

#include "transform.hpp"
#include <gtx\matrix_transform_2d.hpp>
#include <gtx\rotate_vector.hpp>

using namespace atlas;

Transform::Transform(
	) {

}

Transform::Transform(
	glm::vec2 t_position
	) : position( t_position ) {

}

Transform::Transform(
	glm::vec2 t_position,
	float t_rotation
	) : position( t_position ), rotation( t_rotation ) {

}

Transform::Transform(
	glm::vec2 t_position,
	float t_rotation,
	glm::vec2 t_scale
	) : position( t_position ), rotation( t_rotation ), scale( t_scale ) {

}

glm::mat3 Transform::findMatrix(
	) const {

	glm::mat3 output;
	output = glm::scale( output, scale );
	output = glm::rotate( output, rotation );
	output = glm::translate( output, position );
	return output;
}

glm::vec2 Transform::findUp(
	) const {

	return glm::rotate( glm::vec2( 0.0f, 1.0f ), rotation );
}

glm::vec2 Transform::findRight(
	) const {

	return glm::rotate( glm::vec2( 1.0f, 0.0f ), rotation );
}

glm::vec2 Transform::findDown(
	) const {

	return glm::rotate( glm::vec2( 0.0f, -1.0f ), rotation );
}

glm::vec2 Transform::findLeft(
	) const {

	return glm::rotate( glm::vec2( -1.0f, 0.0f ), rotation );
}