#pragma once

#include "luaState.h"
#include <filesystem>

namespace file = std::tr2::sys;

namespace lua {

	/*--------------------------------------------------------------------------------------->
	||	note: default argument values are not given here (see class declaration)
	||	note: function explanations are not given here (see class declaration)
	||	note: 'string' = 'std::string'
	||
	||	class: lua::luastream
	||	intent: Provide a ostream-esque interface to lua
	||	constructors:
	||		default:	given - void
	||		copy:		given - luastream		-- currently private
	||		luaFunc:	given - lua_State*
	||	interface:
	||		dumpImpl:	  return - string			given - void				use - dumpImpl()
	||		operator*:	  return - lua_State*		given - void				use - *<stream>
	||		load:		  return - void				given - file::path&			use - load(<path of lua script>)
	||		move:		  return - void				given - luastream&,int		use - move(<stream to>,<num to move>)
	||		call:		  return - bool				given - string,int			use - call(<func>,<numArgs>)
	||		clear:		  return - void				given - void				use - clear()
	||		size:		  return - int				given -	void				use - size()
	||		valid:		  return - int				given - int,int				use - valid(<index>,<modifier>)
	||		typeat:		  return - string			given - int					use - typeat(<index>)
	||		operator--:	  return - luastream&		given - void				use - --<stream>
	||		operator--:	  return - luastream&		given - void				use - <stream>--
	||		operator++:	  return - luastream&		given - void				use - ++<stream>
	||		operator++:	  return - luastream&		given - void				use - <stream>++
	||		operator<<:	  return - luastream&		insertion operator			use - <stream> << <value>
	||		operator>>:	  return - luastream&		extraction operator			use - <stream> >> <variable>
	||		totop:		  return - void				given - luastream&,int		use - luastream::totop(<stream>,<index>)
	||	TODO:
	||		Enable copy constructor
	||		Have impl be changeable
	||		Template operator>> & operator<<	(luaBindings.h would have to be included)
	\*---------------------------------------------------------------------------------------->*/

	void addFile(lua_State*,file::path&);

	class luastream {
		private:
			luaState stack;

			// forward declaration of pimpl class
			struct impl;
			impl* state;

			luastream& operator=(const luastream&);		// delete function keyword not currently supported in vs2012
			luastream(const luastream&);
		public:
			// constructors/destructors
			luastream();							// default
			luastream(lua_State*);					// luaFunc		Enables luastream use in functions registered with lua
			~luastream();

			// state functions
			std::string dumpImpl(const char*);		//	Returns the internal state as a string

			// stack getters
			luaState& get();						//	Returns internal luaState&
			lua_State* operator*(void);				//	Returns internal lua_State*
			operator lua_State* ();					//	Conversion operator

			// stack interaction
			void load(file::path&);					//	Load a file into lua
			void move(luastream&,int=-1);			//	Move values into the passed luastream. Passes all values by default
			bool call(std::string,int=0);			//	Calls the passed function with x arguments. Defaults to 0 arguments
			void clear();							//	Clears the stack of all values

			// stack info functions
			int size();								//	Returns the number of values in the stack
			bool valid(int,int=0);					//	Is the passed index a valid lua index. Second arg adds to the stack size
													//	(Stack's expected future size at time of use) (defaults to no change)
			std::string typeat(int);				//	Returns the type of the value at stack[index]		-- Mark
			bool typecheck(int,int);				//	Checks if the value at stack[index] is the passed type

			// stack size operators
			luastream& operator--();				//	Decreases the stack's size by one. Removes the top value from the stack
			luastream& operator--(int);
			luastream& operator++();				//	Increases the stack's size by one. Pushes a nil value onto stack top
			luastream& operator++(int);

			// stream operator overloads		-- override these to enable insertion and extraction of custom types
			// do not override with templates if you want the manipulators to work (???)
			friend luastream& operator<<(luastream&,lua_Integer);
			friend luastream& operator<<(luastream&,lua_Number);
			friend luastream& operator<<(luastream&,const char*);
			friend luastream& operator<<(luastream&,std::string);
			friend luastream& operator<<(luastream&,bool);

			friend luastream& operator>>(luastream&,lua_Integer&);
			friend luastream& operator>>(luastream&,lua_Number&);
			friend luastream& operator>>(luastream&,std::string&);
			friend luastream& operator>>(luastream&,bool&);

			// manipulator helper function		-- common code for use in custom manipulators 
			static void totop(luastream&,int);		// Moves the value at <stream>[index] to the top of the stream
	};

	// debug overloads for luastream. Same usage as shown in luaBindings.h
	int start(luastream&);
	int end(luastream&);
	std::string debugHead(luastream&);
	std::string debugBody(luastream&,const char* = "-");
};