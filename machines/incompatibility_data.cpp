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
NO_SUCC(((M.size()*A.size())*(M.size()*A.size()+1))/2) {
    typedef boost::container::flat_set<uint32_t> flat_set;

    std::vector<flat_set> r_symbols(a_size*m_size);


    for (uint32_t s = 0; s < m_size; ++s) {
        for (uint32_t a = 0; a < a_size; ++a) {
//            std::cout<<"s: "<<s<<", a: "<<a<<"\n";
//            printSequence(M.getRSymbols(s));
//
//            printSequence(A.getRSymbols(a));
//
//            std::cout<<"s: "<<s<<", a: "<<a<<"\n";
//
//            std::cout<<"R_symbols size: "<< r_symbols.size()<<", Current: "<<pairToID(s,a)<<"\n";

            r_symbols.at(pairToID(s, a)) = intersect(M.getRSymbols(s), A.getRSymbols(a));
        }
    }


    auto size_impl = NO_SUCC;

    impl.resize(size_impl, {NO_SUCC,COMPAT});

    std::vector<bool> processed(size_impl, false);
    typedef std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> StateTuple;


    std::deque<StateTuple> queue;


    for (uint32_t s = 0; s < m_size; ++s) {
        for (uint32_t t = s + 1; t < m_size; ++t) {
            for (uint32_t a = 0; a < a_size; ++a) {
                for (uint32_t b = 0; b < a_size; ++b) {

                    auto current_tuple = std::make_tuple(s, a, t, b);
                    auto pair1 = pairToID(s, a);
                    auto pair2 = pairToID(t, b);
                    auto entry = toEntry(pair1, pair2);

                    auto &symbols_a = A.getSymbols(a);
                    auto &symbols_b = A.getSymbols(b);

                    IntersectionIterator it(symbols_a,symbols_b);
                    while (it.inRange()){

                        auto in=it.next();

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

    std::cout<<"Queue: \n";
    for(auto tpl:queue){
        auto [s,a,t,b]=tpl;
        std::cout<<"s: " << s<<", a: "<<a<<", t: "<<t<<", b: "<<b<<"\n";
    }

    while (!queue.empty()) {
        auto[s, a, t, b]=queue.front();

        queue.pop_front();
        auto pair1 = pairToID(s, a);
        auto pair2 = pairToID(t, b);
        auto entry = toEntry(pair1, pair2);
        auto &r_symbols1 = r_symbols.at(pair1);
        auto &r_symbols2 = r_symbols.at(pair2);

        IntersectionIterator it(r_symbols1, r_symbols2);
        while(it.inRange()){
            auto r_in=it.next();
            for(auto r_s:M.getRSuccs(s,r_in)){
                for(auto r_t:M.getRSuccs(t,r_in)){
                    for (auto r_a: A.getRSuccs(a, r_in)) {
                        for (auto r_b: A.getRSuccs(b,r_in)){
                            auto r_entry = toEntry(r_s,r_a,r_t,r_b);
                            if(processed[r_entry]) continue;
                            processed[r_entry]=true;
                            queue.emplace_back(r_s,r_a,r_t,r_b);
                            impl[r_entry]={entry,r_in};
                        }
                    }
                }
            }
        }


    }

}
