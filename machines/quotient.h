//
// Created by lazhares on 22/11/2021.
//

#ifndef TAILTEST_QUOTIENT_H
#define TAILTEST_QUOTIENT_H

#include "incompatibility_data.h"
namespace TailTest {
    class Quotient {

    private:
        typedef std::tuple<uint32_t ,uint32_t > Pair;

        const uint32_t m_size;
        const uint32_t a_size;

        std::vector<Pair> impl;
        std::vector<std::vector<Pair>> classes_per_state;
        std::vector<Pair> classes;

        inline size_t pairToID(uint32_t s, uint32_t a) {
            return size_t(m_size) * size_t(a) + size_t(s);
        }

        inline Pair iDToPair(size_t id) {
            uint32_t s = id % m_size;
            uint32_t a = id / m_size;
            return {s, a};
        }

    public:

        Quotient(DFSM M, NFA A, IncompatibilityData data);

        Pair getClass(uint32_t s, uint32_t a);

        const std::vector<Pair>& getClasses(uint32_t a);


    };

}

#endif //TAILTEST_QUOTIENT_H
