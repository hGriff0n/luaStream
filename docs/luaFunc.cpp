#include "luaBindings.h"
#include "luaStream.h"
#include "luaManip.h"

int luaTest(lua_State* L) {
	lua::luastream stream(L);
	try {
		int x,y,z,addor;
		stream >> luaM_insert(-5) >> x >> y >> z;
		if (!stream.typecheck(-2,LUA_TFUNCTION)) throw lua::error("luaTest","invalid type at index");
		lua_pcall(stream,1,LUA_MULTRET,1);
		stream >> addor << (x+addor) << (y+addor) << (z+addor);
		return 3;
	}
	catch (lua::error& e) {
		stream.clear();
		stream << e.what();
		return 1;
	}
};

// taken from http://lua-users.org/wiki/DeferredCall
static int Lua_DcallDelegate(lua_State* L) {
  int efun = (lua_isfunction(L, 1))? 1 : luaL_optint(L, 1, 0);
  int nret = luaL_optint(L, 2, LUA_MULTRET);
  lua_checkstack(L, 1);
  lua_pushboolean(L, TRUE);
  int sm = lua_gettop(L);
  int ix = 1;
  while (!lua_isnone(L, lua_upvalueindex(ix))) {
    lua_checkstack(L, 1);
    lua_pushvalue(L, lua_upvalueindex(ix++));
  }
  ix--;
  if ((ix < 1) || (!lua_isfunction(L, (-1 * ix)))) return luaL_error(L, "Bad Deferred Call");
  if (lua_pcall(L, ix - 1, nret, efun) == 0)
    return (efun == 0)? lua_gettop(L) - sm : lua_gettop(L) - sm + 1; 
  else {
    lua_pushboolean(L, FALSE);
    lua_replace(L, sm);
    return (efun == 0)? lua_error(L) : 2;
  }
};

int luastream_DcallDelegate(lua_State* L) {
	lua::luastream stream(L);
	try {
		int efun = (stream.typecheck(1,LUA_TFUNCTION)) ? 1 : luaL_optint(*stream, 1, 0);
		int nret = luaL_optint(*stream, 2, LUA_MULTRET);
		(stream++) << true;
		int sm = stream.size();
		int ix = 1;
		while (!lua_isnone(stream, lua_upvalueindex(ix))) {
			(stream++) << lua_upvalueindex(ix++);
		}
		ix--;
		if ((ix < 1) || (!stream.typecheck((-1 * ix),LUA_TFUNCTION))) return luaL_error(*stream, "Bad Deferred Call");
		if (!lua_pcall(stream,ix - 1, nret, efun))
			return (stream.size() - sm + ((efun == 0)? 0 : 1)); 
		else {
			stream << false << luaM_replace(sm);
			return (efun == 0) ? lua_error(stream) : 2;
		}
	} catch (lua::error& e) {
		stream.clear();
		stream << e.what();
		return 1;
	}
}