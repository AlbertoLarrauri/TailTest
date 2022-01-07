//
// Created by llarrauriborroto on 21/12/2021.
//

#ifndef TAILTEST_CONTAINMENT_RELATION_H
#define TAILTEST_CONTAINMENT_RELATION_H

#include "machines.h"

namespace TailTest {
    class ContainmentRelation {

    private:
        const uint32_t A_size;
        std::vector<bool> impl;

        inline uint32_t pairToID(uint32_t a, uint32_t b) const {
            return A_size * a + b;
        }

    public:

        inline ContainmentRelation(uint32_t size) :
                A_size(size), impl(size * size) {
            for(uint32_t a=0; a < A_size; ++a){
                setContainment(a,a,true);
            }
        }

        inline bool contains(uint32_t a, uint32_t b) const {
            return impl[pairToID(a, b)];
        }

        void setContainment(uint32_t a, uint32_t b, bool val) {
            impl[pairToID(a, b)] = val;
        }

    };

}
#endif //TAILTEST_CONTAINMENT_RELATION_H
