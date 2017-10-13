
// text_data_file.hpp

// header file for the public implementation of the text data file

#ifndef PANTHEON_TEXT_DATA_FILE_HPP
#define PANTHEON_TEXT_DATA_FILE_HPP

#include "pantheon.hpp"

#include <string>

namespace pantheon {

	// data file attribute types
	// not found indicates the attribute does not exist
	// invalid indicates the attribute was not a recognised attribute type

	const char PANTHEON_DATAFILE_NOT_FOUND{ 0 };
	const char PANTHEON_DATAFILE_STRING{ 's' };
	const char PANTHEON_DATAFILE_NUMERIC{ 'n' };
	const char PANTHEON_DATAFILE_VECTOR{ 'v' };
	const char PANTHEON_DATAFILE_BOOL{ 'b' };
	
	// Class for reading and writing essential human readable data files
	// Should not be used to store large amounts of data, just options and maybe save files.

	class PANTHEON_API TextDataFile {

	private:

		class TextDataFileImpl;
		TextDataFileImpl* m_dataFile{ nullptr };
	public:

		// opens and closes file

		TextDataFile( const std::string& t_path );
		~TextDataFile();

		// checks if the file opened correctly

		int isValid();

		// check if attribute exists and return its type

		int getAttributeType( const std::string& t_name ) const;

		// get one of the 4 types of attribute

		std::string getStringAttribute( const std::string& t_name ) const;
		double getNumericAttribute( const std::string& t_name ) const;
		bool getBoolAttribute( const std::string& t_name ) const;

		// set attribute, creating it if it does not already exist

		void setAttribute( const std::string& t_name, const std::string& t_value );
		void setAttribute( const std::string& t_name, double t_value );
		void setAttribute( const std::string& t_name, bool t_value );

	};
}

#endif