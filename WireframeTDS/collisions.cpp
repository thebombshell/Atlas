
// collisions.cpp

// Source file for the collisions namespace

#include "collisions.hpp"

using namespace wireframe_tds;

glm::vec2 collisions::Circle::findBounds(
	const glm::vec2 t_axis
	) const {

	float position = glm::dot( offset, t_axis );
	return{ position - radius, position + radius };
}

int collisions::getAxisState(
	glm::vec3 t_separatingAxis
	) {

	if ( t_separatingAxis[0] * t_separatingAxis[0]
		+ t_separatingAxis[1] * t_separatingAxis[1]
		< 1.0f - std::numeric_limits<float>::epsilon()
		|| abs(t_separatingAxis[2] -  WTDS_INVALID_AXIS[2]) 
		<= std::numeric_limits<float>::epsilon() ) {

		return WTDS_NO_COLLISION_FOUND;
	}

	return WTDS_COLLISION_FOUND;
}

float collisions::findIntersectionOfAxis(
	const glm::vec2 t_axis,
	const ICollider& t_colliderA,
	const ICollider& t_colliderB 
	) {

	// assert axis has length of 1

	assert( glm::length( t_axis ) - 1.0f <= std::numeric_limits<float>::epsilon() );

	// find bounds of colliders

	glm::vec2 boundsA{ t_colliderA.findBounds( t_axis ) };
	glm::vec2 boundsB{ t_colliderB.findBounds( t_axis ) };

	// if bounds do not overlap return invalid axis

	if ( boundsA[0] > boundsB[1] || boundsA[1] < boundsB[0] ) {

		return WTDS_INVALID_AXIS[2];
	}

	// evaluate collision cases

	float separation{ WTDS_INVALID_AXIS[2] };

	if ( boundsA[0] > boundsB[0] ) {

		if ( boundsA[1] < boundsB[1] ) {

			// A is inside B
			
			float a = boundsA[0] - boundsB[1];
			float b = boundsA[1] - boundsB[0];

			separation = abs( a ) < abs( b ) ? a : b;
		}
		else {

			// resolve right

			separation = boundsA[0] - boundsB[1];
		}
	}
	else {

		if ( boundsA[1] > boundsB[1] ) {

			// B is inside A
			
			float a = boundsA[0] - boundsB[1];
			float b = boundsA[1] - boundsB[0];

			separation = abs( a ) < abs( b ) ? a : b;
		}
		else {

			// resolve left

			separation = boundsA[1] - boundsB[0];
		}
	}

	return separation;
}

glm::vec3 collisions::findMinimumSeparatingAxis(
	const std::vector<glm::vec2>& t_axes,
	const ICollider& t_colliderA,
	const ICollider& t_colliderB
	) {

	// assert axes is not empty

	assert( t_axes.size() > 0 );

	glm::vec3 minimumAxis{ WTDS_INVALID_AXIS };

	// find smallest separating axis

	for ( auto iter = t_axes.begin(); iter != t_axes.end(); ++iter ) {

		float separation = findIntersectionOfAxis(
			*iter, t_colliderA, t_colliderB );
		if ( separation == WTDS_INVALID_AXIS[2] ) {

			return WTDS_INVALID_AXIS;
		}
		if ( separation <= minimumAxis[2] ) {

			minimumAxis = { *iter, separation };
		}
	}

	return minimumAxis;
}

glm::vec3 collisions::findMinimumSeparatingAxis(
	const ICollider& t_colliderA,
	const ICollider& t_colliderB ) {

	// attempt casts

	const Circle* circleA = dynamic_cast<const Circle*>(&t_colliderA);
	const IConvexHull* hullA = dynamic_cast<const IConvexHull*>(&t_colliderA);
	const Circle* circleB = dynamic_cast<const Circle*>(&t_colliderB);
	const IConvexHull* hullB = dynamic_cast<const IConvexHull*>(&t_colliderB);

	// proceed to approrpiate function depending on cast results

	if ( circleA != nullptr ) {

		if ( circleB != nullptr ) {

			return findMinimumSeparatingAxis( *circleA, *circleB );
		}
		if ( hullB != nullptr ) {

			return findMinimumSeparatingAxis( *circleA, *hullB );
		}
		else {

			// unexpected result

			return WTDS_INVALID_AXIS;
		}
	}
	if ( hullA != nullptr ) {

		if ( circleB != nullptr ) {

			return findMinimumSeparatingAxis( *hullA, *circleB );
		}
		if ( hullB != nullptr ) {

			return findMinimumSeparatingAxis( *hullA, *hullB );
		}
		else {

			// unexpected result

			return WTDS_INVALID_AXIS;
		}
	}

	// unexpected result

	return WTDS_INVALID_AXIS;
}

glm::vec3 collisions::findMinimumSeparatingAxis(
	const Circle& t_colliderA,
	const Circle& t_colliderB 
	) {

	// find axis and separation

	glm::vec2 axis{ glm::normalize( t_colliderA.offset - t_colliderB.offset ) };
	float separation{ findIntersectionOfAxis( axis, t_colliderA, t_colliderB ) };

	// return axis

	return{ axis, separation };
}

glm::vec3 collisions::findMinimumSeparatingAxis(
	const Circle& t_colliderA,
	const IConvexHull& t_colliderB 
	) {

	std::vector<glm::vec2> pointsB = t_colliderB.getPoints();

	std::vector<glm::vec2> axes;

	// find axes of collider A

	for ( auto iter = pointsB.begin(); iter != pointsB.end(); ++iter ) {

		glm::vec2 axis{ glm::normalize( t_colliderA.offset - *iter ) };
		axes.push_back( axis );
	}

	// get axes of collider B

	std::vector<glm::vec2> axesB{ t_colliderB.getAxes() };
	axes.insert( axes.end(), axesB.begin(), axesB.end() );

	// find and return minimum separating axis

	return findMinimumSeparatingAxis( axes, t_colliderA, t_colliderB );
}

glm::vec3 collisions::findMinimumSeparatingAxis(
	const IConvexHull& t_colliderA,
	const Circle& t_colliderB
	) {

	// find minimum separating axis

	glm::vec3 output = findMinimumSeparatingAxis( t_colliderB, t_colliderA );

	// reverse resolve direction and return

	output[2] = -output[2];
	return output;
}

glm::vec3 collisions::findMinimumSeparatingAxis(
	const IConvexHull& t_colliderA,
	const IConvexHull& t_colliderB
	) {

	std::vector<glm::vec2> axes;

	// get axes of collider A

	std::vector<glm::vec2> axesA{ t_colliderA.getAxes() };
	axes.insert( axes.end(), axesA.begin(), axesA.end() );

	// get axes of collider B

	std::vector<glm::vec2> axesB{ t_colliderB.getAxes() };
	axes.insert( axes.end(), axesB.begin(), axesB.end() );

	// find and return minimum separating axis

	return findMinimumSeparatingAxis( axes, t_colliderA, t_colliderB );
}
