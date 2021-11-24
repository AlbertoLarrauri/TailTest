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

    private:

        IncompatibilityData data;
        const DFSM& M;
        const NFA& A;

        typedef boost::container::flat_set<uint32_t> flat_set;
        typedef boost::container::flat_map<uint32_t,flat_set> flat_map;
        struct Frame{
            typedef flat_set::const_iterator Iterator;
            Iterator it;
            flat_set inputs;
            flat_set A_set;
            uint32_t s;
            uint32_t node;
        };

        std::vector<uint32_t > curr_seq;
        std::vector<Frame> frames;
        std::vector<flat_set> back_propagation;


        void step();

        void pop();

    public:

        InputTree getSuite(uint32_t k);

    };
}

#endif //TAILTEST_BASIC_H
