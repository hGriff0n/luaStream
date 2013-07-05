#include "luaState.h"
//#include <vector>
//#include <bitset>

namespace lua {

	/*
	|	luaState definitions
	*/

	// luaState pimpl class definition
	struct luaState::impl {
		bool isLua;
		int exitSize;

		impl() : isLua(false),exitSize(-1) {};
		~impl() {};

		void close(luaState& master) {
			if (exitSize > -1 && exitSize != lua_gettop(*master))
				lua_settop(*master,exitSize);		// Very bad procedure. Also print removals to log
			if (!isLua) lua_close(*master);
		};
	};

	// luaState constructors/destructors

	luaState::luaState() : L(luaL_newstate()),state(new luaState::impl())/*,opener(luaState::openState)*/ {
		luaState::openState(L);
	};
	luaState::luaState(lua_State* stack) : L(stack),state(new luaState::impl())/*,opener(luaState::openState)*/ {
		// state->exitSize = lua_gettop(stack)+num;
		state->isLua = true;
	};
	luaState::luaState(const std::function<void(lua_State*)>& openFunc)
			: L(luaL_newstate()),state(new luaState::impl())/*,opener(openFunc)*/ {
		openFunc(L);
	};
	/*luaState::luaState(const luaState&); //: L(lua_newthread(*copy))/*,opener(copy.getFunc()) {
		opener(L);

		Way to set luaState so that it can't delete the new thread while thisState is running
		Various setters to avoid runins
	}; Copy values on stack and use same opening function. Define elsewhere*/
	luaState::~luaState() {
		state->close(*this);
		delete state;
		L = nullptr;
	};

	// luaState unwrappers

	lua_State* luaState::get() { return L; };
	lua_State* luaState::operator*(void) { return L; };
	/*std::function<void(lua_State*)> luaState::getFunc() const {
		return opener;
	};*/

	// luaState code loading

	bool luaState::loadfile(std::string file) {
		return luaL_dofile(L,file.c_str());
	};
	bool luaState::loadchunk(std::string chunk) {
		return luaL_dostring(L,chunk.c_str());
	};

};