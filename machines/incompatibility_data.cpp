//
// Created by llarrauriborroto on 16/11/2021.
//

#include "incompatibility_data.h"
#include <deque>
#include "../utils.h"

using namespace TailTest;


IncompatibilityData::IncompatibilityData(const TailTest::DFSM &M, const TailTest::NFA &A):
m_size(M.size()),
a_size(A.size()),
COMPAT(M.numberOfInputs()),
NO_SUCC(M.size()*A.size()) {
    typedef boost::container::flat_set<uint32_t> flat_set;
    std::vector<flat_set> r_symbols(a_size *m_size);


    for (uint32_t s = 0; s < m_size; ++s) {
        for (uint32_t a = 0; a < a_size; ++a) {
            r_symbols.at(pairToID(s, a)) = intersect(M.getRSymbols(s), A.getRSymbols(a));
        }
    }


    auto size_impl = ((NO_SUCC) * (NO_SUCC + 1)) / 2;
    impl.resize(size_impl);

    std::vector<bool> processed(size_impl, false);


    typedef std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> StateTuple;


    std::deque<StateTuple> queue;


    for (uint32_t s = 0; s < m_size; s++) {
        for (uint32_t t = s + 1; t < m_size; t++) {
            for (uint32_t a = 0; a < a_size; a++) {
                for (uint32_t b = 0; b < a_size; b++) {

                    auto current_tuple = std::make_tuple(s, a, t, b);
                    auto pair1 = pairToID(s, a);
                    auto pair2 = pairToID(t, b);
                    auto entry = toEntry(pair1, pair2);

                    auto &symbols_a = A.getSymbols(a);
                    auto &symbols_b = A.getSymbols(b);
                    auto it_b = symbols_b.begin();


                    for (auto it_a = symbols_a.begin(); it_a != symbols_a.end(); ++it_a) {
                        // This should be optimized;
                        while (*it_b < *it_a) {
                            ++it_b;
                        }

                        if (it_b == symbols_b.end()) break;
                        if (*it_a < *it_b) continue;

                        auto in = *it_a;
                        if (M.getOut(s, in) != M.getOut(t, in)) {
                            processed[entry] = true;
                            queue.emplace_back(s, a, t, b);
                            impl[entry] = {NO_SUCC, in};
                        }
                    }
                }
            }
        }
    }

    while (!queue.empty()) {
        auto[s, a, t, b]=queue.front();
        queue.pop_front();
        auto pair0 = pairToID(s, a);
        auto pair1 = pairToID(t, b);
        auto &r_symbols0 = r_symbols.at(pair0);
        auto &r_symbols1 = r_symbols.at(pair1);
        if (r_symbols0.empty() || r_symbols1.empty()) continue;


        flat_set::const_iterator its[2] = {r_symbols0.begin(), r_symbols1.begin()};
        flat_set::const_iterator ends[2] = {r_symbols0.end(), r_symbols1.end()};
        int current = 0;
        if (*its[1] < *its[0]) current = 1;
        const uint32_t &NO_IN = COMPAT;
        auto next = [&its, &ends, &current, &NO_IN]() {
            while (*its[current] < *its[1 - current]) {
                ++its[current];
                if (its[current] == ends[current]) {
                    return NO_IN;
                }
            }
            if(*its[current]==*its[1-current]){

            }


        };

    }

}
