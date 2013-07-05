#pragma once

template <class _Stream>
class baseManip {
	public:
		virtual _Stream& manipulate(_Stream&) =0;

		friend _Stream& operator<<(_Stream& str,baseManip<_Stream>& manip) {
			return manip.manipulate(str);
		};
		friend _Stream& operator>>(_Stream& str,baseManip<_Stream>& manip) {
			return manip.manipulate(str);
		};
};