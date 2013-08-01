#include "luaBindings.h"

namespace lua {
	std::string luaSplice(lua_State* L,std::string base,char seperator,int position) {
		size_t length = base.find(seperator);				// find the first instance of the seperator in the string
		if (length == std::string::npos) return base;

		std::string field = base.substr(0,length);			// split the string
		std::string var = base.substr(length+1);
		lua_getfield(L,position,field.c_str());				// get the field from the table at index position
		
		if (position != LUA_GLOBALSINDEX) lua_remove(L,-2);	// remove the parent table
		return luaSplice(L,var,seperator,-1);				// recursive call
	};
	int toPosIndex(lua_State* L,int index) {
		if (index < 0) return lua_gettop(L)+index+1;
		return index;		// return LUA_GLOBALSINDEX;	???
	};
	int toNegIndex(lua_State* L,int index) {
		if (index > 0) return index-lua_gettop(L)-1;
		return index;
	};

	void addFile(lua_State* L,std::string& name) {
		if (luaL_dofile(L,name.c_str())) throw lua::error("addFile",lua_tostring(L, -1));
	};

	void getVar(lua_State* L,std::string name) {
		std::string field = luaSplice(L,name);
		if (field == name) {
			lua_getglobal(L,field.c_str());
		} else {
			lua_pushstring(L,field.c_str());
			lua_gettable(L,-2);
			lua_remove(L,-2);
		}
	};

	void pushObj(lua_State* L,double data) {
		lua_pushnumber(L,data);
	};
	void pushObj(lua_State* L,const char* data) {
		lua_pushstring(L,data);
	};
	void pushObjpushObj(lua_State* L,std::string data) {
		lua_pushstring(L,data.c_str());
	};
	void pushObj(lua_State* L,bool data) {
		lua_pushboolean(L,data);
	};
	void pushObj(lua_State* L,int data) {
		lua_pushinteger(L,data);
	};

	void setcvar(lua_State* L, int& value) {
		if (!lua_isnumber(L,-1)) throw lua::error("setcvar(int)","Top of luaStack is not a number");
		value = lua_tointeger(L,-1);
		lua_remove(L,-1);
	};
	void setcvar(lua_State* L, double& value) {
		if (!lua_isnumber(L,-1)) throw lua::error("setcvar(double)","Top of luaStack is not a number"); 
		value = lua_tonumber(L,-1);
		lua_remove(L,-1);
	};
	void setcvar(lua_State* L, std::string& value) {
		if (!lua_isstring(L,-1)) throw lua::error("setcvar(string)","Top of luaStack is not a string");
		value = lua_tostring(L,-1);
		lua_remove(L,-1);
	};
	void setcvar(lua_State* L, bool& value) {
		if (!lua_isboolean(L,-1)) throw lua::error("setcvar(bool)","Top of luaStack is not a boolean");
		value = lua_toboolean(L,-1);
		lua_remove(L,-1);
	};

	error::error(std::string place,std::string what) : funcName(place),errorMsg(what) {};
	error::~error() {};

	std::string error::what() {
		return ("luaStream error in " + funcName + "\n" + errorMsg + "\n");
	};

	int start(lua_State* L) {
		return 1;
	};
	int end(lua_State* L) {
		return lua_gettop(L);
	};
	std::string debugHead(lua_State* L) {
		return "<---lua_State debug information--->\n";
	};
	std::string debugBody(lua_State* L,const char* ch) {
		return "\n"
	};
	lua_State* toluaCore(lua_State* L) {
		return L;
	};
};