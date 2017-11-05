
// shapes_2d.hpp

// source file for 2d colliders and shapes

#include "shapes_2d.hpp"

#include <algorithm>

using namespace pantheon;

Collision2D Collision2D::fromColliders
	( const ICollider2D * const t_colliderA
	, const ICollider2D * const t_colliderB ) {

	// get axes

	std::vector<glm::vec2> axesA = t_colliderA->getAxes( t_colliderB );
	std::vector<glm::vec2> axesB = t_colliderB->getAxes( t_colliderA );

	// set up minimum outputs

	glm::vec2 minimumAxis{ 0.0f, 0.0f };
	float minimumSeparation{ std::numeric_limits<float>::max() };

	// the callback function

	auto callback =
		[&t_colliderA, &t_colliderB, &minimumAxis, &minimumSeparation]
	( const glm::vec2& t_axis )->bool {

		// get bounds along axis

		glm::vec2 boundsA = t_colliderA->getBounds( t_axis );
		glm::vec2 boundsB = t_colliderB->getBounds( t_axis );

		// find degree of separation

		float separation{ 0.0f };
		if ( boundsA[0] >= boundsB[0] ) {

			if ( boundsA[1] <= boundsB[1] ) {

				float a = boundsA[0] - boundsB[1];
				float b = boundsA[1] - boundsB[0];

				separation = abs( a ) < abs( b ) ? a : b;
			}
			else if ( boundsA[0] < boundsB[1] ) {

				separation = boundsA[0] - boundsB[1];
			}
		}
		else {

			if ( boundsA[1] >= boundsB[1] ) {

				float a = boundsA[0] - boundsB[1];
				float b = boundsA[1] - boundsB[0];

				separation = abs( a ) < abs( b ) ? a : b;
			}
			else if ( boundsA[1] > boundsB[0] ) {

				separation = boundsA[1] - boundsB[0];
			}
		}

		// if no separation is found return false

		if ( abs( separation ) < 0.001f ) {

			return false;
		}

		// separation was found, check if is acceptable resolution candidate

		if ( abs( separation ) < abs( minimumSeparation ) ) {

			minimumAxis = t_axis;
			minimumSeparation = separation;
		}
		return true;
	};

	// for each axis find separation

	bool isCollision = true;
	for ( auto iter = axesA.begin(); iter != axesA.end() && isCollision
		; ++iter ) {

		isCollision = callback( *iter );
	}
	for ( auto iter = axesB.begin(); iter != axesB.end() && isCollision
		; ++iter ) {

		isCollision = callback( *iter );
	}

	// return separation if collision, otherwise return no collision

	return isCollision ? Collision2D( minimumAxis, minimumSeparation
		, t_colliderA, t_colliderB ) : NoCollision();
}

const std::vector<glm::vec2>& Circle::getAxes
	( const ICollider2D * const t_other ) const {

	const std::vector<glm::vec2>& points = t_other->getPoints();
	m_axes->clear();
	for ( auto point : points ) {

		m_axes->push_back( glm::normalize( m_points[0] - point ) );
	}
	return *m_axes;
}

glm::vec2 Circle::getBounds( const glm::vec2 & t_axis ) const {

	float dot = glm::dot( glm::vec2{ transform.position }, t_axis );
	return { dot - radius, dot + radius };
}

const std::vector<glm::vec2>& pantheon::Circle::getPoints() const {

	return m_points;
}

void Circle::prepare() {

	m_points[0] = transform.position;
}

glm::vec2 ConvexHull::getBounds( const glm::vec2& t_axis ) const {

	float min, max;
	const std::vector<glm::vec2>& newPoints = getPoints();
	min = max = glm::dot( t_axis, newPoints[0] );
	for ( auto point : newPoints ) {

		float dot = glm::dot( t_axis, point );
		min = min > dot ? dot : min;
		max = max < dot ? dot : max;
	}
	return{ min, max };
}

const std::vector<glm::vec2>& ConvexHull::getAxes( const ICollider2D * const ) const {

	return m_axes;
}

const std::vector<glm::vec2>& ConvexHull::getPoints() const {

	return m_points;
}

void ConvexHull::prepare() {

	m_points.clear();
	m_axes.clear();
	glm::mat4 matrix = transform.getMatrix();
	for ( auto point : points ) {

		m_points.push_back( matrix * glm::vec4( point, 0.0f, 1.0f ) );
	}
	glm::vec2 previousPoint = *(m_points.end() - 1);
	for ( auto point : m_points ) {

		glm::vec2 axis = glm::normalize( point - previousPoint );
		m_axes.push_back( { axis[1], -axis[0] } );
		previousPoint = point;
	}
}

Ray2D Ray2D::fromLineSegment( glm::vec2 t_start, glm::vec2 t_end ) {

	glm::vec2 direction = glm::normalize( t_end - t_start );
	return{ t_start, direction };
}

int pantheon::findIntersectionOfRays
( const Ray2D& t_a, const Ray2D& t_b, glm::vec2& t_outIntersection ) {

	// check if lines are perpendicular

	if ( abs( glm::dot( t_a.direction, t_b.direction ) ) > 0.99f ) {

		return PANTHEON_INTERSECTION_NOT_FOUND;
	}

	// find start and end positions of each ray

	glm::vec2 aStart = t_a.origin;
	glm::vec2 aEnd = t_a.origin + t_a.direction;
	glm::vec2 bStart = t_b.origin;
	glm::vec2 bEnd = t_b.origin + t_b.direction;

	// find denominator and numerator

	float denominator = ((bEnd[1] - bStart[1]) * (aEnd[0] - aStart[0]))
		- ((bEnd[0] - bStart[0]) * (aEnd[1] - aStart[1]));
	float numerator = ((bEnd[0] - bStart[0]) * (aStart[1] - bStart[1]))
		- ((bEnd[1] - bStart[1]) * (aStart[0] - bStart[0]));
	float length = numerator / denominator;

	// find point of intersection

	t_outIntersection = aStart + length * t_a.direction;

	return PANTHEON_INTERSECTION_FOUND;
}
