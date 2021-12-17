//
// Created by llarrauriborroto on 23/11/2021.
//

#ifndef TAILTEST_BASIC_H
#define TAILTEST_BASIC_H

#include "../machines/input_tree.h"
#include "../machines/machines.h"
#include "../machines/incompatibility_data.h"
#include "../machines/quotient.h"
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>
#include <unordered_set>
#include <unordered_map>
#include <utility>

namespace TailTest {
    class BasicTester {

    private:

        const DFSM& M;
        const NFA& A;
        IncompatibilityData data;
        Quotient quotient;

        InputTree suite;

        std::vector<MacroState> cover_data;


        typedef std::vector<NumSet> DownSet;


        struct Certificate{
            uint32_t witness;
            NumVec positions;
            bool exploited=false;
            inline void print() const{

                std::cout<<" Witness: "<<witness<<"\n Positions: ";
                for(auto i:positions){
                    std::cout<<i<<",";
                }
                std::cout<<"\n";
            }

        };

        struct Frame{
            std::unordered_map<uint32_t, Certificate*> node_dict;
            NumVec inputs;
            std::vector<std::unique_ptr<Certificate>> certificates;
            uint32_t in_it;
            uint32_t s;
            NumSet toDistinguish;

            Frame():node_dict(),inputs(),certificates(),in_it(),s(),toDistinguish(){
            }
        };



        NumVec current_seq;
        std::vector< size_t > vertex_seq;

        std::vector< Frame > frames;

        inline uint32_t pairToID(uint32_t s, uint32_t a) const {
            return a*M.size() + s;
        }

        inline std::pair<uint32_t ,uint32_t > IDToPair(uint32_t id) const{
            auto a = id / M.size();
            auto s = id % M.size();
            return std::make_pair(s,a);
        }



        void step();

        void pop();

        void popAndPrepare();

        std::vector<std::unordered_set<uint32_t>> backPropagate(uint32_t a);

        bool searchCertificates(uint32_t k);

        void saturateSequence(size_t initial_vertex);


//        uint32_t getScore(const DownSet& back_propagation);

        void exploitCertificate(Certificate& cert, size_t initial_vertex);

        inline void printFrames() const {
            std::cout<<"Frames: \n";
            for(auto &frame:frames){
                std::cout<<"[ ";
                for(auto [a,ptr]:frame.node_dict){
                    std::cout<<a<<",";
                }
                std::cout<<" ]\n";
            }

        }


    public:

        BasicTester(const DFSM& M, const NFA& A);

        const InputTree& getSuite(uint32_t k);


    };
}

#endif //TAILTEST_BASIC_H
