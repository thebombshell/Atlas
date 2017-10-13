
// transform.hpp

// header file for the public implementation of the transform class

#ifndef PANTHEON_TRANSFORM_HPP
#define PANTHEON_TRANSFORM_HPP

#include "pantheon.hpp"

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

namespace pantheon {

	struct PANTHEON_API Transform {

	public:

		Transform()
			: position{ 0.0f, 0.0f, 0.0f }
			, rotation{ 1.0f, 0.0f, 0.0f, 0.0f }
			, scale{ 1.0f, 1.0f, 1.0f } {
		}

		Transform( const Transform& t_other ) 
			: position{ t_other.position }
			, rotation{ t_other.rotation }
			, scale{ t_other.scale } {

		}

		Transform( glm::vec3 t_position )
			: position{ t_position } {

		}

		Transform( glm::vec3 t_position, glm::quat t_rotation )
			: position{ t_position }
			, rotation{ t_rotation } {

		}

		Transform( glm::vec3 t_position, glm::quat t_rotation, glm::vec3 t_scale )
			: position{ t_position }
			, rotation{ t_rotation }
			, scale{ t_scale } {

		}

		~Transform() {

		}

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
	};

	static Transform operator*( const Transform& t_a, const Transform& t_b );
}

#endif // PANTHEON_TRANSFORM_HPP