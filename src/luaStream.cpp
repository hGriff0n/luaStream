#include "luaStream.h"
#include "luaBindings.h"
// #include <bitset>
#include <sstream>

namespace lua {

	// addFile overload for file::path
	void addFile(lua_State* L, file::path& filepath) {
		addFile(L,filepath.string());
	};

	/*
	|	luastream definitions
	*/

	// luastream pimpl class definition

	struct luastream::impl {
		public:

			impl() {};
			~impl() {};

			/*static */void close(luastream& master) {

			};
	};

	// luastream constructors/destructors

	luastream::luastream() : state(new impl()) {};
	luastream::luastream(lua_State* L) : stack(L),state(new impl()) {
		//state->sizeChange = num;
	};
	luastream::luastream(const luastream& copy) : stack(copy.stack),state(new impl()) {};
	luastream::~luastream() {
		state->close(*this);
		delete state;
	};

	// luastream stack getters

	luaState& luastream::get() { return stack; };
	lua_State* luastream::operator*(void) { return *stack; };

	// luastream stack interaction

	void luastream::load(file::path& p) {
		stack.loadfile(p.string());
	};
	void luastream::move(luastream& to,int num) {

	};
	bool luastream::call(std::string func,int args) {
		lua_pushstring(stack,func.c_str());				//	Push the name of the function onto the stack
		lua_insert(stack,-(args+1));					//	Insert the function under the arguments
		return lua_pcall(stack,args,LUA_MULTRET,0);		//	Call the function
	};
	void luastream::clear() {
		lua_settop(stack,0);
	};

	// luastream stack info functions

	int luastream::size() { return lua_gettop(*stack); };
	bool luastream::valid(int index,int adv) {
		return toPosIndex(*stack,index) <= (lua_gettop(stack)+adv) && index!=0 && stack.isfreeindex(index);
	};
	std::string luastream::typeat(int index) { return lua_typename(*stack,index); };
	bool luastream::typecheck(int index,int type) {	
		return (lua_type(stack,index) == type);
	};

	// luastream stack size operators

	luastream& luastream::operator--() {
		if (lua_gettop(stack) == 0) throw error("luastream decrement","Stack is already empty");
		lua_pop(stack,1);
		return *this;
	};
	luastream& luastream::operator--(int) {
		return this->operator--();
	};
	luastream& luastream::operator++() {
		if (!lua_checkstack(stack,1)) throw error("luastream increment","Stack is full");
		lua_pushnil(stack);
		return *this;
	};
	luastream& luastream::operator++(int) {
		return this->operator++();
	};

	// luastream stream operator overloads
	
	luastream& operator<<(luastream& str,int value) {
		pushObj(*str,value);
		return str;
	};
	luastream& operator<<(luastream& str,double value) {
		pushObj(*str,value);
		return str;
	};
	luastream& operator<<(luastream& str,const char* value) {
		pushObj(*str,value);
		return str;
	};
	luastream& operator<<(luastream& str,std::string value) {
		pushObj(*str,value);
		return str;
	};
	luastream& operator<<(luastream& str,bool value) {
		pushObj(*str,value);
		return str;
	};

	luastream& operator>>(luastream& str,int& var) {
		if (!str.typecheck(-1,LUA_TNUMBER)) throw lua::error("luastream extraction (int)","Data type mismatch");
		setcvar(*str,var);
		return str;
	};
	luastream& operator>>(luastream& str,double& var) {
		if (!str.typecheck(-1,LUA_TNUMBER)) throw lua::error("luastream extraction (int)","Data type mismatch");
		setcvar(*str,var);
		return str;
	};
	luastream& operator>>(luastream& str,std::string& var) {
		if (!str.typecheck(-1,LUA_TSTRING)) throw lua::error("luastream extraction (string)","Data type mismatch");
		setcvar(*str,var);
		return str;
	};
	luastream& operator>>(luastream& str,bool& var) {
		if (!str.typecheck(-1,LUA_TBOOLEAN)) throw lua::error("luastream extraction (bool)","Data type mismatch");
		setcvar(*str,var);
		return str;
	};

	// luastream manipulator helper functions
	
	void luastream::totop(luastream& str,int x) {
		if (!str.valid(x)) throw lua::error("lManip<int> totop","Invalid lua_State index");
		x = lua::toPosIndex(*str,x);
		for(int i=1;i!=(str.size()-x+1);++i) lua_insert(str,x);		// loop might have problems
	};

	// state functions
	std::string dumpImpl() {
		//std::stringstream temp;
		return " ";
	};

	// debug functions
	int start(luastream& L) {
		// change later?
		return 1;
	};
	int end(luastream& L) {
		return L.size();
	};
	std::string debugHead(luastream& L) {
		return "<---luastream debug information--->\n";
	};
	std::string debugBody(luastream& L) {
		return L.dumpImpl() + "\n";
	};
	lua_State* toluaCore(luastream& L) {
		return *L;
	};
};

void lua_call(lua::luastream& L,int nargs,int nresults) {
	return lua_call(*L,nargs,nresults);
};
void lua_concat(lua::luastream& L,int n) {
	return lua_concat(*L,n);
};
int lua_cpcall(lua::luastream& L,lua_CFunction func,void* ud) {
	return lua_cpcall(*L,func,ud);
};
void lua_createtable(lua::luastream& L,int narr,int nrec) {
	return lua_createtable(*L,narr,nrec);
};
int lua_dump(lua::luastream& L,lua_Writer writer,void* data) {
	return lua_dump(*L,writer,data);
};
int lua_equal(lua::luastream& L,int index1,int index2) {
	return lua_equal(*L,index1,index2);
};
int lua_error(lua::luastream& L) {
	return lua_error(*L)
};
int lua_gc(lua::luastream& L,int what,int data) {
	return lua_gc(*L,what,data);
};
void lua_getfenv(lua::luastream& L,int index) {
	return lua_getfenv(*L,index);
};
void lua_getfield(lua::luastream& L,int index,const char* k) {
	return lua_getfield(*L,index,k);
};
int lua_getmetatable(lua::luastream& L,int index) {
	return lua_getmetatable(*L,index);
};
void lua_gettable(lua::luastream& L,int index) {
	return lua_gettable(*L,index);
};
void lua_insert(lua::luastream& L,int index) {
	return lua_insert(*L,index);
};
int lua_iscfunction(lua::luastream& L,int index) {
	return lua_iscfunction(*L,index);
};
int lua_isuserdata(lua::luastream& L,int index) {
	return lua_isuserdata(*L,index);
};
int lua_isstring(lua::luastream& L,int index) {
	return lua_isstring(*L,index);
};
int lua_lessthan(lua::luastream& L,int index1,int index2) {
	return lua_lessthan(*L,index1,index2);
};
lua_State* lua_newthread(lua::luastream& L) {
	return lua_newthread(*L);
};
int lua_next(lua::luastream& L,int index) {
	return lua_next(*L,index);
};
size_t lua_objlen(lua::luastream& L,int index)  {
	return lua_objlen(*L,index);
};
int lua_pcall(lua::luastream& L,int nargs,int nresult,int errfunc)  {
	return lua_pcall(*L,nargs,nresult,errfunc);
};
void lua_pushboolean(lua::luastream& L,int b) {
	return lua_pushboolean(*L,b)
};
void lua_pushcclosure(lua::luastream& L,lua_CFunction fn,int n) {
	return lua_pushcclosure(*L,fn,n);
};
void lua_pushinteger(lua::luastream& L,lua_Integer n) {
	return lua_pushinteger(*L,n);
};
void lua_pushlightuserdata(lua::luastream& L,void* ud) {
	return lua_pushlightuserdata(*L,ud);
};
void lua_pushlstring(lua::luastream& L,const char* s,size_t len) {
	return lua_pushlstring(*L,s,len);
};
void lua_pushnil(lua::luastream& L) {
	return lua_pushnil(*L);
};
void lua_pushnumber(lua::luastream& L,lua_Number n) {
	return lua_pushnumber(*L,n);
};
void lua_pushstring(lua::luastream& L,const char* s) {
	return lua_pushstring(*L,s);
};
int lua_pushthread(lua::luastream& L) {
	return lua_pushthread(*L);
};
void lua_pushvalue(lua::luastream& L,int index) {
	return lua_pushvalue(*L,index);
};
int lua_rawequal(lua::luastream& L,int index1,int index2) {
	return lua_rawequal(*L,index1,index2);
};
void lua_rawget(lua::luastream& L,int index) {
	return lua_rawget(*L,index);
};
void lua_rawgeti(lua::luastream& L,int index,int n) {
	return lua_rawgeti(*L,index,n);
};
void lua_rawset(lua::luastream& L,int index) {
	return lua_rawset(*L,index);
};
void lua_rawseti(lua::luastream& L,int index,int n) {
	return lua_rawseti(*L,index,n);
};
void lua_remove(lua::luastream& L,int index) {
	return lua_remove(*L,index);
};
void lua_replace(lua::luastream& L,int index) {
	return lua_replace(*L,index);
};
int lua_resume(lua::luastream& L,int narg) {
	return lua_resume(*L,narg);
};
int lua_setfenv(lua::luastream& L,int index) {
	return lua_setfenv(*L,index);
};
void lua_setfield(lua::luastream& L,int index,const char* k) {
	return lua_setfield(*L,index,k);
};
int lua_setmetatable(lua::luastream& L,int index) {
	return lua_setmetatable(*L,index);
};
void lua_settable(lua::luastream& L,int index) {
	return lua_settable(*L,index);
};
void lua_settop(lua::luastream& L,int index) {
	return lua_settop(*L,index);
};
int lua_status(lua::luastream& L) {
	return lua_status(*L);
};
int lua_toboolean(lua::luastream& L,int index) {
	return lua_toboolean(*L,index);
};
lua_CFunction lua_tocfunction(lua::luastream& L,int index) {
	return lua_tocfunction(*L,index);
};
lua_Integer lua_tointeger(lua::luastream& L,int index) {
	return lua_tointeger(*L,index);
};
const char* lua_tolstring(lua::luastream& L,int index,size_t* len) {
	return lua_tolstring(*L,index,len);
};
lua_Number lua_tonumber(lua::luastream& L,int index) {
	return lua_tonumber(*L,index);
};
lua_State* lua_tothread(lua::luastream& L,int index) {
	return lua_tothread(*L,index);
};
void lua_xmove(lua::luastream& from,lua::luastream& to,int n) {
	return lua_xmove(*from,*to,n);
};