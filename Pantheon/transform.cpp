
// transform.cpp

// source file for transform class implementation

#include "transform.hpp"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\matrix_transform_2d.hpp>
#include <glm\gtx\quaternion.hpp>

using namespace pantheon;

///////////////////////////////////////////////////////////////////////////////
// Transform2D
///////////////////////////////////////////////////////////////////////////////

Transform2D::Transform2D() {

}
Transform2D::Transform2D( const Transform2D& t_other )
	: position{ t_other.position }, rotation{ t_other.rotation }, scale{ t_other.scale } {

}
Transform2D::Transform2D( const Transform& t_other )
	: position{ t_other.position }, depth{ t_other.position[2] }, scale{ t_other.scale }, rotation{ glm::eulerAngles( t_other.rotation )[2] } {

}
Transform2D::Transform2D( const glm::vec2& t_position )
	: position{ t_position } {

}
Transform2D::Transform2D( const glm::vec3& t_positionAndDepth )
	: position{ t_positionAndDepth }, depth{ t_positionAndDepth[2] } {

}
Transform2D::Transform2D( const glm::vec2& t_position, const glm::vec2& t_scale )
	: position{ t_position }, scale{ t_scale } {

}
Transform2D::Transform2D( const glm::vec3& t_positionAndDepth, const glm::vec2& t_scale )
	: position{ t_positionAndDepth }, depth{ t_positionAndDepth[2] }, scale{ t_scale } {

}
Transform2D::Transform2D( const glm::vec2& t_position, const glm::vec2& t_scale, float t_rotation )
	: position{ t_position }, scale{ t_scale }, rotation{ t_rotation } {

}
Transform2D::Transform2D( const glm::vec3& t_positionAndDepth, const glm::vec2& t_scale, float t_rotation )
	: position{ t_positionAndDepth }, depth{ t_positionAndDepth[2] }, scale{ t_scale }, rotation{ t_rotation } {

}
Transform2D::~Transform2D() {

}

glm::vec2 rotateVec2( float t_x, float t_y, float t_rotation ) {

	float cosr = cos( t_rotation );
	float sinr = sin( t_rotation );
	return
		{ t_x * cosr - t_y * sinr
		, t_x * sinr + t_y * cosr };
}

glm::vec2 Transform2D::findUp() const {
	
	return rotateVec2( 0.0f, 1.0f, rotation );
}
glm::vec2 Transform2D::findDown() const {

	return rotateVec2( 0.0f, -1.0f, rotation );
}
glm::vec2 Transform2D::findRight() const {

	return rotateVec2( 1.0f, 0.0f, rotation );
}
glm::vec2 Transform2D::findLeft() const {

	return rotateVec2( -1.0f, 0.0f, rotation );
}
glm::mat3 Transform2D::findMatrix() const {

	glm::mat3 rotateMatrix = glm::rotate( glm::mat3{ }, rotation );
	glm::mat3 translateMatrix = glm::translate( glm::mat3{ }, position );
	glm::mat3 scaleMatrix = glm::scale( glm::mat3{ }, scale );
	return translateMatrix * rotateMatrix * scaleMatrix;
}

///////////////////////////////////////////////////////////////////////////////
// Transform
///////////////////////////////////////////////////////////////////////////////

Transform::Transform() {

}
Transform::Transform( const Transform& t_other )
	: position{ t_other.position }, rotation{ t_other.rotation }, scale{ t_other.scale } {

}
Transform::Transform( glm::vec3 t_position )
	: position{ t_position } {

}
Transform::Transform( glm::vec3 t_position, glm::quat t_rotation )
	: position{ t_position }, rotation{ t_rotation } {

}
Transform::Transform( glm::vec3 t_position, glm::quat t_rotation, glm::vec3 t_scale )
	: position{ t_position }, rotation{ t_rotation }, scale{ t_scale } {

}
Transform::~Transform() {

}

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

	glm::mat4 rotateMatrix = glm::toMat4( rotation );
	glm::mat4 scaleMatrix = glm::scale( { }, scale );
	glm::mat4 translateMatrix = glm::translate( { }, position );
	return translateMatrix * rotateMatrix * scaleMatrix;
}

Transform pantheon::operator*( const Transform& t_a, const Transform& t_b ) {

	return {
		{ t_a.position * t_b.scale * t_b.rotation + t_b.position * t_a.scale * t_a.rotation },
		{ t_a.rotation * t_b.rotation },
		{ t_a.scale * t_b.scale } };
}