
// transform.hpp

// Header file for the transform class

#ifndef WIREFRAMETDS_TRANSFORM_HPP
#define WIREFRAMETDS_TRANSFORM_HPP

#include "wireframe_tds.hpp"
#define GLM_FORCE_CXX03
#include <glm.hpp>

namespace atlas {

	class Transform {
	private:

	public:

		Transform(
			);

		Transform(
			glm::vec2 t_position
			);

		Transform(
			glm::vec2 t_position,
			float t_rotation
			);

		Transform(
			glm::vec2 t_position,
			float t_rotation,
			glm::vec2 t_scale
			);

		glm::mat3 findMatrix(
			) const;

		glm::vec2 findUp(
			) const;

		glm::vec2 findRight(
			) const;

		glm::vec2 findDown(
			) const;

		glm::vec2 findLeft(
			) const;

		glm::vec2 position{ 0.0f, 0.0f };
		float rotation{ 0.0f };
		glm::vec2 scale{ 1.0f };
	};
}

#endif
