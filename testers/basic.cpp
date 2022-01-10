//
// Created by llarrauriborroto on 23/11/2021.
//

#include "basic.h"
#include "../machines/quotient.h"
#include <unordered_set>
#include <algorithm>

using namespace TailTest;

void BasicTester::step() {

    frames.emplace_back();
    auto &succ_frame = frames.back();
    auto &curr_frame = *(frames.end() - 2);
    auto in_it = curr_frame.in_it;
    auto in = curr_frame.inputs[in_it];

    current_seq.push_back(in);

    if (vertex_seq.size() == frames.size() - 1) {
        auto curr_vertex = vertex_seq.back();
        auto next_vertex = suite.getNext(curr_vertex, in);
        if (next_vertex != InputTree::NO_SUCC) {
            vertex_seq.push_back(next_vertex);
        }
    }


    auto &succ_dict = succ_frame.node_dict;
    auto &succ_inputs = succ_frame.inputs;
//    std::unordered_map<uint32_t, bool> succ_dict;
    std::unordered_set<uint32_t> succ_inputs_aux;


    for (auto&[a, ptr]: curr_frame.node_dict) {
        if (!A.getSymbols(a).contains(in)) continue;
        for (auto b: A.getSuccs(a, in)) {
            auto &cert_ptr = succ_dict[b];
            if (!cert_ptr) {
                cert_ptr = ptr;
            }

            for (auto next_in: A.getSymbols(b)) {
                succ_inputs_aux.insert(next_in);
            }
        }
    }

    succ_inputs.insert(succ_inputs.begin(), succ_inputs_aux.begin(), succ_inputs_aux.end());


    succ_frame.s = M.getSucc(curr_frame.s, in);


}

void BasicTester::pop() {
    assert(vertex_seq.size() == frames.size());

    auto& curr_frame = frames.back();
    auto curr_vertex = vertex_seq.back();
    auto curr_s = curr_frame.s;

    for(auto id:curr_frame.toDistinguish){
        auto [t,a]=IDToPair(id);
        suite.addSequence(data.distinguishingSequence(curr_s,a,t,a), curr_vertex);
    }


    vertex_seq.pop_back();
    frames.pop_back();

    if(!current_seq.empty()){
    current_seq.pop_back();
    }

}


void BasicTester::popAndPrepare() {

    while (frames.size() > 1) {
        pop();
        auto &curr_frame = frames.back();
        auto &it = curr_frame.in_it;
        auto max_it = curr_frame.inputs.size();
        ++it;
        if (it < max_it) return;
    }

    pop();

}



/// witness belongs to frames.back().A_set

std::vector<std::unordered_set<uint32_t>> BasicTester::backPropagate(uint32_t a) {
    DownSet backpropagation;
    backpropagation.push_back({a});

    for (size_t i = 1; i <= current_seq.size(); ++i) {
        auto idx = current_seq.size() - i;
        auto in = current_seq[idx];
        auto &source_node_dict = frames[idx].node_dict;

        backpropagation.emplace_back();
        auto &source_nodes = backpropagation[i];
        auto &target_nodes = backpropagation[i - 1];

        for (auto target: target_nodes) {
            for (auto source: A.getRSuccs(target, in)) {
                if (source_node_dict.count(source)) {
                    source_nodes.insert(source);
                }
            }
        }
    }
    return backpropagation;
}


//uint32_t BasicTester::getScore(const BasicTester::DownSet &back_propagation) {
//    NumVec scores(A.size(), 0);
//    for (const auto &A_set: back_propagation) {
//        for (auto a: A_set) {
//            ++scores[a];
//        }
//    }
//
//    return *(std::max_element(scores.begin(), scores.end()));
//}

void BasicTester::exploitCertificate(Certificate &cert) {
    if (cert.exploited) return;
    cert.exploited = true;
    auto &positions = cert.positions;
    auto &a = cert.witness;
//
//    NumVec state_vec;
//
//    {
//        std::unordered_set<uint32_t> state_set;
//        for (auto pos: positions) {
//            auto s = frames[pos].s;
//            state_set.insert(s);
//        }
//
//        state_vec.insert(state_vec.end(), state_set.begin(), state_set.end());
//    }

    auto &state_vec = quotient.getClasses(a);
    for (auto pos: positions) {
        auto s = frames[pos].s;
        for (auto t: state_vec) {
            if (!data.areCompatible(s, a, t, a)) {
//                auto dist_seq = data.distinguishingSequence(s, a, t, a);
//                NumVec seq_to_add(current_seq.begin(), current_seq.begin() + pos);
//                seq_to_add.insert(seq_to_add.end(), dist_seq.begin(), dist_seq.end());
//                suite.addSequence(seq_to_add, initial_vertex);
                frames[pos].toDistinguish.insert(pairToID(t, a));
            }
        }
    }

}

BasicTester::BasicTester(const DFSM &_M, const NFA &_A) :
        M(_M),
        A(_A),
        suite(A.numberOfInputs()),
        data(_M, _A),
        quotient(_M, _A, data) {}


bool BasicTester::searchCertificates(uint32_t k) {
    auto &curr_frame = frames.back();
    bool all_found = true;


    /// For all nodes try to find a certificate

    for (auto&[a, ptr]: frames.back().node_dict) {

        if (ptr) continue;

        auto back_propagation = backPropagate(a);
        std::vector<NumVec> certificates(A.size());
        NumVec scores(A.size(), 0);
        for (uint32_t b = 0; b < A.size(); ++b) {
            scores[b] = quotient.getClasses(b).size();
        }
        bool found = false;
        uint32_t witness = 0;

        for (uint32_t pos = 0; pos < back_propagation.size(); ++pos) {
            auto &set = *(back_propagation.rbegin() + pos);
            for (auto b: set) {
                auto &positions = certificates[b];
                positions.push_back(pos);
                auto &score = scores[b];
//                ++score;

                if (score + positions.size() == k + 1) {
                    witness = b;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        if (!found) {
            all_found = false;
            continue;
        }


        curr_frame.certificates.push_back(std::make_unique<Certificate>());
        auto &cert_ptr = curr_frame.certificates.back();
        auto &cert = *cert_ptr;

        cert.witness = witness;
        cert.positions = std::move(certificates[witness]);

        ptr = cert_ptr.get();

//        auto rit = back_propagation.rbegin();
//        uint32_t pos = 0;
//
//        while (rit != back_propagation.rend()) {
//            auto &set = *rit;
//            if (set.count(witness)) {
//
//                cert.positions.push_back(pos);
//                if (cert.positions.size() == k + 1) break;
//            }
//
//            ++pos;
//            ++rit;
//        }

    }

    return all_found;
}


void BasicTester::saturateSequence(size_t initial_vertex) {
    suite.addSequence(current_seq, initial_vertex);
    vertex_seq.reserve(current_seq.size());
    auto curr_vertex = vertex_seq.back();

    for (auto it = current_seq.begin() + (vertex_seq.size() - 1); it != current_seq.end(); ++it) {
        curr_vertex = suite.getNext(curr_vertex, *it);
        assert(curr_vertex != InputTree::NO_SUCC);
        vertex_seq.push_back(curr_vertex);
    }

//    std::cout << "Saturating sequence: \n";
//    std::cout << " { ";
//    for (auto i: current_seq) std::cout << i << ", ";
//    std::cout << " }\n";

    auto &curr_frame = frames.back();

    for (auto&[a, ptr]: curr_frame.node_dict) {
        assert(ptr);
        exploitCertificate(*ptr);
    }

}


const InputTree &BasicTester::getSuite(uint32_t k) {

    quotient.generateCover(suite, cover_data);

    size_t cover_size = cover_data.size();

    std::vector<MacroState> cover_macro_states(cover_size);
    cover_macro_states[0] = {{0}, 0};

    {
        std::vector<size_t> stack = {0};
        while (!stack.empty()) {
            auto curr_vertex = stack.back();
            auto curr_s = cover_macro_states[curr_vertex].s;
            auto &curr_A_set = cover_macro_states[curr_vertex].A_set;
            stack.pop_back();
            for (uint32_t in = 0; in < A.numberOfInputs(); ++in) {
                auto next_vertex = suite.getNext(curr_vertex, in);
                if (next_vertex == InputTree::NO_SUCC) continue;
                stack.push_back(next_vertex);
                cover_macro_states[next_vertex].s = M.getSucc(curr_s, in);
                cover_macro_states[next_vertex].A_set = A.propagate(curr_A_set, in);
            }
        }
    }


    for (size_t vertex = 0; vertex < cover_size; ++vertex) {
        auto &A_set = cover_data[vertex].A_set;
        auto s = cover_data[vertex].s;
        for (auto a: A_set) {
            const auto &classes = quotient.getClasses(a);
            for (auto t: classes) {
                if (data.areCompatible(s, a, t, a)) continue;
                auto dist_sequence = data.distinguishingSequence(s, a, t, a);
                suite.addSequence(dist_sequence, vertex);
            }
        }
    }


    for (size_t vertex = 0; vertex < cover_size; ++vertex) {

        auto &pre_A_set = cover_macro_states[vertex].A_set;
        auto pre_s = cover_macro_states[vertex].s;

        for (uint32_t initial_in = 0; initial_in < A.numberOfInputs(); ++initial_in) {

            auto initial_vertex = suite.getNext(vertex, initial_in);
            if (initial_vertex < cover_size) continue;

            auto initial_A_set = A.propagate(pre_A_set, initial_in);
            auto initial_s = M.getSucc(pre_s, initial_in);

            if (initial_A_set.empty()) continue;

            if (initial_vertex == InputTree::NO_SUCC) {
                initial_vertex = suite.addSymbol(initial_in, vertex);
            }


            current_seq.clear();

            vertex_seq = {initial_vertex};
            //// Building initial frame:

            frames.clear();

            frames.emplace_back();

            auto &initial_frame = frames.back();

            auto &initial_dict = initial_frame.node_dict;

            for (auto a: initial_A_set) {
                initial_dict[a] = nullptr;
            }

            auto &initial_inputs = initial_frame.inputs;

            NumSet initial_inputs_aux;

            for (auto a: initial_A_set) {
                for (auto in: A.getSymbols(a)) {
                    initial_inputs_aux.insert(in);
                }
            }

            initial_inputs.insert(initial_inputs.begin(), initial_inputs_aux.begin(),
                                  initial_inputs_aux.end());

            initial_frame.s = initial_s;


            while (!frames.empty()) {

                while (!searchCertificates(k)) {
                    step();
                }

                saturateSequence(initial_vertex);

                popAndPrepare();
            }
        }
    }

    return suite;
}