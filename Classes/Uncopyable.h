//
//  Uncopyable.hpp
//  Eleven
//
//  Created by 榎田直哉 on 2017/03/04.
//
//

#ifndef __Uncopyable__
#define __Uncopyable__

class Uncopyable
{
protected:
    Uncopyable() {}
    ~Uncopyable() {}
private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator = (const Uncopyable&);
};

#endif /* defined(__tlcore__tlUncopyable__) */
