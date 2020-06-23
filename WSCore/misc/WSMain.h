﻿#pragma once

#include "../misc/WSPch.h"

namespace WS {

	/*
	 * // ////////////////////////////////--\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
	 * // |/‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\| \\
	 * // ||--------------------------Entry Point--------------------------|| \\
	 * // |\_______________________________________________________________/| \\
	 * // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\--///////////////////////////////// \\
	 */

	// To Be Declared By The User
	int EntryPoint(int argc, char** argv);

}; // WS

// Entry Point Defined By Weiss That Calls WS::EntryPoint
int main(int argc, char** argv);