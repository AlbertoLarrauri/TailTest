//
// Created by llarrauriborroto on 23/11/2021.
//

#include "basic.h"

using namespace TailTest;

void BasicTester::step() {
    Frame curr_frame=frames.back();
    auto in=*(curr_frame.it);
    curr_seq.push_back(in);
    flat_set next_A_set;
    for(auto a:curr_frame.A_set){
        const auto& succs=A.getSuccs(a,in);
        next_A_set.insert(succs.begin(),succs.end());
    }
}
