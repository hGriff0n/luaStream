#include "luaState.h"
//#include <vector>
//#include <bitset>
#include <sstream>

namespace lua {

	/*
	|	luaState definitions
	*/

	// luaState pimpl class definition
	struct luaState::impl {
		static const int NO_SIZE_CHANGE = -1;

		bool isLua;
		int exitSize,reserved;

		impl() : isLua(false),exitSize(NO_SIZE_CHANGE),reserved(0) {};
		~impl() {};

		/*static */void close(luaState& master) {
			if (exitSize != NO_SIZE_CHANGE)
				lua_settop(master,exitSize);		// Very bad procedure
			if (!isLua) lua_close(*master);
		};
	};

	// luaState constructors/destructors

	luaState::luaState() : L(luaL_newstate()),state(new luaState::impl())/*,opener(luaState::openState)*/ {
		luaState::openState(L);
	};
	luaState::luaState(lua_State* stack)
			: L(stack),state(new luaState::impl())/*,opener(luaState::openState)*/ {
		// state->exitSize = lua_gettop(stack)+num;
		state->isLua = true;
	};
	luaState::luaState(const std::function<void(lua_State*)>& openFunc)
			: L(luaL_newstate()),state(new luaState::impl())/*,opener(openFunc)*/ {
		openFunc(L);
	};
	luaState::~luaState() {
		//luaState::impl::close(*this);
		state->close(*this);
		delete state;
	};

	// luaState unwrappers

	lua_State* luaState::get() { return L; };
	lua_State* luaState::operator*(void) { return L; };
	/*std::function<void(lua_State*)> luaState::getFunc() const {
		return opener;
	};*/
	std::string luaState::dumpImpl(const char* ch) {
		std::stringstream temp;
		temp << ch << "-- isluaFunc: " << ((state->isLua) ? "true" : "false") << "\n";
		temp << ch << "-- exitSize: " << state->exitSize << "\n";
		temp << ch << "-- reserved: " << state->reserved << "\n";
		return temp.str();
	};

	// luaState code loading

	bool luaState::loadfile(std::string file) {
		return luaL_dofile(L,file.c_str());
	};
	bool luaState::loadchunk(std::string chunk) {
		return luaL_dostring(L,chunk.c_str());
	};

	bool luaState::isfreeindex(int idx) {
		return idx >= (this->state->reserved);
	};

	luaState::operator lua_State*() { return L; };

	// debug functions

	int start(luaState& L) {
		// Change later?
		return 1;
	};
	int end(luaState& L) {
		return lua_gettop(L);
	};
	std::string debugHead(luaState& L) {
		return "<---luaState debug information--->\n";
	};
	std::string debugBody(luaState& L,const char* ch) {
		return (L.dumpImpl(ch)+"\n");
	};

};