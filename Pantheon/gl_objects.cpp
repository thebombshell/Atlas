
// gl_objects.cpp

// source file for implementation of gl object classes

#include "gl_objects.hpp"

#include <GL\glew.h>

using namespace pantheon;

GlBuffer::GlBuffer() : m_bufferBind{ GL_NONE } {

	glGenBuffers( 1, &m_bufferId );
}

GlBuffer::~GlBuffer() {

	unbind();
	glDeleteBuffers( 1, &m_bufferId );
}

void fillBuffer( GlBuffer& t_buffer, void* t_dataPointer
	, unsigned int t_dataLength, unsigned int t_usage ) {

	unsigned int bufferLocation = t_buffer.getBind();
	if ( !bufferLocation ) {

		t_buffer.bindToArray();
	}
	glBufferData( bufferLocation ? bufferLocation : GL_ARRAY_BUFFER
		, t_dataLength, t_dataPointer, t_usage );
	if ( !bufferLocation ) {

		t_buffer.unbind();
	}
}

void GlBuffer::fillStaticDraw( void* t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STATIC_DRAW );
}
void GlBuffer::fillDynamicDraw( void* t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_DYNAMIC_DRAW );
}
void GlBuffer::fillStreamDraw( void* t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STREAM_DRAW );
}

void GlBuffer::fillStaticRead( void* t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STATIC_READ );
}
void GlBuffer::fillDynamicRead( void* t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_DYNAMIC_READ );
}
void GlBuffer::fillStreamRead( void* t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STREAM_READ );
}

void GlBuffer::fillStaticCopy( void* t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STATIC_COPY );
}
void GlBuffer::fillDynamicCopy( void* t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_DYNAMIC_COPY );
}
void GlBuffer::fillStreamCopy( void* t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STREAM_COPY );
}

void GlBuffer::bindToArray() {

	glBindBuffer( (m_bufferBind = GL_ARRAY_BUFFER), m_bufferId );
}
void GlBuffer::bindToAtomicCounter() {

	glBindBuffer( (m_bufferBind = GL_ATOMIC_COUNTER_BUFFER), m_bufferId );
}
void GlBuffer::bindToCopyRead() {

	glBindBuffer( (m_bufferBind = GL_COPY_READ_BUFFER), m_bufferId );
}
void GlBuffer::bindToCopyWrite() {

	glBindBuffer( (m_bufferBind = GL_COPY_WRITE_BUFFER), m_bufferId );
}
void GlBuffer::bindToDispatchIndirect() {

	glBindBuffer( (m_bufferBind = GL_DISPATCH_INDIRECT_BUFFER), m_bufferId );
}
void GlBuffer::bindToDrawIndirect() {

	glBindBuffer( (m_bufferBind = GL_DRAW_INDIRECT_BUFFER), m_bufferId );
}
void GlBuffer::bindToElementArray() {

	glBindBuffer( (m_bufferBind = GL_ELEMENT_ARRAY_BUFFER), m_bufferId );
}
void GlBuffer::bindToPixelPack() {

	glBindBuffer( (m_bufferBind = GL_PIXEL_PACK_BUFFER), m_bufferId );
}
void GlBuffer::bindToPixelUnpack() {

	glBindBuffer( (m_bufferBind = GL_PIXEL_UNPACK_BUFFER), m_bufferId );
}
void GlBuffer::bindToQuery() {

	glBindBuffer( (m_bufferBind = GL_QUERY_BUFFER), m_bufferId );
}
void GlBuffer::bindToShaderStorage() {

	glBindBuffer( (m_bufferBind = GL_SHADER_STORAGE_BUFFER), m_bufferId );
}
void GlBuffer::bindToTexture() {

	glBindBuffer( (m_bufferBind = GL_TEXTURE_BUFFER), m_bufferId );
}
void GlBuffer::bindToTransformFeedback() {

	glBindBuffer( (m_bufferBind = GL_TRANSFORM_FEEDBACK_BUFFER), m_bufferId );
}
void GlBuffer::bindToUniform() {

	glBindBuffer( (m_bufferBind = GL_UNIFORM_BUFFER), m_bufferId );
}

void GlBuffer::unbind() {

	glBindBuffer( m_bufferBind, NULL );
	m_bufferBind = GL_NONE;
}

unsigned int GlBuffer::getBind() const {

	return m_bufferBind;
}
unsigned int GlBuffer::getId() const {

	return m_bufferId;
}