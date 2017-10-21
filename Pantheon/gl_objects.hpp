
// gl_objects.hpp

// header file for wrapping up opengl functionality in a RAII compliant objects

#ifndef PANTHEON_GL_OBJECTS_HPP
#define PANTHEON_GL_OBJECTS_HPP

#include "pantheon.hpp"

#include <string>
#include <vector>
#include <map>
#include <memory>

#define GLERRCASE(X) case (X) : printf( #X ); assert( false ); break;
#define GLERROR { switch( glGetError() ) { GLERRCASE(GL_OUT_OF_MEMORY) GLERRCASE(GL_INVALID_ENUM) GLERRCASE(GL_INVALID_VALUE) GLERRCASE(GL_INVALID_OPERATION) GLERRCASE(GL_INVALID_FRAMEBUFFER_OPERATION) case GL_NO_ERROR: break; default: assert( false ); break; } }

namespace pantheon {

	// forward declarations

	class IGlObject;
	class GlGuard;
	class GlBuffer;
	class GlVAObject;
	class GlVertexDefinition;
	class GlMesh;
	class ShaderCompileException;
	class GlShader;
	class ProgramLinkException;
	class GlProgram;
	class TextureInvalidDataLengthException;
	class GlTexture2D;

#ifdef PANTHEON_GL_DEBUG

	// an expensive debug object, which requests everything it can about opengl's state and exposes it to the IDE

	class GlDebug {

	public:

		class GlDebugVertexAttrib {

		public:
			GlDebugVertexAttrib(int t_index);
			~GlDebugVertexAttrib();

			void update();

			int index;
			int bufferBinding;
			int enabled;
			int size;
			int stride;
			int type;
			int normalized;
			int integer;
			int divisor;

		};

		GlDebug();
		~GlDebug();

		void update();

		int arrayBufferBinding;
		int atomicCounterBufferBinding;
		int copyReadBufferBinding;
		int copyWriteBufferBinding;
		int dispatchIndirectBufferBinding;
		int drawIndirectBufferBinding;
		int elementArrayBufferBinding;
		int pixelPackBufferBinding;
		int pixelUnpackBufferBinding;
		int queryBufferBinding;
		int shaderStorageBufferBinding;
		int textureBufferBinding;
		int transformFeedbackBufferBinding;
		int uniformBufferBinding;

		int textureBinding1D;
		int textureBinding2D;
		int textureBinding3D;

		int maxVertexAttribs;
		std::vector<GlDebugVertexAttrib> vertexAttribs;
		int activeTexture;
	};
#endif

	class IGlException : public std::exception {

	public:

		virtual ~IGlException() = 0 {

		}
	};

	// base class for all gl object classes ensuring they can be unbound safely

	class IGlObject {

	public:

		virtual ~IGlObject() = 0 {

		}

		virtual void unbind() = 0;
	};

	// a guard for gl object classes ensuring even in the event of exceptions
	// the given object will be unbound before exiting the function

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

		IGlObject& m_object;
	};

	// gl buffer object class

	class PANTHEON_API GlBuffer : public IGlObject {

	public:

		GlBuffer();
		GlBuffer( const GlBuffer& ) = delete;
		GlBuffer& operator=( const GlBuffer& ) = delete;
		~GlBuffer();

		void fillStaticDraw( const void* const t_dataPointer, unsigned int t_dataLength );
		void fillDynamicDraw( const void* const t_dataPointer, unsigned int t_dataLength );
		void fillStreamDraw( const void* const t_dataPointer, unsigned int t_dataLength );

		void fillStaticRead( const void* const t_dataPointer, unsigned int t_dataLength );
		void fillDynamicRead( const void* const t_dataPointer, unsigned int t_dataLength );
		void fillStreamRead( const void* const t_dataPointer, unsigned int t_dataLength );

		void fillStaticCopy( const void* const t_dataPointer, unsigned int t_dataLength );
		void fillDynamicCopy( const void* const t_dataPointer, unsigned int t_dataLength );
		void fillStreamCopy( const void* const t_dataPointer, unsigned int t_dataLength );

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

		unsigned int m_bind;
		unsigned int m_id;
	};

	// gl vertex array object class

	class PANTHEON_API GlVAObject : public IGlObject {

	public:

		GlVAObject();
		GlVAObject( const GlVAObject& ) = delete;
		GlVAObject& operator=( const GlVAObject& ) = delete;
		~GlVAObject();

		void bind();
		void unbind() override;

		unsigned int getId() const;

	private:

		unsigned int m_id;
	};

	// a data object containing the definition of a vertex attribute

	class PANTHEON_API GlVertexAttribute {

	public:

		static GlVertexAttribute skipAttribute( unsigned int size );
		static GlVertexAttribute byteAttribute( unsigned int size );
		static GlVertexAttribute ubyteAttribute( unsigned int size );
		static GlVertexAttribute shortAttribute( unsigned int size );
		static GlVertexAttribute ushortAttribute( unsigned int size );
		static GlVertexAttribute intAttribute( unsigned int size );
		static GlVertexAttribute uintAttribute( unsigned int size );
		static GlVertexAttribute halfAttribute( unsigned int size );
		static GlVertexAttribute floatAttribute( unsigned int size );
		static GlVertexAttribute doubleAttribute( unsigned int size );

		~GlVertexAttribute();

		unsigned int getByteSize() const;
		unsigned int getSize() const;
		unsigned int getType() const;

	private:

		GlVertexAttribute( unsigned int t_type, unsigned int t_size );
		unsigned int m_type;
		unsigned int m_size;
	};

	// a set of vertex attributes for an interlaced vertex definition

	class PANTHEON_API GlVertexDefinition : public IGlObject {

	public:

		template<typename T>
		GlVertexDefinition( T t_begin, T t_end ) : m_attributes{ t_begin, t_end } {

			findByteSize();
		}
		GlVertexDefinition( const GlVertexDefinition & );
		~GlVertexDefinition();

		void bind();
		void enable();
		void unbind() override;

	private:

		void findByteSize();

		std::vector<GlVertexAttribute> m_attributes;
		unsigned int m_byteSize;
	};

	// collection of gl objects representing a single interlaced mesh

	class PANTHEON_API GlMesh : public IGlObject {

	public:

		GlMesh( const GlVertexDefinition& t_vertexDefinition );
		GlMesh( const GlMesh& ) = delete;
		GlMesh& operator=( const GlMesh& ) = delete;
		~GlMesh();

		template<typename T, typename = std::enable_if<std::is_unsigned<T>::value, T>::type>
		void fillIndicesStatic( const std::vector<T>& t_data ) {

			static_assert( sizeof( T ) <= 4, "only unsigned char, short and int supported.");
			findIndexType( sizeof( T ) );
			fillIndicesStatic( &t_data[0], t_data.size() * sizeof( T ) );
		}
		template<typename T, typename = std::enable_if<std::is_unsigned<T>::value, T>::type>
		void fillIndicesDynamic( const std::vector<T>& t_data ) {

			static_assert( sizeof( T ) <= 4 && "only unsigned char, short and int supported. " );
			findIndexType( sizeof( T ) );
			fillIndicesDynamic( &t_data[0], t_data.size() * sizeof( T ) );
		}
		template<typename T, typename = std::enable_if<std::is_unsigned<T>::value, T>::type>
		void fillIndicesStream( const std::vector<T>& t_data ) {

			static_assert( sizeof( T ) <= 4 && "only unsigned char, short and int supported. " );
			findIndexType( sizeof( T ) );
			fillIndicesStream( &t_data[0], t_data.size() * sizeof( T ) );
		}

		void fillVerticesStatic( void* const t_dataPointer, unsigned int t_dataLength );
		void fillVerticesDynamic( void* const t_dataPointer, unsigned int t_dataLength );
		void fillVerticesStream( void* const t_dataPointer, unsigned int t_dataLength );
		
		void bind();
		void unbind() override;

	private:

		void findIndexType( unsigned int t_size );
		void fillIndicesStatic( const void* const t_dataPointer, unsigned int t_dataLength );
		void fillIndicesDynamic( const void* const t_dataPointer, unsigned int t_dataLength );
		void fillIndicesStream( const void* const t_dataPointer, unsigned int t_dataLength );

		unsigned int m_indexType;
		GlBuffer m_indexBuffer;
		GlBuffer m_vertexBuffer;
		GlVAObject m_vertexArrayObject;
		GlVertexDefinition m_vertexDefinition;
	};

	// Shader Factory Flags

	const int PANTHEON_SHADER_VERTEX{ 0 };
	const int PANTHEON_SHADER_FRAGMENT{ 1 };
	const int PANTHEON_SHADER_TYPE_MASK{ 1 };


	// Factory function returning a generated shader source file from the given flags

	std::string generateShader( const int );

	// shader compile error exception

	class PANTHEON_API ShaderCompileException : public IGlException {

	public:

		ShaderCompileException( const GlShader& t_shader );
		~ShaderCompileException();

		const char* what() const override;

	private:

		std::string m_errorString;
	};

	// gl shader object class

	class PANTHEON_API GlShader {

	public:

		static unsigned int computeShader();
		static unsigned int fragmentShader();
		static unsigned int geometryShader();
		static unsigned int tessControlShader();
		static unsigned int tessEvaluationShader();
		static unsigned int vertexShader();

		GlShader( unsigned int );
		GlShader( const GlShader& ) = delete;
		GlShader& operator=( const GlShader& ) = delete;

		~GlShader();

		void compileFromFile( const std::string& t_path );
		void compile( const std::string& t_source );

		unsigned int getId() const;
		unsigned int getType() const;

	private:

		unsigned int m_id;
		unsigned int m_type;
	};

	class PANTHEON_API GlUniform {

	public:

		GlUniform( const std::string& t_name, int t_location, unsigned int t_type, int t_size );
		~GlUniform();

		void setValue( int t_value );
		void setValue( unsigned int t_value );
		void setValue( float t_value );
		void setValue( double t_value );
		void setValue( std::vector<int> t_value );
		void setValue( std::vector<unsigned int> t_value );
		void setValue( std::vector<float> t_value );
		void setValue( std::vector<double> t_value );

		std::string getName();
		int getLocation();
		unsigned int getType();
		int getSize();
	private:

		std::string m_name;
		int m_location;
		unsigned int m_type;
		int m_size;
	};

	// program link error exception

	class PANTHEON_API ProgramLinkException : public IGlException {

	public:

		ProgramLinkException( const GlProgram& t_program );
		~ProgramLinkException();

		const char* what() const override;

	private:

		
		std::string m_errorString;
	};

	// program uniform does not exist exception

	class PANTHEON_API ProgramUniformDoesNotExistException : public IGlException {

	public:

		ProgramUniformDoesNotExistException( const std::string& t_uniformName );
		~ProgramUniformDoesNotExistException();

		const char* what() const override;

	private:

		std::string m_errorString;
	};

	// gl program object class wrapping up opengl functionality in an RAII compliant object

	class PANTHEON_API GlProgram : public IGlObject {

	public:

		GlProgram();
		GlProgram( const GlProgram& ) = delete;
		GlProgram& operator=( const GlProgram& ) = delete;
		~GlProgram();

		void attachShader( const GlShader& t_shader );
		void bindAttributeLocation( const std::string& t_name, unsigned int t_location );
		void link();

		GlUniform& getUniform( const std::string& t_name );

		void bind();
		void unbind() override;

		unsigned int getId() const;

	private:

		unsigned int m_id;
		std::map<std::string, GlUniform> m_uniforms;
		
	};

	// texture invalid data length exception

	class PANTHEON_API TextureInvalidDataLengthException : public IGlException {

	public:

		TextureInvalidDataLengthException(unsigned int t_expectedLength, unsigned int t_actualLength);
		~TextureInvalidDataLengthException();

		const char* what() const override;

		unsigned int getExpectedDataLength() const;
		unsigned int getActualDataLength() const;
	private:

		unsigned int m_expectedLength;
		unsigned int m_actualLength;
		std::string m_errorString;
	};

	// texture could not load exception

	class PANTHEON_API TextureCouldNotLoadException : public IGlException {
		
	public:

		TextureCouldNotLoadException();
		~TextureCouldNotLoadException();

		const char* what() const override;

	private:

		std::string m_errorString;
	};

	// texture not supported exception

	class PANTHEON_API TextureNotSupportedException : public IGlException {

	public:

		TextureNotSupportedException( const std::string& t_reason );
		~TextureNotSupportedException();

		const char* what() const override;

		const char* getReason() const;

	private:

		std::string m_errorString;
		std::string m_reason;
	};

	//

	class PANTHEON_API GlTexture2D : public IGlObject {

	public:

		GlTexture2D();
		GlTexture2D( const GlTexture2D& ) = delete;
		GlTexture2D& operator=(const GlTexture2D&) = delete;
		~GlTexture2D();

		void fillFromFile( const std::string& t_path );
		template<typename T, typename = std::enable_if<std::is_signed<T>::value, T>::type>
		void fillSigned( int t_level, unsigned int t_width, unsigned int t_height
			, int t_border, std::vector<T> t_data) {

			static_assert((sizeof( T ) == 1 || sizeof( T ) == 2 || sizeof( T ) == 4)
				&& "Only char, short and int types supported.");
			unsigned int format = findFormat( t_width, t_height, t_data.size() );
			unsigned int type = findSignedType( sizeof( T ) );
			fill( t_level, format, t_width, t_height, t_border, format, type, &t_data[0] );
		}
		template<typename T, typename = std::enable_if<std::is_unsigned<T>::value, T>::type>
		void fillUnsigned() {

			static_assert((sizeof( T ) == 1 || sizeof( T ) == 2 || sizeof( T ) == 4)
				&& "Only unsigned char, short and int types supported.");
			unsigned int format = findFormat( t_width, t_height, t_data.size() );
			unsigned int type = findUnsignedType( sizeof( T ) );
			fill( t_level, format, t_width, t_height, t_border, format, type, &t_data[0] );
		}
		template<typename T, typename = std::enable_if<std::is_floating_point<T>::value, T>::type>
		void fillFloating() {

			static_assert((sizeof( T ) == 4 || sizeof( T ) == 8)
				&& "Only float and double types supported.");
			unsigned int format = findFormat( t_width, t_height, t_data.size() );
			unsigned int type = findFloatingType( sizeof( T ) );
			fill( t_level, format, t_width, t_height, t_border, format, type, &t_data[0] );
		}

		void setMinNear();
		void setMinLinear();
		void setMinNearMipMapNear();
		void setMinNearMipMapLinear();
		void setMinLinearMipMapNear();
		void setMinLinearMipMapLinear();
		void setMagNear();
		void setMagLinear();
		void setMinMagNear();
		void setMinMagLinear();
		void setWrapRepeat();
		void setWrapClamp();


		void bind( unsigned int t_index );
		void unbind() override;

	private:

		static unsigned int findFormat( unsigned int t_width, unsigned int t_height, unsigned int t_dataLength );
		static unsigned int findSignedType( unsigned int t_size );
		static unsigned int findUnsignedType( unsigned int t_size );
		static unsigned int findFloatingType( unsigned int t_size );

		void fill( int t_level, unsigned int t_internalFormat
			, unsigned int t_width, unsigned int t_height
			, int t_border, unsigned int t_format, unsigned int t_type
			, const void* const t_dataPointer );

		int m_bind{ -1 };
		unsigned int m_id{ 0 };
	};
}

#endif