luaStream is a way to interact with lua using a ostream-esque syntax
Objects are inserted onto the lua stack using << and extracted using >>
The stack size is increased (pushnil) with ++ and decreased with -- (remove)
The internal lua_State can be accessed with the *<stream> operator

Also provided are:
	A c++ class wrapper around lua_State (lua::luaState)
	A templated debug class for pushing debug info to various screens/files (lua::debug)
	A templated abstract class for stream manipulators (baseManip)
	Three luastream manipulator classes for 0,1 and 2 arguments (lua:: lnManip,lManip,ldManip)
	A templated class for loading c++ class into lua (lua::Lunar)
	An error class for exception handling (lua::error)
	Various helper functions
	
note: Except for baseManip, everything is exported under the lua namespace or has a lua(L/C/?)_ prefix

Use as you see fit. Please send all changes, ideas, or corrections back to me.