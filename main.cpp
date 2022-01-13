#include <iostream>
#include <tuple>
#include <vector>
#include <boost/container/flat_set.hpp>
#include "machines/input_tree.h"
#include "machines/machines.h"
#include "machines/incompatibility_data.h"
#include "machines/quotient.h"
#include "testers/basic.h"
#include "machines/serializer.h"
#include "machines/builders.h"
#include "advanced.h"


//inline boost::container::num_vector<uint32_t> intersect(const boost::container::num_vector<uint32_t>& set0,
//                                                      const boost::container::num_vector<uint32_t>& set1){
//
//    typedef boost::container::num_vector<uint32_t> num_vector;
//    num_vector intersection;
//
//    num_vector::const_iterator its[2]={set0.begin(),set1.begin()};
//    num_vector::const_iterator ends[2]={set0.end(), set1.end()};
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
//    typedef boost::container::num_vector<uint32_t> num_vector;
//    typedef boost::container::num_vector<uint32_t>::const_iterator iterator;
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
//    IntersectionIterator(const num_vector &set0, const num_vector &set1) :
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

typedef std::vector<uint32_t> NumVec;


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

    ContainmentRelation rel(A.size());
    rel.setContainment(0,1,true);


    DFSM H(3,2);
    H.addStates(3);

    H.addTransition(0,0,0,2);
    H.addTransition(0,1,1,1);
    H.addTransition(0,2,1,2);

    H.addTransition(1,0,1,2);
    H.addTransition(1,1,1,2);
    H.addTransition(1,2,1,2);


    H.addTransition(2,0,1,0);
    H.addTransition(2,1,1,0);
    H.addTransition(2,2,1,0);



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
    M.addTransition(1,0,0,0);



    ContainmentRelation relation = computeLookAheadFwdSimRel(A,100);
    for(uint32_t a=0; a<A.size(); ++a){
        for(uint32_t b=0; b<A.size(); ++b){
            std::cout<<" State "<<a<<" contains "<<b<<" : "<<rel.contains(a,b)<<"\n";
        }
    }

//
//    AdvancedTester tester(M,A,rel);
//
//    auto suite=tester.getSuite(4);
//    std::cout<<"Advanced: \n";
//
//    suite.print();
//
//    std::cout<<" Number of tests: "<<suite.countSequences()<<"\n";
//
//    std::cout<<"\n Basic:";
//
//
//    BasicTester tester2(M,A);
//
//    auto suite2 = tester2.getSuite(4);
//
//    suite2.print();
//
//    std::cout<<" Number of tests: "<<suite2.countSequences()<<"\n";
//


    return 0;
}
