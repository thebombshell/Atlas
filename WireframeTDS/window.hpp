
// window_helper.hpp

// Header file for window helper namespace.

#ifndef WIREFRAMETDS_WINDOW_HELPER_HPP
#define WIREFRAMETDS_WINDOW_HELPER_HPP

#include "wireframe_tds.hpp"

namespace atlas {

	namespace window {

		const int WTDS_WINDOW_INITIALIZED{ 0 };
		const int WTDS_WINDOW_NOT_INITIALIZED{ 1 };

		int getInitializedState(
			);

		int initializeWindow(
			int* t_argc,
			char** t_argv
			);

		int finalizeWindow(
			);

		void mainLoop(
			);
	};
}

#endif