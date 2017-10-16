
// gl_objects.hpp

// header file for the public implementation of gl object classes

#ifndef PANTHEON_GL_OBJECTS_HPP
#define PANTHEON_GL_OBJECTS_HPP

#include "pantheon.hpp"
#include "game.hpp"

namespace pantheon {

	class IGlObject {

	public:

		virtual ~IGlObject() = 0 {

		}

		virtual void unbind() = 0;
	};

	template<typename T>
	class GlGuard {

	public:

		GlGuard( IGlObject& t_object ) : m_object{ t_object } {

		}

		GlGuard( const GlGuard& ) = delete;

		~GlGuard() {

			m_object.unbind();
		}

		GlGuard& operator=( const GlGuard& ) = delete;

	private:

		T& m_object;
	};

	class PANTHEON_API GlBuffer : public IGlObject {

	public:

		GlBuffer();
		~GlBuffer();

		void fillStaticDraw( void* t_dataPointer, unsigned int t_dataLength );
		void fillDynamicDraw( void* t_dataPointer, unsigned int t_dataLength );
		void fillStreamDraw( void* t_dataPointer, unsigned int t_dataLength );

		void fillStaticRead( void* t_dataPointer, unsigned int t_dataLength );
		void fillDynamicRead( void* t_dataPointer, unsigned int t_dataLength );
		void fillStreamRead( void* t_dataPointer, unsigned int t_dataLength );

		void fillStaticCopy( void* t_dataPointer, unsigned int t_dataLength );
		void fillDynamicCopy( void* t_dataPointer, unsigned int t_dataLength );
		void fillStreamCopy( void* t_dataPointer, unsigned int t_dataLength );

		void bindToArray();
		void bindToAtomicCounter();
		void bindToCopyRead();
		void bindToCopyWrite();
		void bindToDispatchIndirect();
		void bindToDrawIndirect();
		void bindToElementArray();
		void bindToPixelPack();
		void bindToPixelUnpack();
		void bindToQuery();
		void bindToShaderStorage();
		void bindToTexture();
		void bindToTransformFeedback();
		void bindToUniform();

		void unbind() override;

		unsigned int getBind() const;
		unsigned int getId() const;

	private:

		unsigned int m_bufferBind;
		unsigned int m_bufferId;
	};
}

#endif