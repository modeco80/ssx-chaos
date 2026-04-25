#ifndef ML_CXX_BITCAST_HPP
#define ML_CXX_BITCAST_HPP

#include <ml/types.h>

namespace ml {

	/// Unsafe bit-casting. sizeof(T) Must == sizeof(U) or you get cheeses
	template<class T, class U>
	inline T bitCast(U u) {
		union Union {
			T t;
			U u;
		};
		Union uu;
		uu.u = u;
		return uu.t;
	}

}
#endif
