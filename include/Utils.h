#pragma once

// baseManip - abstract templated parent class for stream manipulators
template <class _Stream>
class baseManip {
	public:
		// Pure virtual function that gets called by the stream operators
		virtual _Stream& manipulate(_Stream&) =0;

		// Stream operator overloads
		friend _Stream& operator<<(_Stream& str,baseManip<_Stream>& manip) {
			return manip.manipulate(str);
		};
		friend _Stream& operator>>(_Stream& str,baseManip<_Stream>& manip) {
			return manip.manipulate(str);
		};
};