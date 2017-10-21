
// line_renderer.hpp

// header file for a game renderer of lines

#ifndef PANTHEON_LINE_RENDERER_HPP
#define PANTHEON_LINE_RENDERER_HPP

#include "pantheon.hpp"

#include "game.hpp"

#include <vector>

namespace pantheon {

	// line renderer vertex object

	struct PANTHEON_API LineRendererVertex {

	public:
		LineRendererVertex() : data{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f } {

		}

		LineRendererVertex( float t_posX, float t_posY, float t_posZ
				, float t_colR, float t_colG, float t_colB )
			: data{ t_posX, t_posY, t_posZ, t_colR, t_colG, t_colB }
			, position{ data }, colour{ data + 3 } {

		}

		LineRendererVertex( const glm::vec3& t_pos, const glm::vec3& t_col )
			: data{ t_pos[0], t_pos[1], t_pos[2], t_col[0], t_col[1]
			, t_col[2] }, position{ data }, colour{ data + 3 } {

		}

		LineRendererVertex( const LineRendererVertex& t_other )
			: position{ data }, colour{ data + 3 } {

			std::copy( t_other.data, t_other.data + 6, data );
		}

		LineRendererVertex& operator=( const LineRendererVertex& t_other ) {

			std::copy( t_other.data, t_other.data + 6, data );
			return *this;
		}

		// returns an invalid vertex used to denote separation between shapes

		static LineRendererVertex separator() {

			return{ NAN, INFINITY, NAN, INFINITY, NAN, INFINITY };
		}

		// returns true if vertex is a separator

		bool isSeperator() const {

			return isnan( data[0] ) && isinf( data[1] ) && isnan( data[2] )
				&& isinf( data[3] ) && isnan( data[4] ) && isinf( data[5] )
				? true : false;
		}

		// copyable raw data array of vertex

		float data[6]{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

		// pointer to position component in data

		float* const position{ data };

		// pointer to colour component in data

		float* const colour{ data + 3 };
	};

	// message to pass through queue draw

	class PANTHEON_API LineRendererMessage {

	public:

		LineRendererMessage() : vertices{ }, transform{ } {

		}

		LineRendererMessage( const glm::mat3& t_transform )
			: vertices{ }, transform{ t_transform } {

		}

		template<typename T>
		LineRendererMessage( T t_begin, T t_end )
			: vertices{ t_begin, t_end }, transform{ } {

		}

		template<typename T>
		LineRendererMessage( T t_begin, T t_end, const glm::mat3& t_transform )
			: vertices{ t_begin, t_end }, transform{ t_transform } {

		}

		void pushVertex( const LineRendererVertex& t_vertex ) {

			vertices.push_back( t_vertex );
		}

		template<typename T>
		void pushVertices( T t_begin, T t_end ) {

			vertices.insert( vertices.end(), t_begin, t_end );
		}

		// adds a separator to the end of the message

		void separate() {

			vertices.push_back( LineRendererVertex::separator() );
		}

		// adds two separators whcih are treated as a loop command at the end
		// of the message

		void loop() {

			vertices.push_back( LineRendererVertex::separator() );
			vertices.push_back( LineRendererVertex::separator() );
		}

		std::vector<LineRendererVertex> vertices;
		glm::mat3 transform;
	};

	// public implementation of thick line renderer

	class PANTHEON_API LineRenderer : public IGameRenderer {

	private:

		LineRenderer( ConstructRendererPermit& );
		~LineRenderer();

		class LineRendererImpl;
		LineRendererImpl* m_lineRenderer{ nullptr };

	public:

		static IGameRenderer* createInstance( ConstructRendererPermit& );

		void render() override;

		void beforeActorsDraw() override;

		void afterActorsDraw() override;

		void queueDraw( LineRendererMessage& t_drawMessage );

	};
}

#endif //PANTHEON_LINE_RENDERER_HPP