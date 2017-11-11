
// gl_objects.cpp

// source file for implementation of gl object classes

#include "gl_objects.hpp"
#include "helpers.hpp"

#include <GL\glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <utility>

using namespace pantheon;

///////////////////////////////////////////////////////////////////////////////
// GlDebug
///////////////////////////////////////////////////////////////////////////////

GlDebug::GlDebugVertexAttrib::GlDebugVertexAttrib( int t_index ) : index{ t_index } {

}
GlDebug::GlDebugVertexAttrib::~GlDebugVertexAttrib() {

}
void GlDebug::GlDebugVertexAttrib::update() {

	glGetVertexAttribiv( index, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &bufferBinding );
	glGetVertexAttribiv( index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled );
	glGetVertexAttribiv( index, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size );
	glGetVertexAttribiv( index, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride );
	glGetVertexAttribiv( index, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type );
	glGetVertexAttribiv( index, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &normalized );
	glGetVertexAttribiv( index, GL_VERTEX_ATTRIB_ARRAY_INTEGER, &integer );
	glGetVertexAttribiv( index, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, &divisor );
}

GlDebug::GlDebug() {

	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs );
	for ( int i = 0; i < maxVertexAttribs; i++ ) {

		vertexAttribs.push_back( { i } );
	}
}
GlDebug::~GlDebug() {

}
void GlDebug::update() {

	glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &arrayBufferBinding );
	glGetIntegerv( GL_ATOMIC_COUNTER_BUFFER_BINDING, &atomicCounterBufferBinding );
	glGetIntegerv( GL_COPY_READ_BUFFER_BINDING, &copyReadBufferBinding );
	glGetIntegerv( GL_COPY_WRITE_BUFFER_BINDING, &copyWriteBufferBinding );
	glGetIntegerv( GL_DISPATCH_INDIRECT_BUFFER_BINDING, &dispatchIndirectBufferBinding );
	glGetIntegerv( GL_DRAW_INDIRECT_BUFFER_BINDING, &drawIndirectBufferBinding );
	glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementArrayBufferBinding );
	glGetIntegerv( GL_PIXEL_PACK_BUFFER_BINDING, &pixelPackBufferBinding );
	glGetIntegerv( GL_PIXEL_UNPACK_BUFFER_BINDING, &pixelUnpackBufferBinding );
	glGetIntegerv( GL_QUERY_BUFFER_BINDING, &queryBufferBinding );
	glGetIntegerv( GL_SHADER_STORAGE_BUFFER_BINDING, &shaderStorageBufferBinding );
	glGetIntegerv( GL_TEXTURE_BUFFER_BINDING, &textureBufferBinding );
	glGetIntegerv( GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, &transformFeedbackBufferBinding );
	glGetIntegerv( GL_UNIFORM_BUFFER_BINDING, &uniformBufferBinding );

	glGetIntegerv( GL_TEXTURE_BINDING_1D, &textureBinding1D );
	glGetIntegerv( GL_TEXTURE_BINDING_2D, &textureBinding2D );
	glGetIntegerv( GL_TEXTURE_BINDING_3D, &textureBinding3D );

	for ( auto& attrib : vertexAttribs ) {

		attrib.update();
	}

	glGetIntegerv( GL_ACTIVE_TEXTURE, &activeTexture );
}

///////////////////////////////////////////////////////////////////////////////
// GlBuffer
///////////////////////////////////////////////////////////////////////////////

GlBuffer::GlBuffer() : m_bind{ GL_NONE } {

	glGenBuffers( 1, &m_id );
}

GlBuffer::~GlBuffer() {

	unbind();
	glDeleteBuffers( 1, &m_id );
}

void fillBuffer( GlBuffer& t_buffer, const void* const t_dataPointer
	, unsigned int t_dataLength, unsigned int t_usage ) {

	unsigned int bufferLocation = t_buffer.getBind();
	if ( bufferLocation == GL_NONE ) {

		t_buffer.bindToArray();
	}
	glBufferData( bufferLocation != GL_NONE ? bufferLocation : GL_ARRAY_BUFFER
		, t_dataLength, t_dataPointer, t_usage );
	if ( bufferLocation == GL_NONE ) {

		t_buffer.unbind();
	}
}

void GlBuffer::fillStaticDraw( const void* const t_dataPointer, unsigned int t_dataLength ) {

	const unsigned short* const reinterpret = reinterpret_cast<const unsigned short* const>(t_dataPointer);
	std::vector<unsigned short> test( reinterpret, reinterpret + t_dataLength / sizeof( unsigned short ) );
	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STATIC_DRAW );
}
void GlBuffer::fillDynamicDraw( const void* const t_dataPointer, unsigned int t_dataLength ) {

	const float* const reinterpret = reinterpret_cast<const float* const>(t_dataPointer);
	std::vector<float> test( reinterpret, reinterpret + t_dataLength / sizeof(float) );
	fillBuffer( *this, t_dataPointer, t_dataLength, GL_DYNAMIC_DRAW );
}
void GlBuffer::fillStreamDraw( const void* const t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STREAM_DRAW );
}

void GlBuffer::fillStaticRead( const void* const t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STATIC_READ );
}
void GlBuffer::fillDynamicRead( const void* const t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_DYNAMIC_READ );
}
void GlBuffer::fillStreamRead( const void* const t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STREAM_READ );
}

void GlBuffer::fillStaticCopy( const void* const t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STATIC_COPY );
}
void GlBuffer::fillDynamicCopy( const void* const t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_DYNAMIC_COPY );
}
void GlBuffer::fillStreamCopy( const void* const t_dataPointer, unsigned int t_dataLength ) {

	fillBuffer( *this, t_dataPointer, t_dataLength, GL_STREAM_COPY );
}

void GlBuffer::bindToArray() {

	glBindBuffer( (m_bind = GL_ARRAY_BUFFER), m_id );
}
void GlBuffer::bindToAtomicCounter() {

	glBindBuffer( (m_bind = GL_ATOMIC_COUNTER_BUFFER), m_id );
}
void GlBuffer::bindToCopyRead() {

	glBindBuffer( (m_bind = GL_COPY_READ_BUFFER), m_id );
}
void GlBuffer::bindToCopyWrite() {

	glBindBuffer( (m_bind = GL_COPY_WRITE_BUFFER), m_id );
}
void GlBuffer::bindToDispatchIndirect() {

	glBindBuffer( (m_bind = GL_DISPATCH_INDIRECT_BUFFER), m_id );
}
void GlBuffer::bindToDrawIndirect() {

	glBindBuffer( (m_bind = GL_DRAW_INDIRECT_BUFFER), m_id );
}
void GlBuffer::bindToElementArray() {

	glBindBuffer( (m_bind = GL_ELEMENT_ARRAY_BUFFER), m_id );
}
void GlBuffer::bindToPixelPack() {

	glBindBuffer( (m_bind = GL_PIXEL_PACK_BUFFER), m_id );
}
void GlBuffer::bindToPixelUnpack() {

	glBindBuffer( (m_bind = GL_PIXEL_UNPACK_BUFFER), m_id );
}
void GlBuffer::bindToQuery() {

	glBindBuffer( (m_bind = GL_QUERY_BUFFER), m_id );
}
void GlBuffer::bindToShaderStorage() {

	glBindBuffer( (m_bind = GL_SHADER_STORAGE_BUFFER), m_id );
}
void GlBuffer::bindToTexture() {

	glBindBuffer( (m_bind = GL_TEXTURE_BUFFER), m_id );
}
void GlBuffer::bindToTransformFeedback() {

	glBindBuffer( (m_bind = GL_TRANSFORM_FEEDBACK_BUFFER), m_id );
}
void GlBuffer::bindToUniform() {

	glBindBuffer( (m_bind = GL_UNIFORM_BUFFER), m_id );
}

void GlBuffer::unbind() {

	glBindBuffer( m_bind, NULL );
	m_bind = GL_NONE;
}

unsigned int GlBuffer::getBind() const {

	return m_bind;
}
unsigned int GlBuffer::getId() const {

	return m_id;
}

///////////////////////////////////////////////////////////////////////////////
// GlVAObject
///////////////////////////////////////////////////////////////////////////////

GlVAObject::GlVAObject() {

	glGenVertexArrays( 1, &m_id );
}

GlVAObject::~GlVAObject() {

	glDeleteVertexArrays( 1, &m_id );
}

void GlVAObject::bind() {

	glBindVertexArray( m_id );
}

void GlVAObject::unbind() {

	glBindVertexArray( NULL );
}

unsigned int GlVAObject::getId() const {

	return m_id;
}

///////////////////////////////////////////////////////////////////////////////
// GlVertexAttribute
///////////////////////////////////////////////////////////////////////////////

GlVertexAttribute GlVertexAttribute::skipAttribute( unsigned int size ) {

	return GlVertexAttribute( GL_NONE, size );
}
GlVertexAttribute GlVertexAttribute::byteAttribute( unsigned int size ) {

	return GlVertexAttribute( GL_BYTE, size );
}
GlVertexAttribute GlVertexAttribute::ubyteAttribute( unsigned int size ) {

	return GlVertexAttribute( GL_UNSIGNED_BYTE, size );
}
GlVertexAttribute GlVertexAttribute::shortAttribute( unsigned int size ) {

	return GlVertexAttribute( GL_SHORT, size );
}
GlVertexAttribute GlVertexAttribute::ushortAttribute( unsigned int size ) {

	return GlVertexAttribute( GL_UNSIGNED_SHORT, size );
}
GlVertexAttribute GlVertexAttribute::intAttribute( unsigned int size ) {

	return GlVertexAttribute( GL_INT, size );
}
GlVertexAttribute GlVertexAttribute::uintAttribute( unsigned int size ) {

	return GlVertexAttribute( GL_UNSIGNED_INT, size );
}
GlVertexAttribute GlVertexAttribute::halfAttribute( unsigned int size ) {

	return GlVertexAttribute( GL_HALF_FLOAT, size );
}
GlVertexAttribute GlVertexAttribute::floatAttribute( unsigned int size ) {

	return GlVertexAttribute( GL_FLOAT, size );
}
GlVertexAttribute GlVertexAttribute::doubleAttribute( unsigned int size ) {

	return GlVertexAttribute( GL_DOUBLE, size );
}

GlVertexAttribute::~GlVertexAttribute() {

}

GlVertexAttribute::GlVertexAttribute( unsigned int t_type, unsigned int t_size )
	: m_type{ t_type }, m_size{ t_size } {

}

unsigned int GlVertexAttribute::getByteSize() const {

	switch ( m_type ) {
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			return m_size;
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
		case GL_HALF_FLOAT:
			return m_size * 2;
		case GL_INT:
		case GL_UNSIGNED_INT:
		case GL_FLOAT:
			return m_size * 4;
		case GL_DOUBLE:
			return m_size * 8;
	}
	return 0;
}

unsigned int GlVertexAttribute::getSize() const {

	return m_size;
}

unsigned int GlVertexAttribute::getType() const {

	return m_type;
}

///////////////////////////////////////////////////////////////////////////////
// GlVertexDefinition
///////////////////////////////////////////////////////////////////////////////

GlVertexDefinition::GlVertexDefinition( const GlVertexDefinition & t_other ) 
	: m_attributes{ t_other.m_attributes.begin(), t_other.m_attributes.end() } {

	findByteSize();
}

GlVertexDefinition::~GlVertexDefinition() {

}

void GlVertexDefinition::bind() {

	unsigned int offset{ 0 };
	unsigned int index{ 0 };

	for ( auto attribute : m_attributes ) {
		
		if ( attribute.getType() != GL_NONE ) {

			glEnableVertexAttribArray( index );
			glVertexAttribPointer( index, attribute.getSize(), attribute.getType(), GL_FALSE, m_byteSize, (void*)(offset) );
		}

		++index;
		offset += attribute.getByteSize();
	}
}

void GlVertexDefinition::enable() {

	unsigned int index{ 0 };
	for ( auto attribute : m_attributes ) {

		if ( attribute.getType() != GL_NONE ) {

			glEnableVertexAttribArray( index );
		}

		++index;
	}
}


void GlVertexDefinition::unbind() {

	unsigned int index{ 0 };
	for ( auto attribute : m_attributes ) {

		if ( attribute.getType() != GL_NONE ) {
		
			glDisableVertexAttribArray( index );
		}

		++index;
	}
}

void GlVertexDefinition::findByteSize() {

	m_byteSize = 0;
	for ( auto attribute : m_attributes ) {

		m_byteSize += attribute.getByteSize();
	}
}

///////////////////////////////////////////////////////////////////////////////
// GlMesh
///////////////////////////////////////////////////////////////////////////////

GlMesh::GlMesh( const GlVertexDefinition& t_vertexDefinition ) : m_vertexDefinition( t_vertexDefinition ) {
	
	GlGuard guards[] = {
		{ m_vertexDefinition },
		{ m_indexBuffer },
		{ m_vertexBuffer },
		{ m_vertexArrayObject } };

	m_vertexArrayObject.bind();
	m_indexBuffer.bindToElementArray();
	m_vertexBuffer.bindToArray();
	m_vertexDefinition.bind();
}

GlMesh::~GlMesh() {

}


void GlMesh::fillVerticesStatic( void* const t_dataPointer, unsigned int t_dataLength ) {

	GlGuard guards[] = {
		{ m_vertexDefinition },
		{ m_vertexBuffer },
		{ m_vertexArrayObject } };

	m_vertexArrayObject.bind();
	m_vertexBuffer.bindToArray();
	m_vertexBuffer.fillStaticDraw( t_dataPointer, t_dataLength );
	m_vertexDefinition.bind();
}
void GlMesh::fillVerticesDynamic( void* const t_dataPointer, unsigned int t_dataLength ) {

	GlGuard guards[] = {
		{ m_vertexDefinition },
		{ m_vertexBuffer },
		{ m_vertexArrayObject } };

	m_vertexArrayObject.bind();
	m_vertexBuffer.bindToArray();
	m_vertexBuffer.fillDynamicDraw( t_dataPointer, t_dataLength );
	m_vertexDefinition.bind();
}
void GlMesh::fillVerticesStream( void* const t_dataPointer, unsigned int t_dataLength ) {

	GlGuard guards[] = {
		{ m_vertexDefinition },
		{ m_indexBuffer },
		{ m_vertexBuffer },
		{ m_vertexArrayObject } };

	m_vertexArrayObject.bind();
	m_vertexBuffer.bindToArray();
	m_vertexBuffer.fillStreamDraw( t_dataPointer, t_dataLength );
	m_vertexDefinition.bind();
}

void GlMesh::findIndexType( unsigned int t_size ) {

	switch ( t_size ) {

		case 1:
			m_indexType = GL_UNSIGNED_BYTE;
			break;
		case 2:
			m_indexType = GL_UNSIGNED_SHORT;
			break;
		case 4:
			m_indexType = GL_UNSIGNED_INT;
		default:
			assert( false && "index type size should only be 1, 2 or 4" );
	}
}
void GlMesh::fillIndicesStatic( const void* const t_dataPointer, unsigned int t_dataLength ) {

	m_indexBuffer.fillStaticDraw( t_dataPointer, t_dataLength );
}
void GlMesh::fillIndicesDynamic( const void* const t_dataPointer, unsigned int t_dataLength ) {

	m_indexBuffer.fillDynamicDraw( t_dataPointer, t_dataLength );
}
void GlMesh::fillIndicesStream( const void* const t_dataPointer, unsigned int t_dataLength ) {

	m_indexBuffer.fillStreamDraw( t_dataPointer, t_dataLength );
}

void GlMesh::bind() {

	m_vertexArrayObject.bind();
	m_vertexDefinition.enable();
}

void GlMesh::unbind() {

	m_vertexArrayObject.unbind();
	m_vertexDefinition.unbind();
}

///////////////////////////////////////////////////////////////////////////////
// ShaderFactory
///////////////////////////////////////////////////////////////////////////////

std::string generateFragmentShader( const int ) {

	return "#version 120 main(){ gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 ); }";
}

std::string generateVertexShader( const int ) {

	return "#version 120 attribute vec3 attributePosition main(){ gl_Position = vec4( attributePosition, 1.0 ); }";
}

std::string pantheon::generateShader( const int t_flags ) {

	if ( (t_flags & PANTHEON_SHADER_TYPE_MASK) == PANTHEON_SHADER_FRAGMENT ) {

		return generateFragmentShader( t_flags );
	}
	return generateVertexShader( t_flags );
}

///////////////////////////////////////////////////////////////////////////////
// ShaderCompileException
///////////////////////////////////////////////////////////////////////////////

ShaderCompileException::ShaderCompileException( const GlShader& t_shader ) {

	const unsigned int id{ t_shader.getId() };
	int length;
	glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length );
	m_errorString.resize( length );
	glGetShaderInfoLog( id, length, NULL, &m_errorString[0] );
}

ShaderCompileException::~ShaderCompileException() {

}

const char* ShaderCompileException::what() const {

	return m_errorString.c_str();
}

///////////////////////////////////////////////////////////////////////////////
// GlShader
///////////////////////////////////////////////////////////////////////////////

GlShader::GlShader( unsigned int t_type ) : m_type{ t_type } {

	m_id = glCreateShader( m_type );
}

GlShader::~GlShader() {

	glDeleteShader( m_id );
}

unsigned int GlShader::computeShader() {

	return GL_COMPUTE_SHADER;
}

unsigned int GlShader::fragmentShader() {

	return GL_FRAGMENT_SHADER;
}

unsigned int GlShader::geometryShader() {

	return GL_GEOMETRY_SHADER;
}

unsigned int GlShader::tessControlShader() {

	return GL_TESS_CONTROL_SHADER;
}

unsigned int GlShader::tessEvaluationShader() {

	return GL_TESS_EVALUATION_SHADER;
}

unsigned int GlShader::vertexShader() {

	return GL_VERTEX_SHADER;
}

void GlShader::compileFromFile( const std::string& t_path ) {

	std::string source{ getFileAsString( t_path ) };
	compile( source );
}

void GlShader::compile(const std::string& t_source) {

	const char * const source{ t_source.c_str() };
	int length{ static_cast<int>(t_source.length()) };
	glShaderSource( m_id, 1, &source, &length);
	glCompileShader( m_id );
	int isSuccess;
	glGetShaderiv( m_id, GL_COMPILE_STATUS, &isSuccess );
	if ( !isSuccess ) {

		throw ShaderCompileException( *this );
	}
}

unsigned int GlShader::getId() const {

	return m_id;
}

unsigned int GlShader::getType() const {

	return m_type;
}


///////////////////////////////////////////////////////////////////////////////
// GlUniform
///////////////////////////////////////////////////////////////////////////////

GlUniform::GlUniform( const std::string& t_name, int t_location, unsigned int t_type, int t_size ) 
	: m_name{ t_name }, m_location{ t_location }, m_type { t_type }, m_size{ t_size } {

}
GlUniform::~GlUniform() {

}

void GlUniform::setValue( int t_value ) {
	
	assert( (m_type == GL_SAMPLER_1D || m_type == GL_SAMPLER_2D || m_type == GL_SAMPLER_3D || m_type == GL_INT) && "Incorrect type for this uniform." );
	glUniform1i( m_location, t_value );
}
void GlUniform::setValue( unsigned int t_value ) {
	
	assert( m_type == GL_UNSIGNED_INT && "Incorrect type for this uniform." );
	glUniform1ui( m_location, t_value );
}
void GlUniform::setValue( float t_value ) {

	assert( m_type == GL_FLOAT && "Incorrect type for this uniform." );
	glUniform1f( m_location, t_value );
}
void GlUniform::setValue( double t_value ) {

	assert( m_type == GL_DOUBLE && "Incorrect type for this uniform." );
	glUniform1d( m_location, t_value );
}
void GlUniform::setValue( std::vector<int> t_value ) {
	
	int size = t_value.size();
	switch ( m_type ) {

		case GL_SAMPLER_1D:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_BOOL:
		case GL_INT:
			assert( size > 0 && size <= m_size && "Element count incorrect for the uniform." );
			glUniform1iv( m_location, size, &t_value[0] );
			break;
		case GL_BOOL_VEC2:
		case GL_INT_VEC2:
			assert( size > 0 && size % 2 == 0 && size <= m_size * 2 && "Element count incorrect for the uniform." );
			glUniform2iv( m_location, size / 2, &t_value[0] );
			break;
		case GL_BOOL_VEC3:
		case GL_INT_VEC3:
			assert( size > 0 && size % 3 == 0 && size <= m_size * 3 && "Element count incorrect for the uniform." );
			glUniform3iv( m_location, size / 3, &t_value[0] );
			break;
		case GL_BOOL_VEC4:
		case GL_INT_VEC4:
			assert( size > 0 && size % 4 == 0 && size <= m_size * 4 && "Element count incorrect for the uniform." );
			glUniform4iv( m_location, size / 4, &t_value[0] );
			break;
		default:
			assert( false && "Incorrect type for this uniform." );
	}
}
void GlUniform::setValue( std::vector<unsigned int> t_value ) {

	int size = t_value.size();
	switch ( m_type ) {

		case GL_UNSIGNED_INT:
			assert( size > 0 && size <= m_size && "Element count incorrect for the uniform." );
			glUniform1uiv( m_location, size, &t_value[0] );
			break;
		case GL_UNSIGNED_INT_VEC2:
			assert( size > 0 && size % 2 == 0 && size <= m_size * 2 && "Element count incorrect for the uniform." );
			glUniform2uiv( m_location, size / 2, &t_value[0] );
			break;
		case GL_UNSIGNED_INT_VEC3:
			assert( size > 0 && size % 3 == 0 && size <= m_size * 3 && "Element count incorrect for the uniform." );
			glUniform3uiv( m_location, size / 3, &t_value[0] );
			break;
		case GL_UNSIGNED_INT_VEC4:
			assert( size > 0 && size % 4 == 0 && size <= m_size * 4 && "Element count incorrect for the uniform." );
			glUniform4uiv( m_location, size / 4, &t_value[0] );
			break;
	}
}
void GlUniform::setValue( std::vector<float> t_value ) {

	int size = t_value.size();
	switch ( m_type ) {

		case GL_FLOAT:
			assert( size > 0 && size <= m_size && "Element count incorrect for the uniform." );
			glUniform1fv( m_location, size, &t_value[0] );
			break;
		case GL_FLOAT_VEC2:
			assert( size > 0 && size % 2 == 0 && size <= m_size * 2 && "Element count incorrect for the uniform." );
			glUniform2fv( m_location, size / 2, &t_value[0] );
			break;
		case GL_FLOAT_VEC3:
			assert( size > 0 && size % 3 == 0 && size <= m_size * 3 && "Element count incorrect for the uniform." );
			glUniform3fv( m_location, size / 3, &t_value[0] );
			break;
		case GL_FLOAT_VEC4:
			assert( size > 0 && size % 4 == 0 && size <= m_size * 4 && "Element count incorrect for the uniform." );
			glUniform4fv( m_location, size / 4, &t_value[0] );
			break;
		case GL_FLOAT_MAT2:
			assert( size > 0 && size % 4 == 0 && size <= m_size * 4 && "Element count incorrect for the uniform." );
			glUniformMatrix2fv( m_location, size / 4, GL_FALSE, &t_value[0] );
			break;
		case GL_FLOAT_MAT2x3:
			assert( size > 0 && size % 6 == 0 && size <= m_size * 6 && "Element count incorrect for the uniform." );
			glUniformMatrix2x3fv( m_location, size / 6, GL_FALSE, &t_value[0] );
			break;
		case GL_FLOAT_MAT2x4:
			assert( size > 0 && size % 8 == 0 && size <= m_size * 8 && "Element count incorrect for the uniform." );
			glUniformMatrix2x4fv( m_location, size / 8, GL_FALSE, &t_value[0] );
			break;
		case GL_FLOAT_MAT3:
			assert( size > 0 && size % 9 == 0 && size <= m_size * 9 && "Element count incorrect for the uniform." );
			glUniformMatrix3fv( m_location, size / 9, GL_FALSE, &t_value[0] );
			break;
		case GL_FLOAT_MAT3x2:
			assert( size > 0 && size % 6 == 0 && size <= m_size * 6 && "Element count incorrect for the uniform." );
			glUniformMatrix3x2fv( m_location, size / 6, GL_FALSE, &t_value[0] );
			break;
		case GL_FLOAT_MAT3x4:
			assert( size > 0 && size % 12 == 0 && size <= m_size * 12 && "Element count incorrect for the uniform." );
			glUniformMatrix3x4fv( m_location, size / 12, GL_FALSE, &t_value[0] );
			break;
		case GL_FLOAT_MAT4:
			assert( size > 0 && size % 16 == 0 && size <= m_size * 16 && "Element count incorrect for the uniform." );
			glUniformMatrix4fv( m_location, size / 16, GL_FALSE, &t_value[0] );
			break;
		case GL_FLOAT_MAT4x2:
			assert( size > 0 && size % 8 == 0 && size <= m_size * 8 && "Element count incorrect for the uniform." );
			glUniformMatrix4x2fv( m_location, size / 8, GL_FALSE, &t_value[0] );
			break;
		case GL_FLOAT_MAT4x3:
			assert( size > 0 && size % 12 == 0 && size <= m_size * 12 && "Element count incorrect for the uniform." );
			glUniformMatrix4x3fv( m_location, size / 12, GL_FALSE, &t_value[0] );
			break;
		default:
			assert( false && "Incorrect type for this uniform." );
	}
}
void GlUniform::setValue( std::vector<double> t_value ) {

	int size = t_value.size();
	switch ( m_type ) {

		case GL_DOUBLE:
			assert( size > 0 && size <= m_size && "Element count incorrect for the uniform." );
			glUniform1dv( m_location, size, &t_value[0] );
			break;
		case GL_DOUBLE_VEC2:
			assert( size > 0 && size % 2 == 0 && size <= m_size * 2 && "Element count incorrect for the uniform." );
			glUniform2dv( m_location, size / 2, &t_value[0] );
			break;
		case GL_DOUBLE_VEC3:
			assert( size > 0 && size % 3 == 0 && size <= m_size * 3 && "Element count incorrect for the uniform." );
			glUniform3dv( m_location, size / 3, &t_value[0] );
			break;
		case GL_DOUBLE_VEC4:
			assert( size > 0 && size % 4 == 0 && size <= m_size * 4 && "Element count incorrect for the uniform." );
			glUniform4dv( m_location, size / 4, &t_value[0] );
			break;
		case GL_DOUBLE_MAT2:
			assert( size > 0 && size % 4 == 0 && size <= m_size * 4 && "Element count incorrect for the uniform." );
			glUniformMatrix2dv( m_location, size / 4, GL_FALSE, &t_value[0] );
			break;
		case GL_DOUBLE_MAT2x3:
			assert( size > 0 && size % 6 == 0 && size <= m_size * 6 && "Element count incorrect for the uniform." );
			glUniformMatrix2x3dv( m_location, size / 6, GL_FALSE, &t_value[0] );
			break;
		case GL_DOUBLE_MAT2x4:
			assert( size > 0 && size % 8 == 0 && size <= m_size * 8 && "Element count incorrect for the uniform." );
			glUniformMatrix2x4dv( m_location, size / 8, GL_FALSE, &t_value[0] );
			break;
		case GL_DOUBLE_MAT3:
			assert( size > 0 && size % 9 == 0 && size <= m_size * 9 && "Element count incorrect for the uniform." );
			glUniformMatrix3dv( m_location, size / 9, GL_FALSE, &t_value[0] );
			break;
		case GL_DOUBLE_MAT3x2:
			assert( size > 0 && size % 6 == 0 && size <= m_size * 6 && "Element count incorrect for the uniform." );
			glUniformMatrix3x2dv( m_location, size / 6, GL_FALSE, &t_value[0] );
			break;
		case GL_DOUBLE_MAT3x4:
			assert( size > 0 && size % 12 == 0 && size <= m_size * 12 && "Element count incorrect for the uniform." );
			glUniformMatrix3x4dv( m_location, size / 12, GL_FALSE, &t_value[0] );
			break;
		case GL_DOUBLE_MAT4:
			assert( size > 0 && size % 16 == 0 && size <= m_size * 16 && "Element count incorrect for the uniform." );
			glUniformMatrix4dv( m_location, size / 16, GL_FALSE, &t_value[0] );
			break;
		case GL_DOUBLE_MAT4x2:
			assert( size > 0 && size % 8 == 0 && size <= m_size * 8 && "Element count incorrect for the uniform." );
			glUniformMatrix4x2dv( m_location, size / 8, GL_FALSE, &t_value[0] );
			break;
		case GL_DOUBLE_MAT4x3:
			assert( size > 0 && size % 12 == 0 && size <= m_size * 12 && "Element count incorrect for the uniform." );
			glUniformMatrix4x3dv( m_location, size / 12, GL_FALSE, &t_value[0] );
			break;
		default:
			assert( false && "Incorrect type for this uniform." );
	}
}

std::string GlUniform::getName() {

	return m_name;
}
int GlUniform::getLocation() {

	return m_location;
}
unsigned int GlUniform::getType() {

	return m_type;
}
int GlUniform::getSize() {

	return m_size;
}

///////////////////////////////////////////////////////////////////////////////
// ProgramLinkException
///////////////////////////////////////////////////////////////////////////////

ProgramLinkException::ProgramLinkException( const GlProgram& t_program ) {

	const unsigned int id{ t_program.getId() };
	int length;
	glGetProgramiv( id, GL_INFO_LOG_LENGTH, &length );
	m_errorString.resize( length );
	glGetProgramInfoLog( id, length, NULL, &m_errorString[0] );
}

ProgramLinkException::~ProgramLinkException() {

}

const char* ProgramLinkException::what() const {

	return m_errorString.c_str();
}

///////////////////////////////////////////////////////////////////////////////
// ProgramUniformDoesNotExistException
///////////////////////////////////////////////////////////////////////////////

ProgramUniformDoesNotExistException::ProgramUniformDoesNotExistException( const std::string& t_uniformName )
	: m_errorString{ "A uniform of name \"" + t_uniformName + "\" does not exist in this program." } {

}

ProgramUniformDoesNotExistException::~ProgramUniformDoesNotExistException() {

}

const char* ProgramUniformDoesNotExistException::what() const {

	return m_errorString.c_str();
}

///////////////////////////////////////////////////////////////////////////////
// GlProgram
///////////////////////////////////////////////////////////////////////////////

GlProgram::GlProgram() {

	m_id = glCreateProgram();
}

GlProgram::~GlProgram() {

	glDeleteProgram( m_id );
}

void GlProgram::attachShader( const GlShader& t_shader ) {

	glAttachShader( m_id, t_shader.getId() );
}

void GlProgram::bindAttributeLocation( const std::string& t_name, unsigned int t_location ) {

	glBindAttribLocation( m_id, t_location, t_name.c_str() );
}

void GlProgram::link() {

	glLinkProgram( m_id );
	int isSuccess;
	glGetProgramiv( m_id, GL_LINK_STATUS, &isSuccess );
	if ( !isSuccess ) {

		throw ProgramLinkException( *this );
	}
	int uniformCount;
	glGetProgramiv( m_id, GL_ACTIVE_UNIFORMS, &uniformCount );

	int nameLength;
	char outName[64];
	int location;
	unsigned int type;
	int size;
	for ( int i = 0; i < uniformCount; ++i ) {

		glGetActiveUniform( m_id, i, 64, &nameLength, &size, &type, outName );
		location = glGetUniformLocation( m_id, outName );
		std::string name{ outName, outName + nameLength };
		m_uniforms.insert( { name, { name, location, type, size } } );
	}
}

GlUniform& GlProgram::getUniform( const std::string& t_name ) {

	auto pair = m_uniforms.find( t_name );
	if ( pair == m_uniforms.end() ) {

		throw ProgramUniformDoesNotExistException( t_name );
	}
	return pair->second;
}

void GlProgram::bind() {

	glUseProgram( m_id );
}

void GlProgram::unbind() {

	glUseProgram( NULL );
}

unsigned int GlProgram::getId() const {

	return m_id;
}

///////////////////////////////////////////////////////////////////////////////
// TextureInvalidDataLengthException
///////////////////////////////////////////////////////////////////////////////

TextureInvalidDataLengthException::TextureInvalidDataLengthException
	( unsigned int t_expectedLength, unsigned int t_actualLength )
	: m_expectedLength{ t_expectedLength }, m_actualLength{ t_actualLength }
	, m_errorString{ "Invalid amount of data given to texture, expected amount divisible by " + std::to_string( t_expectedLength ) 
		+ " resulting in a factor of 4 (or 8 in the case of floating values), instead got " + std::to_string( t_actualLength ) } {

}
TextureInvalidDataLengthException::~TextureInvalidDataLengthException() {

}

const char* TextureInvalidDataLengthException::what() const {

	return m_errorString.c_str();
}

unsigned int TextureInvalidDataLengthException::getExpectedDataLength() const {

	return m_expectedLength;
}
unsigned int TextureInvalidDataLengthException::getActualDataLength() const {

	return m_actualLength;
}

///////////////////////////////////////////////////////////////////////////////
// TextureCouldNotLoadException
///////////////////////////////////////////////////////////////////////////////

TextureCouldNotLoadException::TextureCouldNotLoadException() : m_errorString{ IMG_GetError() } {

}
TextureCouldNotLoadException::~TextureCouldNotLoadException() {

}

const char* TextureCouldNotLoadException::what() const {

	return m_errorString.c_str();
}

///////////////////////////////////////////////////////////////////////////////
// TextureNotSupportedException
///////////////////////////////////////////////////////////////////////////////

TextureNotSupportedException::TextureNotSupportedException( const std::string& t_reason ) 
	: m_reason{ t_reason }, m_errorString{ "Texture loaded is not supported for the following reason: " + t_reason } {

}
TextureNotSupportedException::~TextureNotSupportedException() {

}
const char* TextureNotSupportedException::what() const {

	return m_errorString.c_str();
}

const char* TextureNotSupportedException::getReason() const {

	return m_reason.c_str();
}

///////////////////////////////////////////////////////////////////////////////
// GlTexture2D
///////////////////////////////////////////////////////////////////////////////

GlTexture2D::GlTexture2D() : m_bind{ -1 } {

	glGenTextures( 1, &m_id );
}

GlTexture2D::~GlTexture2D() {

	glDeleteTextures( 1, &m_id );
}

void GlTexture2D::fillFromFile( const std::string& t_path ) {

	// only needed in this single context, ensures RAII behaviour for surface
	class SurfaceGuard {

	public:
		SurfaceGuard( SDL_Surface* t_surface ) : m_surface{ t_surface } {

		}
		~SurfaceGuard() {

			SDL_FreeSurface( m_surface );
		}
	private:
		SDL_Surface* m_surface;
	};

	SDL_Surface* texture;
	texture = IMG_Load( t_path.c_str() );
	if ( texture == nullptr ) {

		throw TextureCouldNotLoadException();
	}

	SurfaceGuard surfaceGuard{ texture };

	unsigned int format = GL_NONE;
	unsigned int internalFormat = GL_NONE;
	switch ( texture->format->BytesPerPixel ) {
		case 1:
			format = GL_RED;
			internalFormat = GL_R8;
			break;
		case 2:
			format = GL_RG;
			internalFormat = GL_RG8;
			break;
		case 3:
			format = GL_RGB;
			internalFormat = GL_RGB8;
			break;
		case 4:
			format = GL_RGBA;
			internalFormat = GL_RGBA8;
			break;
		default:
			throw TextureNotSupportedException( "Unrecognised bytes per pixel format." );
			break;
	}
	fill( 0, internalFormat, texture->w, texture->h, 0, format, GL_UNSIGNED_BYTE
		, texture->pixels );
	setWrapRepeat();
}

void GlTexture2D::fill( int t_level, unsigned int t_internalFormat
	, unsigned int t_width, unsigned int t_height
	, int t_border, unsigned int t_format, unsigned int t_type
	, const void* const t_dataPointer ) {

	assert( m_bind >= 0 && "A texture must be bound in order to be filled." );
	glTexImage2D( GL_TEXTURE_2D, t_level, t_internalFormat, t_width, t_height
		, t_border, t_format, t_type, t_dataPointer );
	glGenerateMipmap( GL_TEXTURE_2D );
	setMinLinearMipMapLinear();
	setMagLinear();
}

void GlTexture2D::setMinNear() {

	assert( m_bind >= 0 && "A texture must be bound in order to be filled." );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
}
void GlTexture2D::setMinLinear() {

	assert( m_bind >= 0 && "A texture must be bound in order to be filled." );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
}
void GlTexture2D::setMinNearMipMapNear() {

	assert( m_bind >= 0 && "A texture must be bound in order to be filled." );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
}
void GlTexture2D::setMinNearMipMapLinear() {

	assert( m_bind >= 0 && "A texture must be bound in order to be filled." );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
}
void GlTexture2D::setMinLinearMipMapNear() {

	assert( m_bind >= 0 && "A texture must be bound in order to be filled." );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
}
void GlTexture2D::setMinLinearMipMapLinear() {

	assert( m_bind >= 0 && "A texture must be bound in order to be filled." );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
}
void GlTexture2D::setMagNear() {

	assert( m_bind >= 0 && "A texture must be bound in order to be filled." );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}
void GlTexture2D::setMagLinear() {

	assert( m_bind >= 0 && "A texture must be bound in order to be filled." );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}
void GlTexture2D::setMinMagNear() {

	setMinNear();
	setMagNear();
}
void GlTexture2D::setMinMagLinear() {

	setMinLinear();
	setMagLinear();
}
void GlTexture2D::setWrapRepeat() {

	assert( m_bind >= 0 && "A texture must be bound in order to be filled." );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}
void GlTexture2D::setWrapClamp() {

	assert( m_bind >= 0 && "A texture must be bound in order to be filled." );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
}


void GlTexture2D::bind( unsigned int t_index ) {

	m_bind = GL_TEXTURE0 + t_index;
	glActiveTexture( GL_TEXTURE0 + t_index );
	glBindTexture( GL_TEXTURE_2D, m_id );
}

void GlTexture2D::unbind() {

	if ( m_bind >= 0 ) {
		glActiveTexture( m_bind );
		glBindTexture( GL_TEXTURE_2D, NULL );
		m_bind = -1;
	}
}

unsigned int GlTexture2D::findFormat( unsigned int t_width, unsigned int t_height, unsigned int t_dataLength ) {

	double totalSize = t_width * t_height;
	double componentCountCheck = static_cast<double>(t_dataLength) / totalSize;
	if ( floor( componentCountCheck ) != componentCountCheck ) {

		throw TextureInvalidDataLengthException( t_width * t_height, t_dataLength );
	}
	unsigned int componentCount = t_dataLength / (t_width * t_height);
	switch ( componentCount ) {
		case 1:
			return GL_RED;
		case 2:
			return GL_RG;
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
	}
	throw TextureInvalidDataLengthException( t_width * t_height, t_dataLength );
}
unsigned int GlTexture2D::findSignedType( unsigned int t_size ) {

	switch ( t_size ) {
		case 1:
			return GL_BYTE;
		case 2:
			return GL_SHORT;
		case 4:
			return GL_INT;
	}
	assert( false && "A size which is not a factor of 4 should not be given here." );
	return GL_NONE;
}
unsigned int GlTexture2D::findUnsignedType( unsigned int t_size ) {
	
	switch ( t_size ) {
		case 1:
			return GL_UNSIGNED_BYTE;
		case 2:
			return GL_UNSIGNED_SHORT;
		case 4:
			return GL_UNSIGNED_INT;
	}
	assert( false && "A size which is not a factor of 4 should not be given here." );
	return GL_NONE;
}
unsigned int GlTexture2D::findFloatingType( unsigned int t_size ) {

	switch ( t_size ) {
		case 4:
			return GL_FLOAT;
		case 8:
			return GL_DOUBLE;
	}
	assert( false && "A size which is not 4 or 8 should not be given here." );
	return GL_NONE;
}
unsigned int GlTexture2D::getId() {

	return m_id;
}

///////////////////////////////////////////////////////////////////////////////
// GlFrameBuffer
///////////////////////////////////////////////////////////////////////////////

GlFrameBuffer::GlFrameBuffer( unsigned int t_targetCount ) : m_targetCount{ t_targetCount } {

	glGenFramebuffers( 1, &m_id );
	m_targets.resize( t_targetCount );
}

GlFrameBuffer::~GlFrameBuffer() {

	glDeleteFramebuffers( 1, &m_id );
}

void GlFrameBuffer::bind() {

	glBindFramebuffer( GL_FRAMEBUFFER, m_id );
}

void GlFrameBuffer::unbind() {

	glBindFramebuffer( GL_FRAMEBUFFER, NULL );
}

void GlFrameBuffer::attachColourTarget( unsigned int t_index, GlTexture2D& t_target ) {

	assert( t_index < m_targetCount && t_index >= 0 && "Index of target is out of range of framebuffer." );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + t_index, GL_TEXTURE_2D, t_target.getId(), 0 );
	m_targets[t_index] = t_target.getId();
}
void GlFrameBuffer::detachColourTarget( unsigned int t_index ) {

	assert( t_index < m_targetCount && t_index >= 0 && "Index of target is out of range of framebuffer." );

	glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + t_index, NULL, 0 );
	m_targets[t_index] = NULL;
}
void GlFrameBuffer::completeAttachment() {

	std::vector<unsigned int> drawBuffers;
	drawBuffers.resize( m_targetCount );
	for ( int i = 0; i < m_targetCount; ++i ) {

		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers( m_targetCount, &drawBuffers[0] );
}
std::string GlFrameBuffer::getStatus() {

	switch ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) ) {

		case GL_FRAMEBUFFER_COMPLETE:
			return "Framebuffer status is complete";
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			return "Framebuffer attachment is incomplete";
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			return "Framebuffer is missing an attachment";
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			return "Framebuffer draw buffer is incomplete";
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			return "Framebuffer read buffer is incomplete";
		case GL_FRAMEBUFFER_UNSUPPORTED:
			return "Framebuffer is not supported";
	}
	return "Unknown framebuffer status";
}
bool GlFrameBuffer::isComplete() {

	return glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE;
}
unsigned int GlFrameBuffer::getId() {

	return m_id;
}
unsigned int GlFrameBuffer::getTargetCount() {

	return m_targetCount;
}