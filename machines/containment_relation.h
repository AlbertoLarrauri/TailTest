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

        inline ContainmentRelation(uint32_t size, bool def_value = false) :
                A_size(size), impl(size * size, def_value) {
            if (def_value) return;
            for (uint32_t a = 0; a < A_size; ++a) {
                setContainment(a, a, true);
            }
        }

        inline bool contains(uint32_t a, uint32_t b) const {
            return impl[pairToID(a, b)];
        }

        void setContainment(uint32_t a, uint32_t b, bool val) {
            impl[pairToID(a, b)] = val;
        }

        inline uint32_t size(){
            return A_size;
        }

    };

    class LookAheadSimGame {
        const NFA &A;
        const ContainmentRelation &rel;

        NumVec input_seq;
        std::vector<NumSet> duplicator_sets;
        std::vector<std::unordered_map<uint32_t, bool>> spoiler_sets;

        inline void pop() {
            input_seq.pop_back();
            duplicator_sets.pop_back();
            spoiler_sets.pop_back();
        }


    public:
        inline LookAheadSimGame(ContainmentRelation &_rel, NFA &_A,
                                uint32_t duplicator_init, uint32_t spoiler_init) :
                rel(_rel),
                A(_A),
                duplicator_sets({{duplicator_init}}),
                spoiler_sets({{{spoiler_init, false}}}) {

            assert(duplicator_init!=spoiler_init);
            assert(rel.contains(duplicator_init,spoiler_init));

        };

        bool play(uint32_t depth);

    };

    ContainmentRelation computeLookAheadFwdSimRel(NFA &A, uint32_t depth);
    void makeTransitive(ContainmentRelation& rel);

}
#endif //TAILTEST_CONTAINMENT_RELATION_H
