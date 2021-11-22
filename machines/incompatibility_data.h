//
// Created by llarrauriborroto on 16/11/2021.
//

#ifndef TAILTEST_INCOMPATIBILITY_DATA_H
#define TAILTEST_INCOMPATIBILITY_DATA_H


#include<iostream>
#include <vector>
#include <cassert>
#include <boost/container/flat_set.hpp>
#include "machines.h"

namespace TailTest {

    class IncompatibilityData {
    private:
        const uint32_t m_size;
        const uint32_t a_size;
        const uint32_t COMPAT;
        const size_t NO_SUCC;

        class Node {
        public:
            size_t next;
            uint32_t in;
        };


        typedef std::vector<Node> IMPL;

        IMPL impl;


        inline size_t pairToID(const uint32_t s, const uint32_t a) const {
            return size_t(m_size) * size_t(a) + size_t(s);
        }

        inline size_t toEntry(size_t state1, size_t state2) {
            if (state1 >= state2) {
                return (state1 * (state1 + 1)) / 2 + state2;
            }
            return (state2 * (state2 + 1)) / 2 + state1;
        };

        inline size_t toEntry(const uint32_t s, const uint32_t a, const uint32_t t, const uint32_t b){
            auto pair1= pairToID(s,a);
            auto pair2= pairToID(t,b);
            return toEntry(pair1,pair2);
        }



    public:

        IncompatibilityData(const DFSM &M, const NFA &A);

        inline bool areCompatible(uint32_t s, uint32_t a, uint32_t t, uint32_t b) {
            auto entry = toEntry(s,a,t,b);
            return (impl.at(entry).in == COMPAT);
        }


        inline std::vector<uint32_t> distinguishingSequence(uint32_t s, uint32_t a, uint32_t t, uint32_t b) {

            std::vector<uint32_t> sequence;
            sequence.reserve(m_size * a_size);

            auto entry = toEntry(s,a,t,b);
            assert(!(impl.at(entry).in== COMPAT));

            while (entry != NO_SUCC) {
                sequence.push_back(impl.at(entry).in);
                entry = impl.at(entry).next;
            }

            sequence.shrink_to_fit();
            return sequence;
        }

    };
}


#endif //TAILTEST_INCOMPATIBILITY_DATA_H
