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
||	defines:
||		luaL_newVar		wrapper of setVar
||		luaL_newFunc	wrapper of lua_register
||
||	class: error
||		intent: lua error class. Type thrown by all luaBinding functions
||		constructor:
||			default: given - string,string	use - error(<function>,<error>)
||		interface:
||			what		return string		given - void		use - what()
||
||	TODO:
||		remove <string> dependency
||		remove "luaClass" dependency	???
||		setVar that works using stack indexes
||		require lua be built as c++
\*---------------------------------------------------------------------------------------->*/

#pragma once

#define luaL_newvar(L,name) lua::setVar(L,name,0)
#define luaL_newfunc(L,name,func) lua_register(L,name,func)

// Includes
#include "lua.hpp"
#include "luaClass.h"
#include <string>

// Convenience Typedef
typedef lua_CFunction luaFunc;

namespace lua {

	// Helper functions
	std::string luaRegEx(lua_State*,std::string,char='.',int=LUA_GLOBALSINDEX);
	int toPosIndex(lua_State*,int);
	int toNegIndex(lua_State*,int);

	// Error class forward declaration
	class error;

	/* 
	|	luaBindings Interface
	*/

	// State interaction
	void addFile(lua_State*,std::string&);
 
	// Variable interactions
	void getVar(lua_State*,std::string);
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
			error(std::string,std::string);			// Could just take const char* 's instead
			~error();

			std::string what();
	};
}