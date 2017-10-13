
// push_buffer.hpp

// header file for the push buffer template

#ifndef PANTHEON_PUSH_BUFFER_HPP
#define PANTHEON_PUSH_BUFFER_HPP

namespace pantheon {

	template<typename T>
	class PushBuffer {
	private:

		int m_index{ 0 };
	public:

		PushBuffer( int t_size )
			: size{ t_size }, pointer{ new T[t_size] }, m_index{ 0 } {

			assert( size > 0 );
		}

		PushBuffer( const PushBuffer& t_other )
			: size{ t_other.size }, pointer{ new T[t_other.size] }
			, m_index{ t_other.m_index } {

			std::copy( t_other.pointer, t_other.pointer + t_other.m_index
				, pointer );
		}

		PushBuffer& operator=( const PushBuffer& t_other ) = delete;

		~PushBuffer() {

			delete[] pointer;
		}

		void pushData( const T* const t_begin, const T* const t_end ) {

			int distance = t_end - t_begin;
			assert( distance > 0 && m_index + distance <= size
				&& "Data out of range and could not be added to buffer." );

			std::copy( t_begin, t_end, pointer + m_index );
			m_index += (t_end - t_begin);
		};

		int getIndex() {

			return m_index;
		}

		T* const pointer{ nullptr };
		const int size{ 0 };
	};
}

#endif