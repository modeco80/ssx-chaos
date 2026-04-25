#ifndef ML_CXX_BITCAST_HPP
#define ML_CXX_BITCAST_HPP

#include <ml/mem.h>
#include <ml/types.h>

namespace ml {

	/// Unsafe bit-casting. sizeof(T) Must == sizeof(U) or you get cheeses
	template <class T, class U>
	inline T bitCast(U u) {
		T t;
		mlMemCpy(&t, &u, sizeof(T));
		return t;
	}

} // namespace ml
#endif
