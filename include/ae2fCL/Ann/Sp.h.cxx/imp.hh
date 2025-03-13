#include "../Sp.h"



#define ae2f_TMP ae2fCL_mAnnSp::

constexprfun cl_mem* ae2f_TMP WCl() {
    return ae2fCL_mAnnSpWCl(_this);
}
constexprfun const cl_mem* ae2f_TMP WCl() const {
    return ae2fCL_mAnnSpWCl(_this, const);
}

constexprfun cl_mem* ae2f_TMP IOCl() {
    return ae2fCL_mAnnSpIOCl(_this);
}
constexprfun const cl_mem* ae2f_TMP IOCl() const {
    return ae2fCL_mAnnSpIOCl(_this, const);
}

template<typename T>
constexprfun const T* ae2f_TMP X() const {
    return ae2fCL_mAnnSpX(_this, T*, const);
}
 
template<typename T>
constexprfun T* ae2f_TMP X() {
    return ae2fCL_mAnnSpX(_this, T*);
}

#undef ae2f_TMP


#define ae2f_TMP ae2fCL_AnnSp::

inline ae2f_TMP ~ae2fCL_AnnSp() {
    ae2fCL_AnnSpClean(this);
    return;
}

#undef ae2f_TMP