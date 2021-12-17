//
// Created by lazhares on 21/11/2021.
//

#ifndef TAILTEST_INPUT_TREE_H
#define TAILTEST_INPUT_TREE_H

#include<vector>
#include<cstdint>
#include <cassert>

namespace TailTest {

    class InputTree {
    private:
        std::vector<size_t> impl;
        const uint32_t in_alphabet_size;
        size_t max_vertex=0;

        inline size_t toID(size_t node, uint32_t in) const {
            return (node)*size_t(in_alphabet_size) + size_t(in);
        }

    public:

        static const uint32_t NO_SUCC = 0xffffffff;

        InputTree(uint32_t in_size): in_alphabet_size(in_size){
            impl.resize(in_alphabet_size,NO_SUCC);
        }

        size_t addSequence(const std::vector<uint32_t>& seq, size_t vertex= 0);

        size_t addSymbol(uint32_t in, size_t vertex);

        size_t countSequences() const;

        /// NOT CORRECT
        inline size_t countSymbols() const{
            return max_vertex - 1;
        }

        inline size_t getNext(size_t vertex, uint32_t in) const {
            assert(vertex != NO_SUCC);
            return impl.at(toID(vertex, in));
        }

        inline size_t getNext(size_t vertex, const std::vector<uint32_t> &seq) const {
            for(auto in:seq){
                vertex = getNext(vertex, in);
            }
            return vertex;
        }

        void print() const;

        inline size_t size() const {
            return max_vertex + 1;
        }


    };

}

#endif //TAILTEST_INPUT_TREE_H
