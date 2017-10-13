
// transform.cpp

// source file for transform class implementation

#include "transform.hpp"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>

using namespace pantheon;

glm::vec3 Transform::findUp() const {

	return rotation * glm::vec3( 0.0f, 1.0f, 0.0f );
}

glm::vec3 Transform::findDown() const {

	return rotation * glm::vec3( 0.0f, -1.0f, 0.0f );
}

glm::vec3 Transform::findLeft() const {

	return rotation * glm::vec3( -1.0f, 0.0f, 0.0f );
}

glm::vec3 Transform::findRight() const {

	return rotation * glm::vec3( 1.0f, 0.0f, 0.0f );
}

glm::vec3 Transform::findForward() const {

	return rotation * glm::vec3( 0.0f, 0.0f, 1.0f );
}

glm::vec3 Transform::findBackward() const {

	return rotation * glm::vec3( 0.0f, 0.0f, -1.0f );
}

glm::mat4 Transform::findMatrix() const {

	glm::mat4 rotMat = glm::toMat4( rotation );
	glm::mat4 scaleMat = glm::scale( { }, scale );
	glm::mat4 translateMat = glm::translate( { }, position );
	return translateMat * rotMat * scaleMat;
}

Transform pantheon::operator*( const Transform& t_a, const Transform& t_b ) {

	return {
		{ t_a.position * t_b.scale * t_b.rotation + t_b.position * t_a.scale * t_a.rotation },
		{ t_a.rotation * t_b.rotation },
		{ t_a.scale * t_b.scale } };
}