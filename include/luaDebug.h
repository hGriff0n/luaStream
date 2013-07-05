#pragma once

// Need way to print to log files
#include <iostream>
#include "lua.hpp"
#include "luaState.h"
#include "luaStream.h"

namespace lua {

	void printIndex(std::ostream&,lua_State*,int);

	void debugStack(std::ostream&,lua_State*);
	void debugState(std::ostream&,luaState&);
	void debugStream(std::ostream&,luastream&);



};