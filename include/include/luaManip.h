#pragma once

#include "Utils.h"
#include "luaStream.h"
#include <functional>

namespace lua {

	// luastream manipulator classes (distinquished by number of arguments)
	// Uses std::function to store manipulator. Supports using lambdas in constructor
	// Manipulator Arguments must be given to the class at construction time
	
	// No Arguments	(lua null)
	class lnManip : public baseManip<luastream> {
		private:
			std::function<luastream&(luastream&)> _func;

		public:
			lnManip(const std::function<luastream&(luastream&)>& func) : _func(func) {};

			luastream& manipulate(luastream& str) {
				return _func(str);
			};
	};

	// One Argument	(default class)
	template <typename T>
	class lManip : public baseManip<luastream> {
		private:
			T _val;
			std::function<luastream&(luastream&,T)> _func;

		public:
			lManip(T val,const std::function<luastream&(luastream&,T)>& func) : _val(val),_func(func) {};

			luastream& manipulate(luastream& str) {
				return _func(str,_val);
			};
	};

	// Two Arguments (lua dual. Type of second argument is assumed to be the same as the first)
	template <typename T, typename E=T>
	class ldManip : public baseManip<luastream> {
		private:
			T _valT;	E _valE;
			std::function<luastream&(luastream&,T,E)> _func;

		public:
			ldManip(T valT,E valE,const std::function<luastream&(luastream&,T,E)>& func) : _valT(valT),_valE(valE),_func(func) {};

			luastream& manipulate(luastream& str) {
				return _func(str,_valT,_valE);
			};
	};

};
	// Provided luastream manipulators
	// Given as functions that return the manipulators
	// Lambdas are almost solely used in the constructors

	// lua stack manipulation/organization

	// Moves the object at the given index to the top of the lua stack
	inline lua::lManip<int> luaM_totop(int x) {
		return lua::lManip<int>(x,[](lua::luastream& str,int x) -> lua::luastream& {
			lua::luastream::totop(str,x);
			return str;
		});
	};
	// Inserts the object at the top of the lua stack at the given index
	inline lua::lManip<int> luaM_insert(int x) {
		return lua::lManip<int>(x,[](lua::luastream& str,int x) -> lua::luastream& {
			if (!str.valid(x)) throw lua::error("lManip<int> insert","Invalid lua_State index");
			lua_insert(str,x);
			return str;
		});
	};
	// Replaces the object at the given index with the object at the top of the lua stack
	inline lua::lManip<int> luaM_replace(int x) {
		return lua::lManip<int>(x,[](lua::luastream& str,int x) -> lua::luastream& {
			if (!str.valid(x)) throw lua::error("lManip<int> replace","Invalid lua_State index");
			lua_replace(str,x);
			return str;
		});
	};
	// Reverses the order of the lua stack from index x to y (no arguments reverses the whole stack)
	inline lua::ldManip<int> luaM_reverse(int x=1,int y=-1) {
		return lua::ldManip<int>(x,y,[](lua::luastream& str,int x,int y) -> lua::luastream& {
			x = lua::toPosIndex(*str,x);	y = lua::toPosIndex(*str,y);
			if (!str.valid(x) || !str.valid(y)) throw lua::error("ldManip<int> reverse","Invalid lua_State index");
			for(int i=0;i!=abs(y-x);++i) {			// loops z times (z being the range of the passed block)
				lua::luastream::totop(str,x);			// moves the object at the bottom of the block to the top of the stack
				lua_insert(str,(y-i));				// inserts the moved object to the top of the block (minus the number of loops)
													//	-- object that was at index i will be inserted at index x because y-x = i
			};
			return str;
		});
	};
	// Copies the item at index x and inserts it at index y (default- stack top)
	inline lua::ldManip<int> luaM_copy(int x,int y=-1) {
		return lua::ldManip<int>(x,y,[] (lua::luastream& str,int x,int y) -> lua::luastream& {
			if (!str.valid(y,1)) throw lua::error("ldManip<int> copy","Invalid lua_State index");
			lua_pushvalue(str,x);
			lua_insert(str,y);
			return str;
		});
	};
	// Deletes the item at the given index
	inline lua::lManip<int> luaM_remove(int x) {
		return lua::lManip<int>(x,[](lua::luastream& str,int x) -> lua::luastream& {
			if (!str.valid(x)) throw lua::error("lManip<int> remove","Invalid lua_State index");
			lua_remove(str,x);
			return str;
		});
	};

	// table manipulations
	
	// Condenses the stack from index x to index y into a lua table (no arguments condenses the whole stack)
	inline lua::ldManip<int> luaM_pack(int x=1,int y=-1) {
		return lua::ldManip<int>(x,y,[](lua::luastream& str, int x,int y) -> lua::luastream& {
			x = lua::toPosIndex(*str,x); y = lua::toPosIndex(*str,y);
			int size = abs(y-x);			// y, x2, x1, z

			lua_createtable(*str,size,0);	// table, y, x2, x1, z

			for(int i=0;i!=size;++i) {
				lua::luastream::totop(str,y-i);	// x1, table, y, x2, z
				lua_rawseti(str,-2,i+1);	// table, y, x2, z
			}
			// insert table at index x
			return str;						// table, y, z
		});
	};
	// Unpacks the table at the given index into its parts
	inline lua::lManip<int> luaM_unpack(int x=-1) {
		return lua::lManip<int>(x,[](lua::luastream& str,int x) -> lua::luastream& {
			if (!str.valid(x)) throw lua::error("lManip<int> unpack","Invalid lua_State index");
			x = lua::toPosIndex(*str,x); 
			++str; int insert = x;

			while (lua_next(str,x)) {
				lua_insert(str,insert);
				++x;
			}
			return str;
		});
	};
	// Merges the two objects at index x and index y into one table
	inline lua::ldManip<int> luaM_merge(int x,int y) {
		return lua::ldManip<int>(x,y,[](lua::luastream& str,int x, int y) -> lua::luastream& {
			if (!str.valid(x) || !str.valid(y)) throw lua::error("lManip<int> merge","Invalid lua_State index");
			if (!str.typecheck(x,LUA_TTABLE) || !str.typecheck(y,LUA_TTABLE)) throw lua::error("ldManip<int> merge","Invalid type at index");
			if (lua_checkstack(*str,3) == 0) throw lua::error("lManip<int> merge","Insufficient stack space");

			lua::luastream::totop(str,x); x = str.size();
			lua::toPosIndex(*str,y); ++str;

			// Get the size of table(y) and add key to it ???
			while (lua_next(str,x)) {
				// Creates two copies of the key. One copy is used for lua_next, the other is popped in rawset
				lua_pushvalue(str,-2);		//	value(a), key(n), table(x)...table(y)
				lua_insert(str,-2);			//	value(a), key(a), key(n), table(x)...table(y)
				lua_rawset(str,y);			//	key(n), table(x)...table(y)
			}
			lua_pop(str,2);
			return str;
		});
	};

	// lua interaction

	// Pushes the lua var of the given name onto the lua stack
	inline lua::lManip<std::string> luaM_grab(std::string name) {
		return lua::lManip<std::string>(name,[](lua::luastream& str,std::string name) -> lua::luastream& {
			lua::getVar(*str,name);
			return str;
		});
	};
	// Sets the lua var of the given name to the value at the given index
	inline lua::ldManip<std::string,int> luaM_set(std::string name,int index) {
		return lua::ldManip<std::string,int> (name,index,[](lua::luastream& str,std::string name,int index) -> lua::luastream& {
			lua::luastream::totop(str,index);
			std::string field = lua::luaSplice(*str,name,'.');
			if (field == name)
				lua_setglobal(str,field.c_str());
			else {
				lua_pushstring(*str,field.c_str());
				lua_insert(str,-2);
				lua_settable(str,-3);
			}
			lua_remove(str,-1);
			return str;
		});
	};