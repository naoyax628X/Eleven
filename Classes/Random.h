//
//  Random.hpp
//  Eleven
//
//  Created by 榎田直哉 on 2017/03/05.
//
//

#ifndef __Random__
#define __Random__

namespace Random
{
    template<class _itr>
    void shuffle(_itr first, _itr last)
    {
        typedef typename std::iterator_traits<_itr>::difference_type difference_type;
        difference_type len = last - first;
        
        while (len > 1)
        {
            int a = static_cast<int>(len) - 1;
            int b = cocos2d::random(0, a);
            
            std::swap(*(first + a), *(first + b));
            
            --len;
        }
    };
};

#endif /* defined(__Random__) */
