//
// Created by llarrauriborroto on 17/11/2021.
//

#ifndef TAILTEST_UTILS_H
#define TAILTEST_UTILS_H

#include <boost/container/flat_set.hpp>

namespace TailTest {

    inline boost::container::flat_set<uint32_t> intersect(const boost::container::flat_set<uint32_t> &set0,
                                                          const boost::container::flat_set<uint32_t> &set1) {

        typedef boost::container::flat_set<uint32_t> flat_set;
        flat_set intersection;
        if (set0.empty() || set1.empty()) return intersection;

        flat_set::const_iterator its[2] = {set0.begin(), set1.begin()};
        flat_set::const_iterator ends[2] = {set0.end(), set1.end()};
        int current = 0;

        while (true) {
            while (*its[current] <= *its[1 - current]) {
                if (*its[current] == *its[1 - current]) {
                    intersection.insert(intersection.end(), *its[current]);
                }


                ++its[current];
                if (its[current] == ends[current]) {
                    return intersection;
                }

            }
            current = 1 - current;
        }
    }

    class IntersectionIterator {
    private:
        typedef boost::container::flat_set<uint32_t> flat_set;
        typedef boost::container::flat_set<uint32_t>::const_iterator iterator;
        iterator its[2];
        iterator ends[2];
        bool current;
        bool in_range=true;

        inline void update(){
            while (true) {
                while (*its[current] <= *its[1 - current]) {
                    if (*its[current] == *its[1 - current]) {
                        return;
                    }

                    ++its[current];
                    if (its[current] == ends[current]) {
                        in_range=false;
                        return;
                    }

                }
                current = 1 - current;
            }
        }

    public:

        IntersectionIterator(const flat_set &set0, const flat_set &set1) :
                its{set0.begin(), set1.begin()},
                ends{set0.end(), set1.end()} {

            if(its[0]== ends[0]|| its[1]==ends[1]){
                in_range= false;
                return;
            }
            current=(*its[1]<*its[0]);
            update();
        }


        inline bool inRange() {
            return in_range;
        }

        inline uint32_t next(){
            const uint32_t& value=*its[current];
            ++its[current];

            if(its[current]==ends[current]) {
                in_range = false;
            } else {
                current = 1 - current;
                update();
            }

            return value;
        }


    };

    template<class T>
    inline void printSequence(const std::vector<T>& seq){
        if(seq.empty()) {
            std::cout<<"\n";
            return;
        }
        for(size_t i=0; i+1<seq.size(); ++i){
            std::cout<<seq[i]<<" - ";
        }
        std::cout<<seq.back()<<"\n";
    }

    template<class T>
    inline void printSequence(const boost::container::flat_set<T>& seq){
        if(seq.empty()) {
            std::cout<<"\n";
            return;
        }
        auto it=seq.begin();
        for(; seq.end()-it>1; ++it){
            std::cout<<*it<<" - ";
        }
        std::cout<<*it<<"\n";
    }

};


#endif //TAILTEST_UTILS_H
