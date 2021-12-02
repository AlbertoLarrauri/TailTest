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
#include <unordered_set>
#include <unordered_map>

namespace TailTest {
    class BasicTester {

    private:

        IncompatibilityData data;
        const DFSM& M;
        const NFA& A;
        InputTree suite;


        typedef std::vector<uint32_t> num_vector;
        typedef std::vector<std::unordered_set<uint32_t>> down_set;


        struct Certificate{
            uint32_t a;
            num_vector positions;
        };

        struct Frame{
            std::unordered_map<uint32_t, Certificate*> node_dict;
            num_vector inputs;
            std::vector<Certificate> certificates;
            uint32_t in_it;
            uint32_t s;
        };



        num_vector current_seq;

        std::vector< Frame > frames;


        void step();

        void pop();

        void pop_and_prepare();

        std::vector<std::unordered_set<uint32_t>> backPropagate(uint32_t a);

        uint32_t getScore(const down_set& back_propagation);

        void exploitCertificate(const Certificate& cert);


    public:

        BasicTester(const DFSM& M, const NFA& A);

        const InputTree& getSuite(uint32_t k);


    };
}

#endif //TAILTEST_BASIC_H
