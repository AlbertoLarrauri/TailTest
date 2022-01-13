
//
// Created by lazhares on 12/01/2022.
//

#include "containment_relation.h"

using namespace TailTest;

ContainmentRelation TailTest::computeLookAheadFwdSimRel(NFA &A, uint32_t depth) {
    ContainmentRelation rel(A.size(), true);
    bool refined;
    do {
        refined = false;
        for (uint32_t a = 0; a < A.size(); ++a) {
            for (uint32_t b = 0; b < A.size(); ++b) {
                if (!rel.contains(a, b) || a == b) continue;
                LookAheadSimGame game(rel, A, a, b);
                auto val = game.play(depth);
                if (!val) {
                    refined = true;
                    rel.setContainment(a, b, false);
                }
            }
        }

    } while (refined);


    makeTransitive(rel);


    return rel;
}

void TailTest::makeTransitive(ContainmentRelation &rel) {
    std::vector<NumVec> down_sets(rel.size());
    std::vector<NumVec> up_sets(rel.size());

    for (uint32_t a = 0; a < rel.size(); ++a) {
        for (uint32_t b = 0; b < rel.size(); ++b) {
            if (!rel.contains(a, b) || a == b) continue;
            down_sets[a].push_back(b);
            up_sets[b].push_back(a);
        }
    }

    for(uint32_t b=0; b<rel.size(); ++b){
        for(auto c:down_sets[b]){
            for(auto a:up_sets[b]){
                if(rel.contains(a,c)) continue;
                rel.setContainment(a,c, true);
                up_sets[c].push_back(a);
                down_sets[a].push_back(c);
            }
        }
    }

}

bool LookAheadSimGame::play(uint32_t depth) {
    input_seq.push_back(0);

    while (true) {
        auto curr_in = input_seq.back();
        if (curr_in >= A.numberOfInputs()) {
            pop();
            if (input_seq.empty()) return true;
            ++input_seq.back();
            continue;
        }

        auto &curr_duplicator_set = duplicator_sets.back();
        auto &curr_spoiler_set = spoiler_sets.back();
        std::unordered_map<uint32_t, bool> next_spoiler_set;

        uint32_t count = 0;
        for (auto &[a, val]: curr_spoiler_set) {
            for (auto b: A.getSuccs(a, curr_in)) {
                if (!next_spoiler_set[b] && val) {
                    ++count;
                    next_spoiler_set[b] = true;
                }
            }
        }

        if (count == next_spoiler_set.size()) {
            pop();
            if (input_seq.empty()) return true;
            ++input_seq.back();
            continue;
        }

        NumSet next_duplicator_set = A.propagate(curr_duplicator_set, curr_in);

        if (next_duplicator_set.empty()) return false;

        for (auto&[b, val]: next_spoiler_set) {
            if (val) continue;
            for (auto a: next_duplicator_set) {
                if (rel.contains(a, b)) {
                    val = true;
                    ++count;
                    break;
                }
            }

            if (count == next_spoiler_set.size()) break;

        }

        if (count == next_spoiler_set.size()) {
            pop();
            if (input_seq.empty()) return true;
            ++input_seq.back();
            continue;
        }

        /// Duplicator has not been able to win yet

        if (input_seq.size() == depth) return false;

        input_seq.push_back(0);
        duplicator_sets.push_back(std::move(next_duplicator_set));
        spoiler_sets.push_back(std::move(next_spoiler_set));

    }

    assert(false);

    return true;
}
