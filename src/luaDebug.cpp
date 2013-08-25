#include "luaDebug.h"
#include "luaBindings.h"
#include <iomanip>
#include <sstream>

namespace lua {

	std::string printIndex(lua_State* stack,int index) {
		std::stringstream stream;
		stream << " >at index " << index << "(" << toNegIndex(stack,index) << ") { " << std::setw(9);
			switch(lua_type(stack,index)) {
				case LUA_TNIL:
					stream << std::left << "nil";
					break;
				case LUA_TNUMBER:
					stream << std::left << lua_tonumber(stack,index);
					break;
				case LUA_TBOOLEAN:
					stream << std::left << (lua_toboolean(stack,index) ? "true" : "false");
					break;
				case LUA_TSTRING:
					stream << std::left << lua_tostring(stack,index);
					break;
				default:
					stream << std::left << lua_typename(stack,index);
					break;
			}
			stream << " }";
			return stream.str();
	};

};