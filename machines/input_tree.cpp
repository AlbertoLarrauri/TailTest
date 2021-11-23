//
// Created by lazhares on 21/11/2021.
//

#include <algorithm>
#include "input_tree.h"
#include "iostream"

using namespace TailTest;

size_t InputTree::addSequence(const std::vector<uint32_t> &seq, size_t node) {
    auto it = seq.begin();
    size_t curr_node = node;
    while (it != seq.end()) {
        auto next = impl[toID(curr_node, *it)];
        if (next == 0) break;
        curr_node = next;
        ++it;
    }

    if (it == seq.end()) return curr_node;
    size_t to_include = seq.end() - it;
    impl.resize((max_node + to_include + 1) * in_alphabet_size, 0);

    while (it != seq.end()) {
        ++max_node;
        impl[toID(curr_node, *it)] = max_node;
        curr_node = max_node;
        ++it;
    }
    return curr_node;
}

size_t InputTree::countSequences() {
    size_t count = 0;
    for (size_t node = 0; node <= max_node; ++node) {
        bool empty = true;
        for (uint32_t in = 0; in < in_alphabet_size; ++in) {
            if (impl[toID(node, in)] != 0) {
                empty = false;
                break;
            }
        }
        if (empty) ++count;
    }
    return count;
}

size_t InputTree::countSymbols() {
    size_t count = 0;
    for (auto node: impl) {
        if (node != 0) ++count;
    }
    return count;
}

void InputTree::print() {

    std::vector<uint32_t> in_stack;
    std::vector<size_t> node_stack;
    node_stack.push_back(0);
    in_stack.push_back(0);
    bool is_leave = true;

    while (!(in_stack.empty())) {
        auto &in = in_stack.back();
        const auto &curr_node = node_stack.back();
        while (in != in_alphabet_size) {
            if (getNext(curr_node, in) == 0) {
                ++in;
            } else {
                break;
            }
        }
//        std::cout<<"Current node: "<<curr_node<<"\n Input: "<<in<<"\n";

        if (in == in_alphabet_size) {
            if (is_leave) {
                for (size_t i = 0; i + 1 < in_stack.size(); ++i) {
                    std::cout << in_stack[i] << "-";
                }
                std::cout << "\n";
            }
            is_leave = false;
            in_stack.pop_back();
            if (!in_stack.empty()) ++in_stack.back();
            node_stack.pop_back();
//            std::cout<<"Popping back \n";
            continue;
        }

//        std::cout<<"Pushing back node: "<<getNext(curr_node,in)<<"\n";
        node_stack.push_back(getNext(curr_node, in));
        in_stack.push_back(0);
        is_leave = true;
    }
}
