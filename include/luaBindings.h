/*--------------------------------------------------------------------------------------->
||	Maker: Grayson Hooper
||	Credits: 
||
||	note: exported under the namespace lua::
||	note: 'string' = 'std::string'
||
||	functions:
||		luaRegEx		return - string		given - lua_State*,string,char,int		use - luaRegEx(state,<var>,<seperator>,<lookup>)
||		toPosIndex		return - int		given - lua_State*,int					use - toPosIndex(state,<index>)
||		toNegIndex		return - int		given - lua_State*,int					use - toNegIndex(state,<index>)
||		addFile			return - void		given - lua_State*,string&				use - addFile(state,<filepath>)
||		getVar			return - void		given - lua_State*,string				use - getVar(state,<name>)
||		setVar			return - void		given - lua_State*,string,template<T>	use - setVar(state,<name>,<value>)
||		pushObj			return - void		given - lua_State*,template<T>			use - pushObj(state,<object>)
||		setcvar			return - void		given - lua_State*,template<T>&			use - setcvar(state,<cvar>&)
||		start			return - int		given - lua_State*						use - start(state)
||		end				return - int		given - lua_State*						use - end(state)
||		debugHead		return - string		given - lua_State*						use - debugHead(state)
||		debugBody		return - string		given - lua_State*						use - debugBody(state)
||	defines:
||		luaL_newVar		wrapper of setVar
||		luaL_newFunc	wrapper of lua_register
||
||	class: error
||		constructor:
||			error msg: given - string,string	use - error(<function>,<error>)
||		interface:
||			what		return string		given - void		use - what()
||
||	TODO (?):
||		remove "luaClass" dependency
||		setVar that works using stack indexes
||		require lua be built as c++
\*---------------------------------------------------------------------------------------->*/

#pragma once

// macros
#define luaL_newvar(L,name) lua::setVar(L,name,0)
#define luaL_newfunc(L,name,func) lua_register(L,name,func)

// Includes
#include "lua.hpp"
#include "luaClass.h"			// Remove this ???		-- Would make setc_var and pushObj non-templated. End user
#include <string>				//						-- would define these for custom objects (like in luastream)

// Convenience Typedefs
typedef lua_CFunction luaFunc;

namespace lua {

	// Error class forward declaration
	class error;

	// Helper functions

	// Splits a lua var name based on passed char to enable tabled lookup of lua vars. Pushes the host table onto the stack if var isn't global
	std::string luaSplice(lua_State*,std::string,char='.',int=LUA_GLOBALSINDEX);
	// Turns given index into a pos/neg index based on the passed lua_State
	int toPosIndex(lua_State*,int);
	int toNegIndex(lua_State*,int);
	// Note: returns passed index if index is invalid. Problematic
	// Throw lua::error or return a crazy number

	/* 
	|	luaBindings Interface
	*/

	// State interaction
	
	// Loads the file given by the passed string into the passed lua_State
	void addFile(lua_State*,std::string&);
 
	// Variable interactions

	// Pushes the lua var of the given name onto the stack of the passed lua_State
	void getVar(lua_State*,std::string);
	// Sets the lua var of the given name in the passed lua_State with the passed value
	template <typename T>
	void setVar(lua_State* L,std::string name,T value) {
		std::string field = luaRegEx(L,name);
		lua_pushstring(L,field.c_str());
		pushObj(L,value);
		if (field == name)
			lua_setglobal(L,field.c_str());
		else
			lua_settable(L,-3);
		lua_remove(L,-1);
	};

	// Change name to push ???

	// Pushes the passed C object onto the stack of the passed lua_State
	template <typename T>
	void pushObj(lua_State* L,T* data) {
		// Needs work here. Not sure how Lunar::push signifies an unsupported error (if it does)
		if (!Lunar<T>::push(L,data)) throw lua::error("addArg(T)","Unsupported data type");
	};
	void pushObj(lua_State*,double);
	void pushObj(lua_State*,const char*);
	void pushObj(lua_State*,std::string);
	void pushObj(lua_State*,bool);
	void pushObj(lua_State*,int);

	// Sets the passed c var to the value of the object at the top of the stack of the passed lua_State (pops the object)
	template <typename T>
	void setcvar(lua_State* L, T& value) {
		/*		Would this work ???
		value = Lunar<T>::check(L,-1);
		if(value == nullptr) throw lua::error("setc_var(T)","Unsupported data type");	*/

		T* temp = Lunar<T>::check(L,-1);
		if (temp == nullptr) throw lua::error("setc_var(T)","Unsupported data type");
		data = *temp;	temp = nullptr;		// Needs work with pointers & references
	};
	void setcvar(lua_State*,int&);
	void setcvar(lua_State*,double&);
	void setcvar(lua_State*,std::string&);
	void setcvar(lua_State*,bool&);

	// error class definition
	class error {
		private:
			std::string funcName;
			std::string errorMsg;
		public:
			error(std::string,std::string);		// Constructor: initializes the class with the passed variables
			~error();

			std::string what();					// Returns a formatted string based on the string members
	};

	// debug functions declaration (see luaDebug.h for explanations)
	
	std::string debugHead(lua_State*);
	std::string debugBody(lua_State*,const char* = "\n");
	int start(lua_State*);
	int end(lua_State*);
}