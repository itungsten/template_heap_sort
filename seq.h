//
// Created by tungsten on 4/17/2021.
//

#ifndef UNTITLED_SEQ_H
#define UNTITLED_SEQ_H

#include<iostream>
#include<type_traits>
#include<algorithm>

//容器
template<int... args>
struct seq{};

//检验正确性的尺子
template<int... args>
void printSeq(seq<args...>){
    auto f=[](auto x){
        ::std::cout<<x<<" ";
    };
    (... , f(args));
    ::std::cout<<::std::endl;
}


//seqSplit的实现
//将seq以pos处为中枢，分为 first(seq),pos(atomic),second(seq)三部分
template<int pos,typename A,typename B>
struct seqSplitHelper
{};
template<int... A,int Bfirst,int... Bothers>
struct seqSplitHelper<1,seq<A...>,seq<Bfirst,Bothers...>>
{
    using first=seq<A...>;
    using second=seq<Bothers...>;
    const static int value=Bfirst;
};
template<int pos,int... A,int Bfirst,int... Bothers>
struct seqSplitHelper<pos,seq<A...>,seq<Bfirst,Bothers...>>
{
    using first=typename seqSplitHelper<pos - 1,seq<A...,Bfirst>,seq<Bothers...>>::first;
    using second=typename seqSplitHelper<pos - 1,seq<A...,Bfirst>,seq<Bothers...>>::second;
    const static int value=seqSplitHelper<pos - 1,seq<A...,Bfirst>,seq<Bothers...>>::value;
};
template<int pos,typename T>
using seqSplit=seqSplitHelper<pos,seq<>,T>;

//将序列seqA和seqB合并
template<typename A,typename B>struct seqMerge;
template<int... A,int... B>
struct seqMerge<seq<A...>,seq<B...>>
{
    using res=seq<A...,B...>;
};

//将一个原子push到seq尾部
template<typename T,int ele>
struct elePush;
template<int... args,int ele>
struct elePush<seq<args...>,ele>
{
    using res=typename seq<args...,ele>;
};

//交换序列seq中posA和posB处元素值
template<int posA,int posB,typename T>struct eleSwap;
template<int posA,int posB,int... args>
struct eleSwap<posA,posB,seq<args...>>
{
    //切
    using secL=typename seqSplit<posA,seq<args...>>::first;
    using tmp=typename seqSplit<posA,seq<args...>>::second;
    using secM=typename seqSplit<posB - posA,tmp>::first;
    using secR=typename seqSplit<posB,seq<args...>>::second;
    const static int valA=seqSplit<posA,seq<args...>>::value;
    const static int valB=seqSplit<posB,seq<args...>>::value;
    //合
    using tmp1=typename elePush<secL,valB>::res;
    using tmp2=typename seqMerge<tmp1,secM>::res;
    using tmp3=typename elePush<tmp2,valA>::res;
    using res=typename seqMerge<tmp3,secR>::res;
};
#endif //UNTITLED_SEQ_H
