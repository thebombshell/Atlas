
// transform.hpp

// header file for the public implementation of the transform class

#ifndef PANTHEON_TRANSFORM_HPP
#define PANTHEON_TRANSFORM_HPP

#include "pantheon.hpp"

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

//#include <memory>

namespace pantheon {

	// forward declarations

	class Transform2D;
	class Transform;

	// 2D transform class

	struct PANTHEON_API Transform2D {

	public:

		Transform2D();
		Transform2D( const Transform2D& t_other );
		Transform2D( const Transform& t_other );
		Transform2D( const glm::vec2& t_position );
		Transform2D( const glm::vec3& t_positionAndDepth );
		Transform2D( const glm::vec2& t_position, const glm::vec2& t_scale );
		Transform2D( const glm::vec3& t_positionAndDepth, const glm::vec2& t_scale );
		Transform2D( const glm::vec2& t_position, const glm::vec2& t_scale, float t_rotation );
		Transform2D( const glm::vec3& t_positionAndDepth, const glm::vec2& t_scale, float t_rotation );
		~Transform2D();

		glm::vec2 findUp() const;
		glm::vec2 findDown() const;
		glm::vec2 findRight() const;
		glm::vec2 findLeft() const;
		glm::mat3 findMatrix() const;

		glm::vec2 position{ 0.0f, 0.0f };
		glm::vec2 scale{ 1.0f, 1.0f };
		float rotation{ 0.0f };
		float depth{ 0.0f };

	private:

		//glm::vec2 old_position{ 0.0f, 0.0f };
		//glm::vec2 old_scale{ 1.0f, 1.0f };
		//float old_rotation{ 0.0f };

		//std::unique_ptr<glm::mat3> m_matrix{ std::make_unique<glm::mat3>() };
	};

	// 3D transform class

	struct PANTHEON_API Transform {

	public:

		Transform();
		Transform( const Transform& t_other );
		Transform( glm::vec3 t_position );
		Transform( glm::vec3 t_position, glm::quat t_rotation );
		Transform( glm::vec3 t_position, glm::quat t_rotation, glm::vec3 t_scale );

		~Transform();

		glm::vec3 findUp() const;
		glm::vec3 findDown() const;
		glm::vec3 findLeft() const;
		glm::vec3 findRight() const;
		glm::vec3 findForward() const;
		glm::vec3 findBackward() const;
		glm::mat4 findMatrix() const;

		void reset() {

			position = { 0.0f, 0.0f, 0.0f };
			rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
			scale = { 1.0f, 1.0f, 1.0f };
		}

		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

	private:

		Transform( bool ) : m_old{ nullptr }, m_old_mat{ nullptr } {

		}

		Transform* m_old;
		glm::mat4* m_old_mat;
		
	};

	static Transform operator*( const Transform& t_a, const Transform& t_b );
}

#endif