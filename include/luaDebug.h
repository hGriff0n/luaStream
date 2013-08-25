#pragma once

#include "lua.hpp"
#include <string>

namespace lua {
	template <class _stream>
	class debug {
		private:
			_stream& stream;

		public:
			debug();
			debug(_stream& output) : stream(output) {};
			~debug();

			void pass(std::string& data) {
				stream << data << "\n";
			};
			void flush() {
				stream << "<---------------------------------------->\n\n";
				// Do filestreams have a flush member?
			};
	};

	std::string printIndex(lua_State*,int);

	template <class luaL,class _stream>
	void debuglua(luaL& obj,debug<_stream>& debugger) {
		debugger.pass(debugHead(obj));
		for(int i=start(obj);i!=end(obj)+1;++i) {
			// end returns size of lua stack
			// start returns 1 or x if stack below x is unneccessary (?) Is this a good thing?
			debugger.pass(printIndex(obj,i));
		}
		debugger.pass(debugBody(obj));
		debugger.flush();
	};

	// Have a targeted version of debuglua ???

	/*
	int start(obj);						-- Returns the index of the bottom of the luaStack of the passed obj
	int end(obj);						-- Returns the index of the top of the luaStack of the passed obj
	string debugHead(obj);				-- Returns the custom debug header for the passed obj
	string debugBody(obj);				-- Returns other state information for the passed obj
	operator lua_State*;				-- Class conversion operator to lua_State* must be defined
	*/
};