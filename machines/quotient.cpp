//
// Created by lazhares on 22/11/2021.
//

#include "quotient.h"

using namespace TailTest;

Quotient::Quotient(const DFSM &_M, const NFA &_A, const IncompatibilityData &data) :
        M(_M),
        A(_A),
        pairs_to_class_ID(M.size() * A.size(), 0),
        classes_per_state(A.size()),
        reachable_per_state(A.size()) {

    for (uint32_t a = 0; a < A.size(); ++a) {
        for (uint32_t s = 0; s < M.size(); ++s) {
            bool is_new = true;
            auto &classes = classes_per_state[a];

            for (uint32_t i = 0; i < classes.size(); ++i) {
                auto t = classes[i];
                if (data.areCompatible(s, a, t, a)) {
                    is_new = false;
                    std::cout << " In context " << a << " the states " << s << " and " << t << " are equivalent. \n";
                    pairs_to_class_ID[pairToID(s, a)] = i;
                    break;
                }

            }

            if (!is_new) continue;
            pairs_to_class_ID[pairToID(s, a)] = classes.size();
            classes.push_back(s);
        }

        auto &classes = classes_per_state[a];
        auto &reachable = reachable_per_state[a];
        reachable.resize(classes.size(), false);
    }


}

void Quotient::generateCover(InputTree &cover, std::vector<MacroState> &cover_macro_states) {

    assert(!has_reachability_data);

    /// Cover should have only the initial vertex
    assert(cover.size() == 1);

    assert(cover_macro_states.empty());

    has_reachability_data = true;


    cover_macro_states.push_back({{{0}, 0}});

    setReachable(0, 0);

    for (uint32_t i = 0; i < cover_macro_states.size(); ++i) {
        uint32_t node = i;


        for (uint32_t in = 0; in < A.numberOfInputs(); ++in) {
            const auto &A_set = cover_macro_states[i].A_set;
            const auto &s = cover_macro_states[i].s;

            if (cover.getNext(node, in) != InputTree::NO_SUCC) continue;
            auto next_A_set = A.propagate(A_set, in);
            auto next_s = M.getSucc(s, in);

            /// REFACTOR IF OTHER SETS ARE USED

            auto it = next_A_set.begin();
            while (it != next_A_set.end()) {
                auto next_a = *it;
                if (isReachable(next_s, next_a)) {
                    it = next_A_set.erase(it);
                } else {
                    setReachable(next_s, next_a);
                    ++it;
                }

            }

            if (next_A_set.empty()) continue;

            cover.addSymbol(in, node);
            cover_macro_states.push_back({std::move(next_A_set), next_s});
        }

    }


}

void Quotient::generateMinimalCover(InputTree &cover,
                                    MinimalCoverData &cover_data,
//                                    std::vector<MacroState> &cover_data,
//                                    std::unordered_map<uint32_t, uint32_t> &states_to_vertex,
//                                    std::vector<bool> &is_vertex_in_cover,
                                    const ContainmentRelation &rel) {
    assert(cover.size() == 1);
    assert(!has_reachability_data);

    std::vector<MacroState> &cover_macro_states= cover_data.cover_macro_states;
    std::unordered_map<uint32_t, uint32_t> &states_to_vertex = cover_data.states_to_vertex;
    std::vector<bool> &is_vertex_in_cover= cover_data.is_vertex_in_cover;

    cover_macro_states.clear();
    states_to_vertex.clear();
    is_vertex_in_cover.clear();

    /// Generate reachability data

    {
        MacroState initial_macro_state = {{0}, 0};
        std::vector<MacroState> stack = {initial_macro_state};
        for (uint32_t i = 0; i < stack.size(); ++i) {
            auto curr_s = stack[i].s;
            auto &curr_A_set = stack[i].A_set;
            bool new_reached = false;
            for (auto a: curr_A_set) {
                if (!isReachable(curr_s, a)) {
                    new_reached = true;
                    setReachable(curr_s, a);
                }
            }
            if (!new_reached) continue;
            for (uint32_t in = 0; in < A.numberOfInputs(); ++in) {
                stack.push_back({A.propagate(stack[i].A_set, in),
                                 M.getSucc(curr_s, in)});
            }
        }


    }


    /// Containment lists for A's states

    std::vector<NumVec> A_down_sets(A.size());
    std::vector<NumVec> A_up_sets(A.size());


    for (uint32_t a = 0; a < A.size(); ++a) {
        for (uint32_t b = 0; b < A.size(); ++b) {
            if (rel.contains(a, b)) {
                A_down_sets[a].push_back(b);
                A_up_sets[b].push_back(a);
            }
        }
    }

    /// Containment lists for locations

    std::vector<std::vector<std::pair<uint32_t, uint32_t >>> prod_down_sets(A.size() * M.size());
    std::vector<std::vector<std::pair<uint32_t, uint32_t >>> prod_up_sets(A.size() * M.size());

    for (uint32_t a = 0; a < A.size(); ++a) {

        auto &down_set = A_down_sets[a];
        auto &up_set = A_up_sets[a];

        for (auto s: getClasses(a)) {

            if (!isReachable(s, a)) continue;

            for (auto b: A_down_sets[a]) {
                auto t = getClass(s, b);
                if (!isReachable(t, b)) continue;
                prod_down_sets[pairToID(s, a)].emplace_back(t, b);
                prod_up_sets[pairToID(t, b)].emplace_back(s, a);
            }
        }
    }

    /// Computing a core

    std::vector<std::pair<uint32_t, uint32_t >> core;
    std::vector<bool> is_in_core(A.size() * M.size());

    for (uint32_t a = 0; a < A.size(); ++a) {
        for (auto s: getClasses(a)) {
            if (prod_up_sets[pairToID(s, a)].size() == 1) {
                core.emplace_back(s, a);
                is_in_core[pairToID(s, a)] = true;
            }
        }
    }


    std::cout << " Basis : ";
    for (auto[s, a]: core) {
        std::cout << "( " << s << ", " << a << " ), ";
    }
    std::cout << "\n";

    /// Computing cover


    std::vector<bool> already_reached(A.size() * M.size());
    uint32_t count = 0;
    if (is_in_core[0]) count = 1;

///    std::unordered_map<uint32_t, uint32_t> &states_to_vertex,
///    std::vector<bool> &is_vertex_in_cover,

    cover_macro_states = {{{0}, 0}};
    already_reached[0] = true;


    for (uint32_t i = 0; i < cover_macro_states.size(); ++i) {
        uint32_t node = i;


        for (uint32_t in = 0; in < A.numberOfInputs(); ++in) {
            const auto &A_set = cover_macro_states[i].A_set;
            const auto &s = cover_macro_states[i].s;

            if (cover.getNext(node, in) != InputTree::NO_SUCC) continue;
            auto next_A_set = A.propagate(A_set, in);
            auto next_s = M.getSucc(s, in);

            /// REFACTOR IF OTHER SETS ARE USED

            auto it = next_A_set.begin();
            while (it != next_A_set.end()) {
                auto next_a = *it;
                auto t = getClass(next_s, next_a);
                if (already_reached[pairToID(t, next_a)]) {
                    it = next_A_set.erase(it);
                } else {
                    already_reached[pairToID(t, next_a)] = true;
                    if (is_in_core[pairToID(t, next_a)]) {
                        states_to_vertex[pairToID(t, next_a)] = cover.size();
                        ++count;
                    }
                    ++it;
                }

            }

            if (next_A_set.empty()) continue;
            cover.addSymbol(in, node);
            cover_macro_states.push_back({std::move(next_A_set), next_s});
            if (count == core.size()) goto COVER_COMPUTED;
        }

    }

    COVER_COMPUTED:

    assert(count == core.size());

    is_vertex_in_cover.resize(cover.size());
    for (auto[s, a]: core) {
        auto vertex = states_to_vertex[pairToID(s, a)];
        is_vertex_in_cover[vertex] = true;
        for (auto[t, b]: prod_down_sets[pairToID(s, a)]) {
            auto id = pairToID(t, b);
            if (!states_to_vertex.count(id)) {
                states_to_vertex[id] = vertex;
            } else if (states_to_vertex[id] > vertex) {
                states_to_vertex[id] = vertex;
            }
        }
    }


    for (auto[s, a]: core) {
        std::cout << " Location (" << s << ", " << a << ") reached in vertex " << states_to_vertex[pairToID(s, a)]
                  << "\n";

    }

    std::cout << cover.getNext(0, {1, 1, 1}) << "\n";

}
