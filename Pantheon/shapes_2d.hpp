
// shapes_2d_hpp

// header file for public implementation of 2D shapes

#ifndef PANTHEON_SHAPES_2D_HPP
#define PANTHEON_SHAPES_2D_HPP

#include "pantheon.hpp"

#include "transform.hpp"

#include <vector>

namespace pantheon {

	class Collision2DComponent;

	const int PANTHEON_INTERSECTION_NOT_FOUND{ 0 };
	const int PANTHEON_INTERSECTION_FOUND{ 1 };

	struct PANTHEON_API ICollider2D {

	public:

		ICollider2D( const Transform& t_transform ) 
			: transform( t_transform ) {

		}

		// prepares the colliders axes and points

		virtual void prepare() = 0;

		// returns the minimum and maximum bounds of the collider given a 
		// projection axis

		virtual glm::vec2 getBounds( const glm::vec2& t_axis ) const = 0;

		// returns a list of potential separating axes given the other collider

		virtual const std::vector<glm::vec2>& getAxes( const ICollider2D* const t_other ) const = 0;

		// returns a list of shape important points
		// used in finding axes for rounded colliders

		virtual const std::vector<glm::vec2>& getPoints() const = 0;

		const Transform& transform;
	};

	struct PANTHEON_API Collision2D {

	public:

		Collision2D( const glm::vec2& t_axis, float t_separation
			, const ICollider2D* const t_colliderA
			, const ICollider2D* const t_colliderB )
			: axis{ t_separation < 0.0f ? -t_axis : t_axis }, separation{ t_separation < 0.0f ? -t_separation : t_separation }
			, colliderA{ t_colliderA }, colliderB{ t_colliderB } {

		}
		Collision2D( const Collision2D& t_other )
			: axis{ t_other.axis }, separation{ t_other.separation }
			, colliderA{ t_other.colliderA }, colliderB{ t_other.colliderB } {

		}

		Collision2D findInverted() {

			return{
				-axis, separation, colliderB, colliderA
			};
		}

		bool hasCollided() {

			return abs( separation ) > 0.001f;
		}

		static Collision2D NoCollision() {

			return{ glm::vec2{ 0.0f, 0.0f }, 0.0f, nullptr, nullptr };
		}

		static Collision2D fromColliders( const ICollider2D* const t_colliderA
			, const ICollider2D* const t_colliderB );

		const glm::vec2 axis;
		const float separation;
		const ICollider2D* const colliderA;
		const ICollider2D* const colliderB;
	};

	struct PANTHEON_API CollisionGroup2D {

	public:

		CollisionGroup2D( Actor& t_other
			, const std::vector<Collision2D>& t_collisions )
			: other{ t_other }, collisions{ t_collisions } {

		}
		CollisionGroup2D( const CollisionGroup2D& t_other )
			: other{ t_other.other }, collisions{ t_other.collisions } {

		}

		~CollisionGroup2D() {

		}

		const std::vector<Collision2D> collisions;
		Actor& other;
	};

	struct PANTHEON_API Circle : public ICollider2D {

	public:

		Circle( const Transform& t_transform ) : ICollider2D{ t_transform } {

		}

		Circle( const Transform& t_transform, float t_radius )
			: ICollider2D{ t_transform }, radius{ t_radius } {

		}

		~Circle() {

			delete m_axes;
		}

		virtual glm::vec2 getBounds( const glm::vec2& t_axis ) const override;

		virtual const std::vector<glm::vec2>& getAxes( const ICollider2D* const t_other ) const override;

		virtual const std::vector<glm::vec2>& getPoints() const override;

		virtual void prepare() override;

		float radius{ 1.0f };

	private:

		std::vector<glm::vec2> m_points{ 1 };
		std::vector<glm::vec2>* m_axes{ new std::vector<glm::vec2>() };
	};

	struct PANTHEON_API ConvexHull : public ICollider2D {

	public:

		ConvexHull( const Transform& t_transform )
			: ICollider2D{ t_transform } {

		}

		template<typename T>
		ConvexHull
		( const Transform& t_transform, T t_begin, T t_end)
			: ICollider2D{ t_transform }, points { t_begin, t_end } {

		}

		virtual const std::vector<glm::vec2>& getAxes( const ICollider2D * const t_other ) const override;

		virtual glm::vec2 getBounds( const glm::vec2 & t_axis ) const override;

		virtual const std::vector<glm::vec2>& getPoints() const override;

		virtual void prepare() override;

		std::vector<glm::vec2> points;

	private:

		std::vector<glm::vec2> m_points;
		std::vector<glm::vec2> m_axes;
	};

	struct PANTHEON_API Ray2D {

	public:
		Ray2D( const glm::vec2& t_origin, const glm::vec2& t_direction )
			: origin{ t_origin }, direction{ t_direction } {

		}

		static Ray2D fromLineSegment( glm::vec2 t_start, glm::vec2 t_end );

		glm::vec2 origin{ 0.0f, 0.0f };
		glm::vec2 direction{ 0.0f, 0.0f };
	};

	int PANTHEON_API findIntersectionOfRays
	( const Ray2D& t_a, const Ray2D& t_b, glm::vec2& t_outIntersection );
}

#endif