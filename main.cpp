#include <iostream>
#include <tuple>
#include <vector>
#include <boost/container/flat_set.hpp>
#include "machines/input_tree.h"
#include "machines/machines.h"
#include "machines/incompatibility_data.h"
#include "utils.h"



//inline boost::container::flat_set<uint32_t> intersect(const boost::container::flat_set<uint32_t>& set0,
//                                                      const boost::container::flat_set<uint32_t>& set1){
//
//    typedef boost::container::flat_set<uint32_t> flat_set;
//    flat_set intersection;
//
//    flat_set::const_iterator its[2]={set0.begin(),set1.begin()};
//    flat_set::const_iterator ends[2]={set0.end(), set1.end()};
//    int current=0;
//
//    while(true){
//        while(*its[current]<=*its[1-current]){
//            if(*its[current]==*its[1-current]){
//                intersection.insert(intersection.end(),*its[current]);
//            }
//
//
//            ++its[current];
//            if(its[current]==ends[current]){
//                return intersection;
//            }
//
//        }
//        current=1-current;
//    }
//}
//
//
//class IntersectionIterator {
//private:
//    typedef boost::container::flat_set<uint32_t> flat_set;
//    typedef boost::container::flat_set<uint32_t>::const_iterator iterator;
//    iterator its[2];
//    iterator ends[2];
//    bool current;
//    bool in_range=true;
//
//    inline void update(){
//        while (true) {
//            while (*its[current] <= *its[1 - current]) {
//                if (*its[current] == *its[1 - current]) {
//                    return;
//                }
//
//                ++its[current];
//                if (its[current] == ends[current]) {
//                    in_range=false;
//                    return;
//                }
//
//            }
//            current = 1 - current;
//        }
//    }
//
//public:
//
//    IntersectionIterator(const flat_set &set0, const flat_set &set1) :
//            its{set0.begin(), set1.begin()},
//            ends{set0.end(), set1.end()} {
//
//        if(its[0]== ends[0]|| its[1]==ends[1]){
//            in_range= false;
//            return;
//        }
//        current=(*its[1]<*its[0]);
//        update();
//    }
//
//
//    inline bool inRange() {
//        return in_range;
//    }
//
//    inline uint32_t next(){
//        const uint32_t& value=*its[current];
//        ++its[current];
//
//        if(its[current]==ends[current]) {
//            in_range = false;
//        } else {
//            current = 1 - current;
//            update();
//        }
//
//        return value;
//    }
//
//
//};
//



int main() {
    using namespace TailTest;

    NFA A(2);
//    A.addStates(3);
//
//    A.addSucc(0,1,2);
//    A.addSucc(0,1,1);
//
//    A.addSucc(1,0,2);
//    A.addSucc(1,0,0);
//
//    A.addSucc(2,0,0);
//    A.addSucc(2,1,1);

    A.addStates(3);

    A.addSucc(0,0,2);
    A.addSucc(0,1,1);
    A.addSucc(0,1,2);

    A.addSucc(1,1,2);

    A.addSucc(2,1,0);

    DFSM M(2,2);
//    M.addStates(3);
//
//    M.addTransition(0,1,1,1);
//    M.addTransition(0,0,0,2);
//
//    M.addTransition(1,1,0,2);
//    M.addTransition(1,0,1,0);
//
//    M.addTransition(2,1,1,1);
//    M.addTransition(2,0,1,0);

    M.addStates(2);
    M.addTransition(0,1,1,1);
    M.addTransition(0,0,1,1);

    M.addTransition(1,1,1,0);
    M.addTransition(1,0,0,1);


    std::cout<<"Machines built \n";

    typedef boost::container::flat_set<uint32_t> flat_set;

//    printSequence(M.getRSymbols(0));
//    printSequence(M.getRSymbols(1));
//    printSequence(M.getRSymbols(2));
//
//    flat_set set1={};
//    flat_set set2={1,2};
//    flat_set set3= intersect(set1,set2);


   IncompatibilityData data(M,A);

   for(uint32_t s=0; s<M.size(); ++s){
       for(uint32_t t=s+1; t<M.size(); ++t){
           for (uint32_t a = 0; a <A.size(); ++a) {
               for (uint32_t b = 0; b < A.size(); ++b) {
                   std::cout<<"("<<s<<','<<a<<"), and ("<<t<<","<<b<<") compatible: "<<
                   data.areCompatible(s,a,t,b)<<"\n";
               }
           }
       }
   }

    printSequence(data.distinguishingSequence(0,2,1,0));







//    std::vector<uint32_t> a={3,1};
//    std::vector iter={a.begin(),c.begin()};
//
//    for(auto it:iter){
//        std::cout<<*it<<"\n";
//    }


//    std::tuple<uint32_t ,uint32_t > pair=std::make_tuple(a,b);
//    std::cout<<std::apply(f,pair)<<"\n";

//    typedef boost::container::flat_set<uint32_t> flat_set;
//    flat_set set1,set2;
////    using boost::container::flat_set;
////    flat_set<uint32_t> set1,set2;
//    set1.insert(2);
//    set1.insert(3);
//    set1.insert(4);
//    set1.insert(7);
//    set1.insert(9);
//
//
//    set2.insert(2);
//    set2.insert(4);
//    set2.insert(5);
//    set2.insert(7);
//
//    auto set= intersect(set1,set2);
//
//    std::cout<<" Intersection: \n";
////    set1.insert(boost::container::ordered_unique_range, set2.begin(),set2.end());
////    std::cout << "Hello, World! " << set1.size()<< std::endl;
//    for(auto i=set.begin();
//    i!=set.end();
//    ++i){
//        std::cout<<*i<<"\n";
//    }
//
//    std::cout<<"\n Intersection iterator: \n";
//    IntersectionIterator iterator(set1,set2);
//    while(iterator.inRange()){
//        std::cout<<iterator.next()<<"\n";
//    }


    return 0;
}
