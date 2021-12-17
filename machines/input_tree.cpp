//
// Created by lazhares on 21/11/2021.
//

#include <algorithm>
#include "input_tree.h"
#include "iostream"

using namespace TailTest;

size_t InputTree::addSymbol(uint32_t in, size_t vertex) {
    auto next= getNext(vertex, in);
    if(next!=NO_SUCC) return next;
    ++max_vertex;
    impl.resize((max_vertex + 1) * in_alphabet_size, NO_SUCC);
    impl[toID(vertex, in)] = max_vertex;
    return max_vertex;
}

size_t InputTree::addSequence(const std::vector<uint32_t> &seq, size_t vertex) {
    auto it = seq.begin();
    size_t curr_node = vertex;
    while (it != seq.end()) {
        auto next = impl[toID(curr_node, *it)];
        if (next == NO_SUCC) break;
        curr_node = next;
        ++it;
    }

    if (it == seq.end()) return curr_node;
    size_t to_include = seq.end() - it;
    impl.resize((max_vertex + to_include + 1) * in_alphabet_size, NO_SUCC);

    while (it != seq.end()) {
        ++max_vertex;
        impl[toID(curr_node, *it)] = max_vertex;
        curr_node = max_vertex;
        ++it;
    }
    return curr_node;
}

size_t InputTree::countSequences() const {
    size_t count = 0;
    for (size_t node = 0; node <= max_vertex; ++node) {
        bool empty = true;
        for (uint32_t in = 0; in < in_alphabet_size; ++in) {
            if (impl[toID(node, in)] != NO_SUCC) {
                empty = false;
                break;
            }
        }
        if (empty) ++count;
    }
    return count;
}

void InputTree::print() const {

    std::vector<uint32_t> in_stack;
    std::vector<size_t> node_stack;
    node_stack.push_back(0);
//    in_stack.push_back(0);


    while (!node_stack.empty()) {
        bool is_leave = false;


        PROPAGATE_NODE:
        {
            auto curr_node = node_stack.back();
            for (uint32_t in = 0; in < in_alphabet_size; ++in) {
                auto succ = getNext(curr_node, in);
                if (succ != NO_SUCC) {
                    node_stack.push_back(succ);
                    in_stack.push_back(in);
                    goto PROPAGATE_NODE;
                }
            }
        };

        PRINT:

        for (auto in: in_stack) {
            std::cout << in << "-";
        }
        std::cout << "\n";


        POP:

        node_stack.pop_back();

        while (!node_stack.empty()) {

            auto curr_node = node_stack.back();
            auto succ = NO_SUCC;
            auto &curr_in = in_stack.back();
            ++curr_in;

            for (; curr_in < in_alphabet_size; ++curr_in) {

                succ = getNext(curr_node, curr_in);
                if (succ != NO_SUCC) {
                    node_stack.push_back(succ);
                    goto PROPAGATE_NODE;
                }
            }

            in_stack.pop_back();
            node_stack.pop_back();
        }

    }

}


