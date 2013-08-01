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

	// debug functions

	int start(luaState& L) {
		// Change later?
		return 1;
	};
	int end(luaState& L) {
		return lua_gettop(*L);
	};
	std::string debugHead(luaState& L) {
		return "<---luaState debug information--->\n";
	};
	std::string debugBody(luaState& L,const char* ch) {
		return (L.dumpImpl(ch)+"\n");
	};
	lua_State* toluaCore(luaState& L) {
		return *L;
	};
};

void lua_call(lua::luaState& L,int nargs,int nresults) {
	return lua_call(*L,nargs,nresults);
};
int lua_checkstack(lua::luaState& L,int extra) {
	return lua_checkstack(*L,extra);
};
void lua_concat(lua::luaState& L,int n) {
	return lua_concat(*L,n);
};
int lua_cpcall(lua::luaState& L,lua_CFunction func,void* ud) {
	return lua_cpcall(*L,func,ud);
};
void lua_createtable(lua::luaState& L,int narr,int nrec) {
	return lua_createtable(*L,narr,nrec);
};
int lua_dump(lua::luaState& L,lua_Writer writer,void* data) {
	return lua_dump(*L,writer,data);
};
int lua_equal(lua::luaState& L,int index1,int index2) {
	return lua_equal(*L,index1,index2);
};
int lua_error(lua::luaState& L) {
	return lua_error(*L)
};
int lua_gc(lua::luaState& L,int what,int data) {
	return lua_gc(*L,what,data);
};
void lua_getfenv(lua::luaState& L,int index) {
	return lua_getfenv(*L,index);
};
void lua_getfield(lua::luaState& L,int index,const char* k) {
	return lua_getfield(*L,index,k);
};
int lua_getmetatable(lua::luaState& L,int index) {
	return lua_getmetatable(*L,index);
};
void lua_gettable(lua::luaState& L,int index) {
	return lua_gettable(*L,index);
};
int lua_gettop(lua::luaState& L) {
	return lua_gettop(*L);
};
void lua_insert(lua::luaState& L,int index) {
	return lua_insert(*L,index);
};
int lua_iscfunction(lua::luaState& L,int index) {
	return lua_iscfunction(*L,index);
};
int lua_isuserdata(lua::luaState& L,int index) {
	return lua_isuserdata(*L,index);
};
int lua_isstring(lua::luaState& L,int index) {
	return lua_isstring(*L,index);
};
int lua_lessthan(lua::luaState& L,int index1,int index2) {
	return lua_lessthan(*L,index1,index2);
};
lua_State* lua_newthread(lua::luaState& L) {
	return lua_newthread(*L);
};
int lua_next(lua::luaState& L,int index) {
	return lua_next(*L,index);
};
size_t lua_objlen(lua::luaState& L,int index)  {
	return lua_objlen(*L,index);
};
int lua_pcall(lua::luaState& L,int nargs,int nresult,int errfunc)  {
	return lua_pcall(*L,nargs,nresult,errfunc);
};
void lua_pushboolean(lua::luaState& L,int b) {
	return lua_pushboolean(*L,b)
};
void lua_pushcclosure(lua::luaState& L,lua_CFunction fn,int n) {
	return lua_pushcclosure(*L,fn,n);
};
void lua_pushinteger(lua::luaState& L,lua_Integer n) {
	return lua_pushinteger(*L,n);
};
void lua_pushlightuserdata(lua::luaState& L,void* ud) {
	return lua_pushlightuserdata(*L,ud);
};
void lua_pushlstring(lua::luaState& L,const char* s,size_t len) {
	return lua_pushlstring(*L,s,len);
};
void lua_pushnil(lua::luaState& L) {
	return lua_pushnil(*L);
};
void lua_pushnumber(lua::luaState& L,lua_Number n) {
	return lua_pushnumber(*L,n);
};
void lua_pushstring(lua::luaState& L,const char* s) {
	return lua_pushstring(*L,s);
};
int lua_pushthread(lua::luaState& L) {
	return lua_pushthread(*L);
};
void lua_pushvalue(lua::luaState& L,int index) {
	return lua_pushvalue(*L,index);
};
int lua_rawequal(lua::luaState& L,int index1,int index2) {
	return lua_rawequal(*L,index1,index2);
};
void lua_rawget(lua::luaState& L,int index) {
	return lua_rawget(*L,index);
};
void lua_rawgeti(lua::luaState& L,int index,int n) {
	return lua_rawgeti(*L,index,n);
};
void lua_rawset(lua::luaState& L,int index) {
	return lua_rawset(*L,index);
};
void lua_rawseti(lua::luaState& L,int index,int n) {
	return lua_rawseti(*L,index,n);
};
void lua_remove(lua::luaState& L,int index) {
	return lua_remove(*L,index);
};
void lua_replace(lua::luaState& L,int index) {
	return lua_replace(*L,index);
};
int lua_resume(lua::luaState& L,int narg) {
	return lua_resume(*L,narg);
};
int lua_setfenv(lua::luaState& L,int index) {
	return lua_setfenv(*L,index);
};
void lua_setfield(lua::luaState& L,int index,const char* k) {
	return lua_setfield(*L,index,k);
};
int lua_setmetatable(lua::luaState& L,int index) {
	return lua_setmetatable(*L,index);
};
void lua_settable(lua::luaState& L,int index) {
	return lua_settable(*L,index);
};
void lua_settop(lua::luaState& L,int index) {
	return lua_settop(*L,index);
};
int lua_status(lua::luaState& L) {
	return lua_status(*L);
};
int lua_toboolean(lua::luaState& L,int index) {
	return lua_toboolean(*L,index);
};
lua_CFunction lua_tocfunction(lua::luaState& L,int index) {
	return lua_tocfunction(*L,index);
};
lua_Integer lua_tointeger(lua::luaState& L,int index) {
	return lua_tointeger(*L,index);
};
const char* lua_tolstring(lua::luaState& L,int index,size_t* len) {
	return lua_tolstring(*L,index,len);
};
lua_Number lua_tonumber(lua::luaState& L,int index) {
	return lua_tonumber(*L,index);
};
lua_State* lua_tothread(lua::luaState& L,int index) {
	return lua_tothread(*L,index);
};
int lua_type(lua::luaState& L,int index) {
	return lua_type(*L,index);
};
const char* lua_typename(lua::luaState& L,int tp) {
	return lua_typename(*L,tp);
};
void lua_xmove(lua::luaState& from,lua::luaState& to,int n) {
	return lua_xmove(*from,*to,n);
};