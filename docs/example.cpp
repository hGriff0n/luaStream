#include <iostream>
#include <iomanip>

#include "luaBindings.h"
#include "luaStream.h"
#include "luaManip.h"
#include "luaDebug.h"

int main(int argc,const char* argv[]) {
	using namespace std;
	using namespace lua;

	luastream str;
	bool test = true;
	try {
		str << 5 << 4 << 3 << 2 << 1 << luaM_pack(3) << luaM_unpack();
	}
	catch (lua::error& e) {
		cout << e.what();
		cin.get(); exit(0);
	};

	debugStack(cout,*str);
	cin.get();
};

void luaState::openState(lua_State* L) {
	luaL_openlibs(L);
};