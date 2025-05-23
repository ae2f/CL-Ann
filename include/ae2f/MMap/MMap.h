#ifndef ae2f_MMap_h
#define ae2f_MMap_h

#include <ae2f/Float.auto.h>
#include <ae2f/Call.h>
#include <ae2f/Cast.h>
#include <ae2f/errGlob.h>
#include <stdlib.h>

#include <ae2f/Pack/Beg.h>

/** @brief
 * Multilinear map, or tensor.
 *
 * 
 * */
typedef struct ae2f_mMMap {
	/** @brief aka dimension count */
	size_t dim;

#if ae2f_WhenCXX(!) 0
#include "./MMap.x/mMMap.hh"
#endif

} ae2f_mMMap;

/** @brief Length vector for every dimension index. */
#define ae2f_mMMapDimLen(mmap, ...) \
	ae2f_reinterpret_cast(__VA_ARGS__ size_t*, (mmap) + 1)

/** @breif Memory field. Indexer needed. */
#define ae2f_mMMapField(mmap, ...) \
	ae2f_reinterpret_cast(__VA_ARGS__ ae2f_float_t*, ae2f_mMMapDimLen(mmap, __VA_ARGS__) + (mmap)->dim)

#include <assert.h>

/**
 * @brief
 * Calculates the memory length needed in bytes.
 *
 * @param dim
 * Dimension, aka length of lens.
 *
 * @param lens
 * Length vector.
 *
 * */
ae2f_WhenC(inline static) ae2f_WhenCXX(constexpr)
size_t ae2f_mMMapSz(
		size_t dim, 
		const size_t* lens
		) noexcept
{
	size_t v = 1;

	if(!lens) {
		return -1;
	}

	for(size_t _dim = dim - 1;_dim != -1; _dim--) {
		size_t L = lens[_dim];

		if(L)
			v *= L;
		else
			return -1;
	}

	return 
			v * sizeof(ae2f_float_t)
		+	(dim + 1) * sizeof(size_t);
}

// 1, 2, ..... n
ae2f_WhenC(inline static) ae2f_WhenCXX(constexpr)
size_t ae2f_mMMapFieldIdx(
		const ae2f_mMMap* mmap,
		size_t dim,
		const size_t* idxs
		) noexcept
{
	if(!idxs) return -1;
	if(!mmap) return -1;
	if(dim > mmap->dim) dim = mmap->dim;

	size_t i = 0, ret = 0;

	const size_t *lens = ae2f_mMMapDimLen(mmap, const);

	for(; i < dim - 1; i++) {
		const size_t L = lens[dim - i - 1], I = idxs[dim - 1 - i] % L;
		if(!L) return -1;
		ret += I;
		ret *= L;
	}

	ret += idxs[dim - 1 - i] % lens[dim - i - 1];
	dim = mmap->dim;

	for(; i < dim - 1; i++) {
		ret *= lens[dim - 1 - i];
	}

	return ret; // / lens[i - 1];
}


ae2f_extern ae2f_SHAREDCALL
size_t ae2f_mMMapInit(
		ae2f_mMMap* mmap,
		size_t dim,
		const size_t* lens,
		ae2f_err_t* errret
		) noexcept;


ae2f_extern ae2f_SHAREDCALL
ae2f_mMMap* ae2f_mMMapMk(
		size_t dim,
		const size_t* lens,
		ae2f_err_t* errret
		) noexcept;


#define ae2f_mMMapDel free

typedef union ae2f_MMap {
	ae2f_mMMap MMap;
#if ae2f_WhenCXX(!)0
#include "./MMap.x/MMap.hh"
#endif
} ae2f_MMap;

/*** END ***/
#if ae2f_WhenCXX(!)0
#include "./MMap.x/imp.hh"
#endif

#include <ae2f/Pack/End.h>

#endif
