
// text_data_file.cpp

// Source file for the text data file

#include "text_data_file.hpp"

#include <SDL.h>
#include <map>
#include <regex>

using namespace pantheon;

class TextDataFile::TextDataFileImpl {

	friend class TextDataFile;

	TextDataFileImpl( std::string t_path ) {
		
		openFile( t_path );
	}
	
	~TextDataFileImpl() {

		closeFile();
	}

	void openFile( std::string t_path ) {

		SDL_assert( m_file == nullptr
			&& "A file is already open, close one before opening another." );

		// open file with append then close to be sure the file exists

		const char* cPath = t_path.c_str();
		m_file = SDL_RWFromFile( cPath, "a" );
		if ( m_file == NULL || m_file->close( m_file ) != 0 ) {

			m_file = nullptr;
		}

		// open file for read and write operations

		m_file = SDL_RWFromFile( cPath, "r+" );
		if ( m_file == NULL ) {

			m_file = nullptr;
		}

		// reads file before returning

		readFile();
	}

	void closeFile() {

		SDL_assert( m_file != nullptr
			&& "No file is open, make sure the file was opened correctly." );

		// writes file before closing

		writeFile();

		// close file, if unsuccessful log that there was a problem

		if ( m_file->close( m_file ) ) {
			
			SDL_Log( "File could not be closed properly, writes may not have occured." );
		}
		m_file = nullptr;
	}

	void writeFile() {

		auto writeString = [file = m_file]( const std::string& t_string) {

			for (auto character : t_string ) {

				SDL_WriteU8( file, character );
			}
		};
		auto findStringOutput = [&]( const std::string& t_name )->std::string {

			auto result = m_strings.find( t_name );
			if ( result != m_strings.end() ) {

				return "s" + t_name + " = " + "\"" + result->second + "\";";
			}
			return "";
		};
		auto findNumberOutput = [&]( const std::string& t_name )->std::string {

			auto result = m_numbers.find( t_name );
			if ( result != m_numbers.end() ) {

				return "n" + t_name + " = " + std::to_string(result->second) + ";";
			}
			return "";
		};
		auto findBoolOutput = [&]( const std::string& t_name )->std::string {

			auto result = m_bools.find( t_name );
			if ( result != m_bools.end() ) {

				return "n" + t_name + " = " + (result->second ? "True" : "False") + ";";
			}
			return "";
		};
		for( auto pair : m_attributes ) {

			switch ( pair.second ) {
				case 's':
					writeString( findStringOutput( pair.first ) );
					break;
				case 'n':
					writeString( findNumberOutput( pair.first ) );
					break;
				case 'b':
					writeString( findBoolOutput( pair.first ) );
					break;
			}
		}
	}

	int readAttribute( std::string& t_name, std::string& t_value ) const {

		unsigned char character{ 0 };

		// read until end of file or an equals is found appending alphanumeric characters to name

		while ( (character = SDL_ReadU8( m_file )) != 0 && character != '=' ) {

			if ( isalnum( character ) ) {

				t_name += character;
			}
		}

		// read until end of file or a semicolon is found appending characters to value

		while ( (character = SDL_ReadU8( m_file )) != 0 && character != ';' ) {

			if ( !isspace( character ) ) {

				t_value += character;
			}
		}

		if ( character == 0 ) {

			return PANTHEON_ERROR;
		}
		return PANTHEON_OK;
	}

	void sortNumber( const std::string& t_name, const std::string& t_value ) {

		// parse double

		char* end{ nullptr };
		double value = std::strtod( t_value.c_str(), &end );

		// if something was parsed assume good value and insert into numbers

		if ( end != t_value.c_str() ) {

			m_numbers.insert( { t_name, value } );
		}
	};

	void sortBool( const std::string& t_name, const std::string& t_value ) {

		if ( t_value == "True" ) {

			m_bools.insert( { t_name, true } );
		}
		else if ( t_value == "False" ) {

			m_bools.insert( { t_name, false } );
		}
	};

	void sortAttribute( std::string t_name, std::string t_value ) {

		// find name by removing identifier character and files away

		std::string finalName{ t_name.begin() + 1, t_name.end() };
		m_attributes.insert( { finalName, t_name[0] } );

		// find type from identifier character

		switch ( t_name[0] ) {
			case 's':
				m_strings.insert( { finalName, t_value } );
				break;
			case 'n':
				sortNumber( finalName, t_value );
				break;
			case 'b':
				sortBool( finalName, t_value );
				break;
		}

	}

	void readFile() {

		SDL_assert( m_file != nullptr
			&& "No file is open, make sure the file was opened correctly." );

		m_file->seek( m_file, 0, RW_SEEK_SET );

		std::string name;
		std::string value;

		while ( readAttribute( name, value ) != PANTHEON_ERROR );
	}

	int getAttributeType( const std::string& t_name ) const {

		auto attribute = m_attributes.find( t_name );
		if ( attribute == m_attributes.end() ) {

			return PANTHEON_DATAFILE_NOT_FOUND;
		}
		return attribute->second;
	}

	std::string getStringAttribute( const std::string& t_name ) {

		auto attribute = m_strings.find( t_name );
		SDL_assert( attribute != m_strings.end() && "Attribute does not exist" );
		return attribute->second;
	}

	double getNumericAttribute( const std::string& t_name ) {

		auto attribute = m_numbers.find( t_name );
		SDL_assert( attribute != m_numbers.end() && "Attribute does not exist" );
		return attribute->second;
	}

	bool getBoolAttribute( const std::string& t_name ) {

		auto attribute = m_bools.find( t_name );
		SDL_assert( attribute != m_bools.end() && "Attribute does not exist" );
		return attribute->second;
	}

	void setAttribute( const std::string& t_name, const std::string& t_value ) {

		m_attributes[t_name] = 's';
		m_strings[t_name] = t_value;
	}

	void setAttribute( const std::string& t_name, double t_value ) {

		m_attributes[t_name] = 'n';
		m_numbers[t_name] = t_value;
	}

	void setAttribute( const std::string& t_name, bool t_value ) {

		m_attributes[t_name] = 'b';
		m_bools[t_name] = t_value;
	}

	std::map<std::string, char> m_attributes{ };
	std::map<std::string, std::string> m_strings{ };
	std::map<std::string, double> m_numbers{ };
	std::map<std::string, bool> m_bools{ };
	SDL_RWops* m_file{ nullptr };
};

TextDataFile::TextDataFile( const std::string& t_path ) {

	m_dataFile = new TextDataFileImpl( t_path );
}

TextDataFile::~TextDataFile() {

	delete m_dataFile;
}

int TextDataFile::isValid() {

	return m_dataFile->m_file != nullptr;
}

int TextDataFile::getAttributeType( const std::string& t_name ) const {

	return m_dataFile->getAttributeType( t_name );
}

std::string TextDataFile::getStringAttribute( const std::string& t_name ) const {

	return m_dataFile->getStringAttribute( t_name );
}

double TextDataFile::getNumericAttribute( const std::string& t_name ) const {

	return m_dataFile->getNumericAttribute( t_name );
}

bool TextDataFile::getBoolAttribute( const std::string& t_name ) const {

	return m_dataFile->getBoolAttribute( t_name );
}

void TextDataFile::setAttribute( const std::string& t_name, const std::string& t_value ) {

	m_dataFile->setAttribute( t_name, t_value );
}

void TextDataFile::setAttribute( const std::string& t_name, double t_value ) {

	m_dataFile->setAttribute( t_name, t_value );
}

void TextDataFile::setAttribute( const std::string& t_name, bool t_value ) {

	m_dataFile->setAttribute( t_name, t_value );
}