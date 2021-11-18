//
// Created by llarrauriborroto on 13/01/2021.
//

#include "machines.h"


#include<vector>
#include <unordered_set>


using namespace TailTest;

void DFSM_old::print() const {
    std::cout << "State / Input --> Output / State \n";
    std::cout << "---------------------------------\n";
    for (uint32_t state = 0; state < size_impl; ++state) {
        for (uint32_t i = 0; i < in_alphabet_size; ++i) {
            std::cout << state << " / " << i << " --> " << getOut(state, i) << " / " << getSucc(state, i) << "\n";
        }
    }
}

void OFA_old::print() {
    std::cout << "State / Input --> Output / States \n";
    std::cout << "---------------------------------\n";
    for (uint32_t state = 0; state < size_impl; ++state) {
        for (uint32_t i = 0; i < in_alphabet_size; ++i) {
            std::cout << state << " / " << i << " --> ";
            if (!hasTransition(state, i)) {
                std::cout << "*\n";
            } else {
                const std::vector<uint32_t> &targets = getSuccs(state, i);
                std::cout << getOut(state, i) << " / [ " << targets[0];

                for (uint32_t j = 1; j < targets.size(); ++j) {
                    std::cout << ", " << targets[j];
                }
                std::cout << " ] \n";
            }
        }
    }
}

size_t OFA_old::numberOfTransitions() {
    size_t size=0;
    for(auto v:impl){
        if(!v.has_value()){
//            ++size;
        }else{
            size+=(*v).successors.size();
        }
    }
    return size;
}


bool TailTest::areEquivalent(const DFSM_old &A, const DFSM_old &B) {
    if (&A == &B) return true;
    if (A.numberOfInputs() != B.numberOfInputs()) return false;
    if (A.size() == 0 && B.size() == 0) return true;
    if (A.size() == 0 || B.size() == 0) return false;

    uint32_t inputs = A.numberOfInputs();
    uint32_t size1 = A.size();
    uint32_t size2 = B.size();
    auto ID = [size2](uint32_t state1, uint32_t state2) {
        return size2 * state1 + state2;
    };

    std::unordered_set<uint32_t> visited_states;
    std::vector<uint32_t> unexplored;
    unexplored.reserve(size1*size2);
    unexplored.push_back(0);
    visited_states.insert(0);

    while (!unexplored.empty()) {
        uint32_t pair = unexplored.back();
        unexplored.pop_back();

        uint32_t state1 = pair / size2;
        uint32_t state2 = pair % size2;


        for (uint32_t i = 0; i < inputs; ++i) {
            uint32_t o = A.getOut(state1, i);
            uint32_t t = B.getOut(state2, i);
            if (o != t) return false;
            uint32_t next1 = A.getSucc(state1, i);
            uint32_t next2 = B.getSucc(state2, i);
            uint32_t next_pair = ID(next1, next2);

            if (!visited_states.count(next_pair)) {
                visited_states.insert(next_pair);
                unexplored.push_back(next_pair);
            }

        }
    }

    return true;
}


