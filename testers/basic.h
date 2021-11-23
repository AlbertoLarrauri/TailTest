//
// Created by llarrauriborroto on 23/11/2021.
//

#ifndef TAILTEST_BASIC_H
#define TAILTEST_BASIC_H

#include "../machines/input_tree.h"
#include "../machines/machines.h"
#include "../machines/incompatibility_data.h"
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>

namespace TailTest {
    class BasicTester {

//        IncompatibilityData data;
//        const DFSM& M;
//        const NFA& A;
    private:
        typedef boost::container::flat_set<uint32_t> flat_set;
        typedef boost::container::flat_map<uint32_t,flat_set> flat_map;
        struct Frame{
            typedef flat_set::const_iterator Iterator;
            Iterator it;
            flat_set inputs;
            flat_set A_set;
            flat_map r_succs;
            uint32_t s;
            uint32_t node;
        };

        std::vector<uint32_t > current_seq;
        std::vector< Frame > frames;

        void step(uint32_t in);

        void pop();

    public:

        InputTree getSuite(const DFSM& M, const NFA& A, uint32_t k);


    };
}

#endif //TAILTEST_BASIC_H
