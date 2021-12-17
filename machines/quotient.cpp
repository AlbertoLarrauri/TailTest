//
// Created by lazhares on 22/11/2021.
//

#include "quotient.h"

using namespace TailTest;

Quotient::Quotient(const DFSM &_M, const NFA &_A, const IncompatibilityData &data):
M(_M),
A(_A),
pairs_to_class_ID(M.size()*A.size(),0),
classes_per_state(A.size()),
reachable_per_state(A.size())
{

    for(uint32_t a=0; a<A.size(); ++a){
        for(uint32_t s=0; s<M.size(); ++s){
            bool is_new=true;
            auto& classes = classes_per_state[a];

            for(uint32_t i=0; i < classes.size(); ++i){
                auto t=classes[i];
                if(data.areCompatible(s,a,t,a)){
                    is_new=false;
                    std::cout<<" In context "<<a<<" the states "<<s<<" and "<< t <<" are equivalent. \n";
                    pairs_to_class_ID[pairToID(s,a)]=i;
                    break;
                }

            }

            if(!is_new) continue;
            pairs_to_class_ID[pairToID(s,a)]=classes.size();
            classes.push_back(s);
        }

        auto& classes = classes_per_state[a];
        auto& reachable = reachable_per_state[a];
        reachable.resize(classes.size(),false);
    }



}

void Quotient::generateCover(InputTree& cover, std::vector<MacroState>& cover_data) {

    assert(!has_reachability_data);

    /// Cover should have only the initial vertex
    assert(cover.size()==1);

    assert(cover_data.empty());

    has_reachability_data=true;



    cover_data.push_back({{{0}, 0} });

    setReachable(0,0);

    for(uint32_t i = 0; i < cover_data.size(); ++i){
        uint32_t node = i;


        for(uint32_t in=0; in<A.numberOfInputs(); ++in){
            const auto& A_set = cover_data[i].A_set;
            const auto& s = cover_data[i].s;

            if(cover.getNext(node,in)!=InputTree::NO_SUCC) continue;
            auto next_A_set = A.propagate(A_set,in);
            auto next_s = M.getSucc(s, in);

            /// REFACTOR IF OTHER SETS ARE USED

            auto it = next_A_set.begin();
            while(it!=next_A_set.end()){
                auto next_a = *it;
                if(isReachable(next_s,next_a)){
                    it = next_A_set.erase(it);
                } else {
                    setReachable(next_s, next_a);
                    ++it;
                }

            }

            if(next_A_set.empty()) continue;

            cover.addSymbol(in, node);
            cover_data.push_back({std::move(next_A_set), next_s});
        }

    }


}
