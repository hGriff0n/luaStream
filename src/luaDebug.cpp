#include "luaDebug.h"
#include "luaBindings.h"
#include <iomanip>

namespace lua {

	void printIndex(std::ostream& print,lua_State* stack,int index) {
		print << std::endl << ">at index " << index << "(" << lua::toNegIndex(stack,index) << ") { " << std::setw(9);
			switch(lua_type(stack,index)) {
				case LUA_TNIL:
					print << std::left << "nil";
					break;
				case LUA_TNUMBER:
					print << std::left << lua_tonumber(stack,index);
					break;
				case LUA_TBOOLEAN:
					print << std::left << (lua_toboolean(stack,index) ? "true" : "false");
					break;
				case LUA_TSTRING:
					print << std::left << lua_tostring(stack,index);
					break;
				default:
					print << std::left << lua_typename(stack,index);
					break;
			}
			print << " }";
	};

	void debugStack(std::ostream& print,lua_State* stack) {
		using namespace std;

		print << endl << "\n----lua stack debug info-----\nstack size: " << lua_gettop(stack) << endl;
		// other debug info goes here
	
		for(int i=1;i!=lua_gettop(stack)+1;++i) {
			printIndex(print,stack,i);
		}
		print << endl << "\n-----------------------------\n" << endl;
	};
	void debugState(std::ostream& print,luaState& stack) {
		using namespace std;

		print << endl << "\n----luaState debug info-----\nstack size: " << lua_gettop(*stack) << endl;
		// other debug info goes here
	
		for(int i=1;i!=lua_gettop(*stack)+1;++i) {
			printIndex(print,*stack,i);
		}
		print << endl << "\n-----------------------------\n" << endl;
	};
	void debugStream(std::ostream& print,luaStream& str) {
		using namespace std;

		print << endl << "\n----luaStream debug info-----\nstack size: " << str.size() << endl;
		// other debug info goes here
	
		for(int i=1;i!=str.size()+1;++i) {
			printIndex(print,*str,i);
		}
		print << endl << "\n-----------------------------\n" << endl;
	};

};