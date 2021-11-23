//
// Created by lazhares on 21/11/2021.
//

#ifndef TAILTEST_INPUT_TREE_H
#define TAILTEST_INPUT_TREE_H

#include<vector>
#include<cstdint>

namespace TailTest {

    class InputTree {
    private:
        std::vector<size_t> impl;
        const uint32_t in_alphabet_size;
        size_t max_node=0;

        inline size_t toID(size_t node, uint32_t in){
            return node*size_t(in_alphabet_size) + size_t(in);
        }

    public:

        InputTree(uint32_t in_size): in_alphabet_size(in_size){
            impl.resize(in_alphabet_size,0);
        }

        size_t addSequence(const std::vector<uint32_t>& seq, size_t node= 0);

        size_t countSequences();

        size_t countSymbols();

        inline size_t getNext(size_t node, uint32_t in){
            return impl.at(toID(node,in));
        }

        inline size_t getNext(size_t node, const std::vector<uint32_t>& seq){
            for(auto in:seq){
                node= getNext(node,in);
            }
            return node;
        }

        void print();


    };

}

#endif //TAILTEST_INPUT_TREE_H
