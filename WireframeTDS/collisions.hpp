
// collisions.hpp

// Header file for the collisions namespace

#ifndef WIREFRAMETDS_COLLISIONS_HPP
#define WIREFRAMETDS_COLLISIONS_HPP

#include "wireframe_tds.hpp"

#include "forward_declaration.hpp"

#define GLM_FORCE_CXX03

#include <glm.hpp>
#include <limits>
#include <vector>

namespace wireframe_tds {

	namespace collisions {
		
		// collision constants

		const int WTDS_NO_COLLISION_FOUND{ 0 };
		const int WTDS_COLLISION_FOUND{ 1 };
		const glm::vec3 WTDS_INVALID_AXIS{
			0.0f, 0.0f, std::numeric_limits<float>::epsilon() };


		// An interface for collider objects

		struct ICollider {
		public:

			// finds bounds of collider given an axis

			virtual glm::vec2 findBounds( 
				const glm::vec2 t_axis 
				) const = 0;

		};

		// A circle collider

		struct Circle : public ICollider {
		public:
			Circle(
				) = delete;

			Circle(
				glm::vec2 t_offset
				, float t_radius
				) : offset( t_offset ), radius( t_radius ) {

			}

			glm::vec2 findBounds(
				const glm::vec2 t_axis
				) const override;

			// the position of the circle

			glm::vec2 offset;

			// the radius of the circle

			float radius;
		};

		// An interface for convex hull colliders

		struct IConvexHull : public ICollider {
		public:

			// returns the axes of the hull

			virtual std::vector<glm::vec2> getAxes(
				) const = 0;

			// return the points of the hull

			virtual std::vector<glm::vec2> getPoints(
				) const = 0;
		};

		// A template class for convex hull colliders with N points

		template<unsigned int N> struct ConvexHull : public IConvexHull {
		protected:

			// updates min and or max if specified point is a more suitable candidate

			float comparePointToBoundsOnAxis(
				const int t_pointIndex,
				const glm::vec2 axis,
				float& min,
				float& max
				) const {

				// assert point index is in range and axis is of length 1

				assert( t_pointIndex >= 0 || t_pointIndex < N
					&& "point index is out of range");
				assert( abs(glm::length( axis ) - 1.0f) <= 0.01f
					&& "axis is not normalized");

				// if dot product is a better candidate for bounds, replace bounds

				float dot = glm::dot( points[t_pointIndex], axis );
				if ( dot < min )
					min = dot;
				if ( dot > max )
					max = dot;

				return dot;
			}

			// the axes of the convex hull

			glm::vec2 m_axes[N];
		public:
			ConvexHull(
				) = delete;

			explicit ConvexHull( 
				const glm::vec2 (&t_points)[N] 
				) {

				// assert hull is not a non-degenerate shape

				static_assert( N >= 3, "N is not large enough " );

				for ( int i = 0; i < N; i++ ) {

					points[i] = t_points[i];
				}
			}

			std::vector<glm::vec2> getPoints(
				) const override {

				return std::vector<glm::vec2>( points, points + N );
			}

			std::vector<glm::vec2> getAxes(
				) const override {
				
				return std::vector<glm::vec2>( m_axes, m_axes + N );
			}

			glm::vec2 findBounds( 
				const glm::vec2 t_axis 
				) const override {

				float min{ std::numeric_limits<float>::max() };
				float max{ std::numeric_limits<float>::min() };
				for ( int i = 0; i < N; i++ ) {

					comparePointToBoundsOnAxis(i, t_axis, min, max);
				}
				return { min, max };
			}

			// recalculate axes

			void findAxes(
				) {

				std::vector<glm::vec2> output{ N };

				// find surface axes

				for ( int i = 0; i < N - 1; i++ ) {

					glm::vec2 axis = glm::normalize( points[i] - points[i + 1] );
					m_axes[i][0] = -axis[1];
					m_axes[i][1] = axis[0];
					assert( abs(glm::length( m_axes[i] ) - 1.0f) <= 0.01f
						&& "invalid axis found" );
				}

				// find last axis

				glm::vec2 axis = glm::normalize( points[N - 1] - points[0] );
				m_axes[N - 1][0] = -axis[1];
				m_axes[N - 1][1] = axis[0];

				assert(  abs(glm::length( m_axes[N - 1] ) - 1.0f) <= 0.01f
					&& "invalid axis found" );
			}

			// the points of the convex hull

			glm::vec2 points[N];
		};

		typedef ConvexHull<3> Triangle;
		typedef ConvexHull<4> Quad;
		typedef ConvexHull<6> Hexagon;
		typedef ConvexHull<5> Pentagon;
		typedef ConvexHull<7> Septagon;
		typedef ConvexHull<8> Octogon;

		// get whether separating axis is valid or not

		int getAxisState(
			glm::vec3 t_separatingAxis
			);

		// find intersection of colliders along an axis

		float findIntersectionOfAxis(
			const glm::vec2 t_axis,
			const ICollider& t_colliderA,
			const ICollider& t_colliderB 
			);

		// find minimum separating axis between 2 colliders given a list of axes

		glm::vec3 findMinimumSeparatingAxis(
			const std::vector<glm::vec2>& t_axes,
			const ICollider& t_colliderA,
			const ICollider& t_colliderB
			);

		// find minimum separating axes between 2 colliders, overloads exist for
		// primative types, convex hull and circle

		glm::vec3 findMinimumSeparatingAxis(
			const ICollider& t_colliderA,
			const ICollider& t_colliderB );
		glm::vec3 findMinimumSeparatingAxis(
			const Circle& t_colliderA,
			const Circle& t_colliderB );
		glm::vec3 findMinimumSeparatingAxis(
			const Circle& t_colliderA,
			const IConvexHull& t_colliderB );
		glm::vec3 findMinimumSeparatingAxis(
			const IConvexHull& t_colliderA,
			const Circle& t_colliderB );
		glm::vec3 findMinimumSeparatingAxis(
			const IConvexHull& t_colliderA,
			const IConvexHull& t_colliderB );

	}
}

#endif