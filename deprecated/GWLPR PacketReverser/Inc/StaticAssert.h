#ifndef StaticAssert_h__
#define StaticAssert_h__
/*
 © 2011 ACB
*/

template<bool> class StaticAssert;
template<> class StaticAssert<true>{};

#define static_assert(x) StaticAssert<(x)>()

#endif // StaticAssert_h__