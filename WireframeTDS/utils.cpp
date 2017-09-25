
// utils.cpp

// Source file of miscelaneous helper functions.

#include "utils.hpp"
#include <fstream>
#include "error.hpp"

#ifdef _WIN32
#include <Windows.h>
#include <cstdlib>

void wireframe_tds::showMessage(
	const std::string t_message
	) {

	const char* cMessage = t_message.c_str();
	wchar_t* wMessage = new wchar_t[t_message.length()];
	std::mbstowcs( wMessage, cMessage, strlen(cMessage) * sizeof(wchar_t) );
	MessageBox( NULL, wMessage, L"WireframeTDS has encountered a problem.", MB_OK );
}

#else

#include <cstdio>

void WIREFRAME_TDS::showMessage(
	const std::string t_message
	) {

	std::cout << t_message;
}

#endif

const int wireframe_tds::getFileAsString(
	std::string t_path,
	std::string& t_outContents
	) {

	std::ifstream file;
	file.open(t_path);
	if ( file.is_open() ) {
		std::string contents(
			(std::istreambuf_iterator<char>( file )),
			(std::istreambuf_iterator<char>( )));
		t_outContents = contents;
	}
	else {
		return WTDS_ERROR_UTLS_COULD_NOT_OPEN_FILE;
	}

	return WTDS_ERROR_OK;
}