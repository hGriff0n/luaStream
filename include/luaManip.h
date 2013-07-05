#pragma once

#include "Utils.h"
#include "luaStream.h"
#include <functional>

namespace lua {

	// luastream manipulator classes (distinquished by number of arguments)
	// Uses std::function to store manipulator. Supports using lambdas in constructor
	// Manipulator Arguments must be given to the class at construction time
	// baseManip<luastream> should define insertion and extraction operators for the classes
	
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

	// Two Arguments (lua dual)
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

	// Provided luastream manipulators
	// Given as functions that return the manipulators
	// Lambdas are almost solely used in the constructors

	// lua stack manipulation/organization

	inline lManip<int> totop(int x) {
		return lManip<int>(x,[](luastream& str,int x) -> luastream& {
			luastream::totop(str,x);
			return str;
		});
	};
	inline lManip<int> insert(int x) {
		return lManip<int>(x,[](luastream& str,int x) -> luastream& {
			if (!str.valid(x)) throw lua::error("lManip<int> insert","Invalid lua_State index");
			lua_insert(*str,lua::toPosIndex(*str,x));
			return str;
		});
	};
	inline ldManip<int> reverse(int x=1,int y=-1) {
		return ldManip<int>(x,y,[](luastream& str,int x,int y) -> luastream& {
			x = lua::toPosIndex(*str,x);	y = lua::toPosIndex(*str,y);
			if (!str.valid(x) || !str.valid(y)) throw lua::error("ldManip<int> reverse","Invalid lua_State index");
			for(int i=0;i!=abs(y-x);++i) { 
				luastream::totop(str,x);
				lua_insert(*str,(y-i));
			};
			return str;
		});
	};
	inline ldManip<int> copy(int x,int y=-1) {
		return ldManip<int>(x,y,[] (luastream& str,int x,int y) -> luastream& {
			if (!str.valid(y,1)) throw lua::error("ldManip<int> copy","Invalid lua_State index");
			lua_pushvalue(*str,x);
			lua_insert(*str,y);
			return str;
		});
	};
	inline lManip<int> remove(int x) {
		return lManip<int>(x,[](luastream& str,int x) -> luastream& {
			if (!str.valid(x)) throw lua::error("lManip<int> remove","Invalid lua_State index");
			lua_remove(*str,x);
			return str;
		});
	};

	// table manipulations
	
	inline ldManip<int> pack(int x=1,int y=-1) {
		return ldManip<int>(x,y,[](luastream& str, int x,int y) -> luastream& {
			x = lua::toPosIndex(*str,x); y = lua::toPosIndex(*str,y);
			int size = abs(y-x);

			lua_createtable(*str,size,0);	// table, y, x2, x1, z

			for(int i=0;i!=size;++i) {
				luastream::totop(str,y-i);	// x1, table, y, x2, z
				lua_rawseti(*str,-2,i+1);	// table, y, x2, z
			}
			return str;
		});
	};
	inline lManip<int> unpack(int x=-1) {
		return lManip<int>(x,[](luastream& str,int x) -> luastream& {
			if (!str.valid(x)) throw lua::error("lManip<int> unpack","Invalid lua_State index");
			x = lua::toPosIndex(*str,x); 
			++str; int insert = x;

			while (lua_next(*str,x)) {
				lua_insert(*str,insert);
				++x;
			}
			return str;
		});
	};
	// Needs work. lua::pack doesn't produce a table (of type LUA_TTABLE)
	inline ldManip<int> merge(int x,int y) {
		return ldManip<int>(x,y,[](luastream& str,int x, int y) -> luastream& {
			if (!str.valid(x) || !str.valid(y)) throw lua::error("lManip<int> merge","Invalid lua_State index");
			if (!lua_istable(*str,x) || !lua_istable(*str,y)) throw lua::error("ldManip<int> merge","Invalid type at index");
			if (lua_checkstack(*str,3) == 0) throw lua::error("lManip<int> merge","Insufficient stack space");

			luastream::totop(str,x); x = str.size();
			lua::toPosIndex(*str,y); ++str;

			// Get the size of table(y) and add key to it ???
			while (lua_next(*str,x)) {
				// Creates two copies of the key. One copy is used for lua_next, the other is popped in rawset
				lua_pushvalue(*str,-2);		//	value(a), key(n), table(x)...table(y)
				lua_insert(*str,-2);			//	value(a), key(a), key(n), table(x)...table(y)
				lua_rawset(*str,y);			//	key(n), table(x)...table(y)
			}
			lua_pop(*str,2);
			return str;
		});
	};

	// lua interaction

	inline lManip<std::string> grab(std::string name) {
		return lManip<std::string>(name,[](luastream& str,std::string name) -> luastream& {
			getVar(*str,name);
			return str;
		});
	};
	inline ldManip<std::string,int> set(std::string name,int index) {
		return ldManip<std::string,int> (name,index,[](luastream& str,std::string name,int index) -> luastream& {
			luastream::totop(str,index);
			std::string field = luaRegEx(*str,name,'.');
			if (field == name)
				lua_setglobal(*str,field.c_str());
			else {
				lua_pushstring(*str,field.c_str());
				lua_insert(*str,-2);
				lua_settable(*str,-3);
			}
			lua_remove(*str,-1);
			return str;
		});
	};


};