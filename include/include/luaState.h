#pragma once

#include "lua.hpp"
#include <functional>
#include <string>
//#include "luaBindings.h"		luaStream.h doesn't need luaBindings.h
							//	could put luaState member in the impl struct

namespace lua {

	/*--------------------------------------------------------------------------------------->
	||	note: default argument values are not given here (see class declaration)
	||	note: function explanations are not given here (see class declaration)
	||	note: 'string' = 'std::string'
	||
	||	class: lua::luaState
	||	intent: Provide a c++ class wrapper around lua_State*
	||	constructors:
	||		default:	given - void
	||		copy:		given - luaState			-- currently private
	||		function:	given - const function<void(lua_State*)>&
	||		luaFunc:	given - lua_State*
	||	interface:
	||		operator*:	return - lua_State*		given - void		use - *<state>
	||		get:		return - lua_State*		given - void		use - get()
	||		loadfile	return - bool			given - string		use - loadfile(<path>)
	||		loadchunk	return - bool			given - string		use - loadchunk(<code>)
	||		openState	return - void			given - lua_State*	use - luaState::openState(<lua_State*>)
	||
	||	TODO:
	||		Enable storage of opening function (impl ?)
	||		Keep a list of opened files (impl feature ?)
	||		ranged for support ???
	\*---------------------------------------------------------------------------------------->*/

	class luaState {
		private:
			struct impl;

			lua_State* L;
			impl* state;
			//std::function<void(lua_State*)> openFunc;				// this line fails

			luaState(const luaState&);								//	copy constructor
			luaState& operator=(const luaState&);
		public:
			// constructors/destructors
			luaState();												//	default
			luaState(lua_State*);									//	luaFunc		Enables luaState use in functions registered with lua
			luaState(const std::function<void(lua_State*)>&);		//	functio		Provides custom function for luaState opening
			~luaState();											//				--overrides luaState::openState in constructor

			// getters
			lua_State* get();										//	Returns the internal lua_State*
			lua_State* operator*(void);
			//std::function<void(lua_State*)> getFunc() const;		//	Returns the opening function
			std::string dumpImpl(const char*);						//	Dumps the internal state as a string

			// code loading
			bool loadfile(std::string);								//	loads a lua script into the lua_State*
			bool loadchunk(std::string);							//	runs a chunk of code in the lua_State*

			bool isfreeindex(int);
			static void openState(lua_State*);						//	default opening function for lua_State*
																	//	undefined in luaState.cpp  Must be defined to use default luaState
	};

	// debug overloads for luaState. Same usage as shown in luaBindings.h
	int start(luaState&);
	int end(luaState&);
	std::string debugHead(luaState&);
	std::string debugBody(luaState&,const char* = "-");
	lua_State* toluaCore(luaState&);
};

// luaState overrides for lua functions (forwards to the appropriate lua function)
// except lua_getallocf,lua_load,lua_setallocf,lua_topointer

void lua_call(lua::luaState&,int,int);
int lua_checkstack(lua::luaState&,int);
void lua_concat(lua::luaState&,int);
int lua_cpcall(lua::luaState&,lua_CFunction,void*);
void lua_createtable(lua::luaState&,int,int);				// lua_newtable is a macro and is defined by this line
int lua_dump(lua::luaState&,lua_Writer,void*);
int lua_equal(lua::luaState&,int,int);
int lua_error(lua::luaState&);
int lua_gc(lua::luaState&,int,int);
void lua_getfenv(lua::luaState&,int);
void lua_getfield(lua::luaState&,int,const char*);			// lua_getglobal is a macro and is defined by this line
int lua_getmetatable(lua::luaState&,int);
void lua_gettable(lua::luaState&,int);
int lua_gettop(lua::luaState&);
void lua_insert(lua::luaState&,int);
int lua_iscfunction(lua::luaState&,int);
int lua_isuserdata(lua::luaState&,int);
int lua_isstring(lua::luaState&,int);
int lua_lessthan(lua::luaState&,int,int);
lua_State* lua_newthread(lua::luaState&);
//void lua_newuserdata(lua::luaState&,size_t);
int lua_next(lua::luaState&,int);
size_t lua_objlen(lua::luaState&,int);
int lua_pcall(lua::luaState&,int,int,int);
void lua_pushboolean(lua::luaState&,int);
void lua_pushcclosure(lua::luaState&,lua_CFunction,int);	// lua_pushcfunction/lua_register are macros and are defined by this line
//const char* lua_pushfstring(lua::luaState&,const char*,...);
void lua_pushinteger(lua::luaState&,lua_Integer);
void lua_pushlightuserdata(lua::luaState&,void*);
void lua_pushlstring(lua::luaState&,const char*,size_t);	// lua_pushliteral is a macro and is defined by this line
void lua_pushnil(lua::luaState&);
void lua_pushnumber(lua::luaState&,lua_Number);
void lua_pushstring(lua::luaState&,const char*);
int lua_pushthread(lua::luaState&);
void lua_pushvalue(lua::luaState&,int);
//const char* lua_pushvfstring(lua::luaState&,const char*,va_list);
int lua_rawequal(lua::luaState&,int,int);
void lua_rawget(lua::luaState&,int);
void lua_rawgeti(lua::luaState&,int,int);
void lua_rawset(lua::luaState&,int);
void lua_rawseti(lua::luaState&,int,int);
void lua_remove(lua::luaState&,int);
void lua_replace(lua::luaState&,int);
int lua_resume(lua::luaState&,int);
int lua_setfenv(lua::luaState&,int);
void lua_setfield(lua::luaState&,int,const char*);
int lua_setmetatable(lua::luaState&,int);
void lua_settable(lua::luaState&,int);
void lua_settop(lua::luaState&,int);
int lua_status(lua::luaState&);
int lua_toboolean(lua::luaState&,int);
lua_CFunction lua_tocfunction(lua::luaState&,int);
lua_Integer lua_tointeger(lua::luaState&,int);
const char* lua_tolstring(lua::luaState&,int,size_t*);		// lua_tostring is a macro and is defined by this line
lua_Number lua_tonumber(lua::luaState&,int);
lua_State* lua_tothread(lua::luaState&,int);
//void* lua_touserdata(lua::luaState&,int);
int lua_type(lua::luaState&,int);							// lua_isX functions are mainly macros and are defined by this line
const char* lua_typename(lua::luaState&,int);
void lua_xmove(lua::luaState&,lua::luaState&,int);
//lua_yield