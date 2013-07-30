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

			// static ???
			void close(luastream& master) {

			};
	};

	// luastream constructors/destructors

	luastream::luastream() : state(new impl()) {};
	luastream::luastream(lua_State* L) : stack(L),state(new impl()) {
		//state->sizeChange = num;
	};
	//luastream(const luastream& copy) : stack(copy.stack),state(new impl()) {};
	luastream::~luastream() {
		state->close(*this);
		delete state;
	};

	// luastream stack getters

	luaState& luastream::get() { return stack; };
	lua_State* luastream::operator*(void) { return *stack; };

	// luastream stack interaction

	void luastream::load(file::path& p) {
		addFile(*stack,p);
		// stack.loadFile(p.string());
	};
	void luastream::move(luastream& to,int num) {

	};
	bool luastream::call(std::string func,int args) {
		lua_pushstring(*stack,func.c_str());			//	Push the name of the function onto the stack
		lua_insert(*stack,-(args+1));					//	Insert the function under the arguments
		return lua_pcall(*stack,args,LUA_MULTRET,0);	//	Call the function
	};
	bool luastream::call(int index,int numRet) {
		return lua_pcall(*stack,(lua_gettop(*stack)-lua::toPosIndex(*stack,index)),numRet,0);
	};
	void luastream::clear() {
		lua_settop(*stack,0);
	};

	// luastream stack info functions

	int luastream::size() { return lua_gettop(*stack); };
	bool luastream::valid(int index,int adv) {
		return toPosIndex(*stack,index) <= (lua_gettop(*stack)+adv) && index!=0;
	};
	std::string luastream::typeat(int index) { return lua_typename(*stack,index); };
	bool luastream::typecheck(int index,int type) {	
		if (type == LUA_TFUNCTION) {
			return lua_isfunction(*stack,index);
		} else
			return (lua_type(*stack,index) == type);
	};

	// luastream stack size operators

	luastream& luastream::operator--() {
		if (lua_gettop(*stack) == 0) throw error("luastream decrement","Stack is already empty");
		lua_pop(*stack,1);
		return *this;
	};
	luastream& luastream::operator--(int) {
		return this->operator--();
	};
	luastream& luastream::operator++() {
		if (!lua_checkstack(*stack,1)) throw error("luastream increment","Stack is full");
		lua_pushnil(*stack);
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
		if (!lua_isnumber(*str,-1)) throw lua::error("luastream extraction (int)","Data type mismatch");
		setcvar(*str,var);
		return str;
	};
	luastream& operator>>(luastream& str,double& var) {
		if (!lua_isnumber(*str,-1)) throw lua::error("luastream extraction (double)","Data type mismatch");
		setcvar(*str,var);
		return str;
	};
	luastream& operator>>(luastream& str,std::string& var) {
		if (!lua_isstring(*str,-1)) throw lua::error("luastream extraction (string)","Data type mismatch");
		setcvar(*str,var);
		return str;
	};
	luastream& operator>>(luastream& str,bool& var) {
		if (!lua_isboolean(*str,-1)) throw lua::error("luastream extraction (bool)","Data type mismatch");
		setcvar(*str,var);
		return str;
	};

	// luastream manipulator helper functions
	
	void luastream::totop(luastream& str,int x) {
		if (!str.valid(x)) throw lua::error("lManip<int> totop","Invalid lua_State index");
		x = lua::toPosIndex(*str,x);
		for(int i=1;i!=lua_gettop(*str)-x+1;++i) lua_insert(*str,x);		// loop might have problems
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