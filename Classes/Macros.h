//
//  Macros.hpp
//  Eleven
//
//  Created by 榎田直哉 on 2017/03/04.
//
//

#define ETOI(__VAL__)           static_cast<int>(__VAL__)
#define ITOE(__VAL__, __TYPE__) static_cast<__TYPE__>(__VAL__)

#define CREATE_FUNC_WITH_PARAM(__TYPE__, __PARAM__) \
static __TYPE__ * create(__PARAM__ param) \
{ \
    __TYPE__* pRet = new (std::nothrow)__TYPE__(); \
    if (pRet && pRet->init(param)) \
    { \
       pRet->autorelease(); \
       return pRet; \
    } \
    else \
    { \
       delete pRet; \
       pRet = nullptr; \
       return nullptr; \
    } \
}

