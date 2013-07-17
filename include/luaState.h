#pragma once

#include "lua.hpp"
#include <functional>
#include <string>

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
	||		Enable copy constructor ("clonable")
	||		Enable storage of opening function (impl ?)
	||		Keep a list of opened files (impl feature ?)
	||		ranged for support ???
	\*---------------------------------------------------------------------------------------->*/

	class luaState {
		private:
			lua_State* L;
			
			struct impl;
			impl* state;
			//std::function<void(lua_State*)> openFunc;				// this line fails

			luaState(const luaState&);								//	copy constructor
			luaState& operator=(const luaState&);
		public:
			// constructors/destructors
			luaState();												//	default
			luaState(lua_State*);									//	luaFunc		Enables luaState use in functions registered with lua
			luaState(const std::function<void(lua_State*)>&);		//	functio		Provides custom function for luaState opening
																	//				--overrides luaState::openState in constructor
			~luaState();

			// unwrappers
			lua_State* get();										//	Returns the internal lua_State*
			lua_State* operator*(void);								//	Way to use luaState with lua functions (require lua_State*)
			//std::function<void(lua_State*)> getFunc() const;		//	Returns the opening function
			std::string dumpImpl();									//	Dumps the internal state as a string

			// code loading
			bool loadfile(std::string);								//	loads a lua script into the lua_State*
			bool loadchunk(std::string);							//	runs a chunk of code in the lua_State*

			static void openState(lua_State*);						//	default opening function for lua_State*
																	//	undefined in luaState.cpp  Must be defined to use default luaState
	};

	// debug overloads for luaState. Same usage as shown in luaBindings.h
	int start(luaState&);
	int end(luaState&);
	std::string debugHead(luaState&);
	std::string debugBody(luaState&);
	lua_State* toluaCore(luaState&);
};