//
// Created by lazhares on 22/11/2021.
//

#ifndef TAILTEST_QUOTIENT_H
#define TAILTEST_QUOTIENT_H

#include "incompatibility_data.h"
#include "input_tree.h"
#include "containment_relation.h"

namespace TailTest {

    struct MinimalCoverData;

    class Quotient {

    private:
        typedef std::vector<uint32_t> NumVec;

        const DFSM& M;
        const NFA& A;

        bool has_reachability_data = false;


        NumVec pairs_to_class_ID;
        std::vector<NumVec> classes_per_state;
        std::vector<std::vector<bool>> reachable_per_state;

        inline size_t pairToID(uint32_t s, uint32_t a) const {
            return size_t(M.size()) * size_t(a) + size_t(s);
        }

    public:

        Quotient(const DFSM& M, const NFA& A, const IncompatibilityData& data);


        inline uint32_t getClass(uint32_t s, uint32_t a) const {
            auto pair = pairToID(s, a);
            auto class_ID = pairs_to_class_ID[pair];
            return classes_per_state[a][class_ID];
        }

        inline bool isReachable(uint32_t s, uint32_t a) const{
//            assert(has_reachability_data);
            auto pair = pairToID(s, a);
            auto class_ID = pairs_to_class_ID[pair];
            return reachable_per_state[a][class_ID];
        }

        inline void setReachable(uint32_t s, uint32_t a){
            auto pair = pairToID(s, a);
            auto class_ID = pairs_to_class_ID[pair];
            reachable_per_state[a][class_ID]=true;
        }


        inline const NumVec& getClasses(uint32_t a) const{
            return classes_per_state[a];
        }


        void generateCover(InputTree& cover, std::vector<MacroState>& cover_macro_states);

        void generateMinimalCover(InputTree & cover,
                                    MinimalCoverData& cover_data,
//                                  std::vector<MacroState> &cover_data,
//                                  std::unordered_map<uint32_t ,uint32_t >& states_to_vertex,
//                                  std::vector<bool>& is_vertex_in_cover,
                                  const ContainmentRelation& rel);

    };

    struct MinimalCoverData{
        std::vector<MacroState> cover_macro_states;
        std::unordered_map<uint32_t ,uint32_t > states_to_vertex;
        std::vector<bool> is_vertex_in_cover;
    };


}

#endif //TAILTEST_QUOTIENT_H
