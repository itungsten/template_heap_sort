//
// Created by tungsten on 4/17/2021.
//

#ifndef HEAP_SORT_HEAP_SORT_H
#define HEAP_SORT_HEAP_SORT_H

#include "seq.h"

template<int pos,typename T>struct heapDownHelper;
template<typename T1,typename T2,int pos, typename A> struct heapDown;  //标签分发  tag1:hasLeft tag2:hasRight

template<int pos, int... A>
struct heapDown<::std::false_type ,::std::false_type,pos,seq<A...>>  //!hasLeft !hasRight
{
    using res=seq<A...>;
};
template<int pos, int... A>
struct heapDown<::std::true_type ,::std::false_type ,pos,seq<A...>> //hasLeft !hasRight
{
    const static int leftV=seqSplit<pos * 2,seq<A...>>::value;
    const static int posV=seqSplit<pos,seq<A...>>::value;
    const static bool toSwap=leftV>posV;
    using res=::std::conditional_t<
            toSwap,
            typename eleSwap<pos,pos*2,seq<A...>>::res,
            seq<A...>
    >;
};
template<int pos, int... A>
struct heapDown<::std::true_type ,::std::true_type,pos,seq<A...>>   //hasLeft hasRight
{
    const static int posV=seqSplit<pos,seq<A...>>::value;
    const static int leftV=seqSplit<pos * 2,seq<A...>>::value;
    const static int rightV=seqSplit<pos * 2 + 1,seq<A...>>::value;
    const static int maxV=::std::max(::std::max(leftV,rightV),posV);
    //找出三者最大节点

    const static bool isPos= posV==maxV;
    const static bool isLeft= leftV==maxV;

    //三种情况
    using res=::std::conditional_t<
            isPos,
            seq<A...>,
            ::std::conditional_t<
                    isLeft,
                    typename heapDownHelper<pos * 2,typename eleSwap<pos,pos*2,seq<A...>>::res>::res,
                    typename heapDownHelper<pos * 2 + 1,typename eleSwap<pos,pos*2+1,seq<A...>>::res>::res
            >
    >;
};

template<int pos,int... args>
struct heapDownHelper<pos,seq<args...>>
{
    const static bool hasLeft=(pos*2<=sizeof...(args));
    const static bool hasRight=(pos*2+1<=sizeof...(args));
    using res=typename heapDown<
            ::std::bool_constant<hasLeft>,
            ::std::bool_constant<hasRight>,
            pos,
            seq<args...>
    >::res;
};

//只在建堆时使用，从n/2节点数到1号节点
template<int pos, typename A> struct heapUp;
template<int... A>
struct heapUp<0,seq<A...>>
{
    using res=seq<A...>;
};
template<int pos,int... A>
struct heapUp<pos,seq<A...>>
{
    using res=typename heapUp<pos - 1,typename heapDownHelper<pos,seq<A...>>::res>::res;
};

template<typename A,typename B> struct heap_sort;
template<int... Aint,int Bfirst,int... Bothers>
struct heap_sort<seq<Aint...>,seq<Bfirst,Bothers...>>
{
    static const int lenB=sizeof...(Bothers)+1;
    using swaped=typename eleSwap<1,lenB,seq<Bfirst,Bothers...>>::res; //首元素下沉
    using removed=typename seqSplit<lenB,swaped>::first;    //弹出最大值
    using updated=typename heapDownHelper<1,removed>::res;  //维护堆性质

    using res=typename heap_sort<
            seq<Aint...,Bfirst>,
            updated
    >::res;
    //下一次循环
};

template<int... Aint,int Bfirst> //递归出口
struct heap_sort<seq<Aint...>,seq<Bfirst>>
{
    using res=seq<Aint...,Bfirst>;
};

template<typename T>struct sort;
template<int... Bint>
struct sort<seq<Bint...>>
{
    using init=typename heapUp<(sizeof...(Bint)) / 2,seq<Bint...>>::res; //初始化建堆
    using res=typename heap_sort<seq<>,init>::res;
};

#endif //HEAP_SORT_HEAP_SORT_H
