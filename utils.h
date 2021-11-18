//
// Created by llarrauriborroto on 17/11/2021.
//

#ifndef TAILTEST_UTILS_H
#define TAILTEST_UTILS_H
#include <boost/container/flat_set.hpp>

namespace TailTest{

    inline boost::container::flat_set<uint32_t> intersect(const boost::container::flat_set<uint32_t>& set0,
                                                          const boost::container::flat_set<uint32_t>& set1){

        typedef boost::container::flat_set<uint32_t> flat_set;
        flat_set intersection;
        if(set0.empty() || set1.empty()) return intersection;

        flat_set::const_iterator its[2]={set0.begin(),set1.begin()};
        flat_set::const_iterator ends[2]={set0.end(), set1.end()};
        int current=0;

        while(true){
            while(*its[current]<=*its[1-current]){
                if(*its[current]==*its[1-current]){
                    intersection.insert(intersection.end(),*its[current]);
                }


                ++its[current];
                if(its[current]==ends[current]){
                    return intersection;
                }

            }
            current=1-current;
        }
    }




};


#endif //TAILTEST_UTILS_H
