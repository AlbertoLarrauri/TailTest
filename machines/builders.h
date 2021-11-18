//
// Created by lazhares on 18/02/2021.
//

#ifndef PMIN_BUILDERS_H
#define PMIN_BUILDERS_H


#include <memory>
#include <unordered_set>
#include <random>
#include "machines.h"

namespace TailTest {

    OFA_old buildOFA(const DFSM_old &driver, const DFSM_old &driven);

    OFA_old buildHeuristicOFA(const DFSM_old &driver, const DFSM_old &driven);

    DFSM_old buildCascadeDFSM(const DFSM_old &driver, const DFSM_old &driven);

    int getStateCoverage(const DFSM_old &driver, const DFSM_old &driven);

    DFSM_old makeRandomDFSM(int in_size, int out_size, int size, int padding,
                            std::minstd_rand0& generator);

    DFSM_old makeRandomDFSM(int in_size, int out_size, int size, int padding=0);

    class ModularDFSMBuilder{
    private:
        int in_size=0;
        int out_size=0;
        int size=0;
        int number_of_bags=1;
        double extra_edge_probability=0;
        int padding=1;

    public:

        inline ModularDFSMBuilder(int _in_size, int _out_size, int _size):
        in_size(_in_size),
        out_size(_out_size),
        size(_size){}

        inline void setNumberOfInputs(int _number){
            in_size=_number;
        }

        inline void setNumberOfOutputs(int _number){
            out_size=_number;
        }
        void setSize(int _number){
            size=_number;
        }
        void setNumberOfBags(int _number){
            number_of_bags=_number;
        }

        void setPadding(int _number){
            padding=_number;
        }
        void setEdgeProbability(float _number){
            extra_edge_probability=_number;
        }

        DFSM_old getDFSM();

    };

}


#endif //PMIN_BUILDERS_H
