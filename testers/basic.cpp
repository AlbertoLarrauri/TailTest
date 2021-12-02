//
// Created by llarrauriborroto on 23/11/2021.
//

#include "basic.h"
#include <unordered_set>
#include <algorithm>

using namespace TailTest;

void BasicTester::step() {

    auto& curr_frame=frames.back();
    auto in_it=curr_frame.in_it;
    auto in= curr_frame.inputs[in_it];


    current_seq.push_back(in);

    std::unordered_map<uint32_t,Certificate*> succ_dict;
    std::unordered_set<uint32_t> succ_inputs;




    for(auto& [a,ptr]:curr_frame.node_dict){
        for(auto b:A.getSuccs(a,in)){
            auto& cert_ptr=succ_dict[b];
            if(cert_ptr==nullptr) {
                cert_ptr = ptr;
            }

            for(auto next_in:A.getSymbols(b)){
                succ_inputs.insert(next_in);
            }
        }
    }


    auto next_s=M.getSucc(curr_frame.s,in);
    Frame next_frame={std::move(succ_dict),
                      num_vector(succ_inputs.begin(), succ_inputs.end()),
                      {},
                      0,
                      next_s};

}

void BasicTester::pop() {
    frames.pop_back();
    current_seq.pop_back();
}


void BasicTester::pop_and_prepare() {
    bool valid_it=false;
    while(!frames.empty()){
        pop();
        auto& curr_frame=frames.back();
        auto& it=curr_frame.in_it;
        auto max_it=curr_frame.inputs.size();
        ++it;
        if(it<max_it) return;
    }

}



/// a belongs to frames.back().A_set

std::vector<std::unordered_set<uint32_t>> BasicTester::backPropagate(uint32_t a) {
    down_set backpropagation;
    backpropagation.push_back({a});

    for(size_t i=1; i<=current_seq.size(); ++i){
        auto idx =  current_seq.size() - i;
        auto in = current_seq[idx];
        auto& source_node_dict=frames[idx].node_dict;

        backpropagation.emplace_back();
        auto& source_nodes=backpropagation[i];
        auto& target_nodes=backpropagation[i - 1];

        for(auto target:target_nodes){
            for(auto source:A.getRSuccs(target,in)){
                if(source_node_dict.count(source)){
                    source_nodes.insert(source);
                }
            }
        }
    }
    return backpropagation;
}


uint32_t BasicTester::getScore(const BasicTester::down_set &back_propagation) {
    num_vector scores(A.size(), 0);
    for(const auto& A_set:back_propagation){
        for(auto a:A_set){
            ++scores[a];
        }
    }

    return *(std::max_element(scores.begin(), scores.end()));
}

void BasicTester::exploitCertificate(const Certificate& cert) {
    auto& positions=cert.positions;
    auto& a=cert.a;

    num_vector state_vec;

    {
        std::unordered_set<uint32_t> state_set;
        for (auto pos: positions) {
            auto s = frames[pos].s;
            state_set.insert(s);
        }

        state_vec.insert(state_vec.end(),state_set.begin(),state_set.end());
    }

    for(auto pos:positions){
        auto s = frames[pos].s;
        for(auto t:state_vec){
            if(!data.areCompatible(s,a,t,a)){
                auto dist_seq=data.distinguishingSequence(s,a,t,a);
                num_vector seq_to_add(current_seq.begin(), current_seq.begin()+ pos);
                seq_to_add.insert(seq_to_add.end(),dist_seq.begin(),dist_seq.end());
                suite.addSequence(seq_to_add);
            }
        }
    }

}

BasicTester::BasicTester(const DFSM &_M, const NFA &_A):
M(_M),
A(_A),
suite(A.numberOfInputs()),
data(_M,_A)
{}

const InputTree &BasicTester::getSuite(uint32_t k) {

    return suite;
};






