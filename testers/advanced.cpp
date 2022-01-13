//
// Created by llarrauriborroto on 07/01/2022.
//

#include "advanced.h"
#include "../machines/quotient.h"
#include <unordered_set>
#include <algorithm>

using namespace TailTest;

void AdvancedTester::step() {

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

void AdvancedTester::pop() {
    assert(vertex_seq.size() == frames.size());

    auto &curr_frame = frames.back();
    auto curr_vertex = vertex_seq.back();
    auto curr_s = curr_frame.s;

    for (auto id: curr_frame.toDistinguish) {
        auto[t, a]=IDToPair(id);
        suite.addSequence(data.distinguishingSequence(curr_s, a, t, a), curr_vertex);
    }


    vertex_seq.pop_back();
    frames.pop_back();

    if (!current_seq.empty()) {
        current_seq.pop_back();
    }

}


void AdvancedTester::popAndPrepare() {

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

std::vector<std::unordered_set<uint32_t>> AdvancedTester::backPropagate(uint32_t a) {
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


//uint32_t AdvancedTester::getScore(const AdvancedTester::DownSet &back_propagation) {
//    NumVec scores(A.size(), 0);
//    for (const auto &A_set: back_propagation) {
//        for (auto a: A_set) {
//            ++scores[a];
//        }
//    }
//
//    return *(std::max_element(scores.begin(), scores.end()));
//}

void AdvancedTester::exploitCertificate(Certificate &cert) {
    if(cert.exploited) return;
    cert.exploited = true;

    auto &chain = cert.chain;
    NumVec states;
    for (auto a: chain.states) {
        if (states.empty() || a != states.back()) {
            states.push_back(a);
        }
    }

    for (uint32_t i = 0; i < chain.size(); ++i) {
        auto pos = chain.positions[i];
        auto a = chain.states[i];
        auto s = frames[pos].s;
        for (auto[t, b]: cert.basis) {
            if (data.areCompatible(s, a, t, b)) continue;
            for (auto rit = states.rbegin(); rit != states.rend(); ++rit) {
                auto c = *rit;
                if (data.areCompatible(s, c, t, c)) continue;
//                std::cout << " Distinguishing location (" << s << ", " << a << ") from (" <<
//                          t << ", " << c << ") at position " << pos << "\n";
                frames[pos].toDistinguish.insert(pairToID(t, c));
                break;
            }
        }
    }

    for(uint32_t i=0; i< cert.basis.size(); ++i){
        auto [s,a]=cert.basis[i];
        auto vertex = cover_data.states_to_vertex[pairToID(s,a)];
        for( uint32_t j=i+1; j< cert.basis.size(); ++j){
            auto [t,b]=cert.basis[j];
            if( data.areCompatible(s,a,t,b)) continue;
            for (auto rit = states.rbegin(); rit != states.rend(); ++rit) {
                auto c = *rit;
                if (data.areCompatible(s, c, t, c)) continue;
//                std::cout << " Distinguishing basis locations (" << s << ", " << a << ") from (" <<
//                          t << ", " << b << ") at vertex " << vertex << " using "<< c << "\n";
                auto dist_seq= data.distinguishingSequence(s,c,t, c);
//                std::cout<<"Distinguishing sequence : ";
//                for(auto symb:dist_seq){
//                    std::cout<<symb<<" - ";
//                }
//                std::cout<<"\n";
                suite.addSequence(dist_seq, vertex);

                break;
            }

        }

    }

}

AdvancedTester::AdvancedTester(const DFSM &_M, const NFA &_A, const ContainmentRelation &_rel) :
        M(_M),
        A(_A),
        rel(_rel),
        suite(A.numberOfInputs()),
        data(_M, _A),
        quotient(_M, _A, data) {}


bool AdvancedTester::searchCertificates(uint32_t k) {
    auto &curr_frame = frames.back();
    bool all_found = true;

    struct ChainSearchNode {
        ChainSearchNode *parent = nullptr;
        uint32_t state = 0;
        uint32_t position = 0;
        uint32_t height = 0;
    };

    /// For all nodes try to find a certificate

    for (auto&[a, ptr]: frames.back().node_dict) {

        if (ptr) continue;

        auto back_propagation = backPropagate(a);
        std::vector<Chain> chains(A.size());

        std::vector<std::unique_ptr<ChainSearchNode>> chain_search;
        std::unordered_map<uint32_t, ChainSearchNode *> maximal_chains;


        for (uint32_t pos = 0; pos < back_propagation.size(); ++pos) {
            auto &set = *(back_propagation.rbegin() + pos);
            for (auto b: set) {
                uint32_t best_score = 0;
                uint32_t best_candidate = b;
                for (auto&[c, search_node]: maximal_chains) {
                    if (!rel.contains(c, b)) continue;
                    auto curr_score = search_node->height;
                    if (curr_score > best_score) {
                        best_score = curr_score;
                        best_candidate = c;
                    }
                }

                chain_search.push_back(std::make_unique<ChainSearchNode>());
                auto &last_node = chain_search.back();
                last_node->position = pos;
                last_node->state = b;
                last_node->height = best_score + 1;
                if (best_score) {
                    auto parent_node_ptr = maximal_chains[best_candidate];
                    last_node->parent = parent_node_ptr;
                }
                maximal_chains[b] = last_node.get();
            }
        }

        bool found = false;
        uint32_t witness;
        Basis basis;

        for (auto &[state, node]: maximal_chains) {
            auto score = node->height;
            if (score + M.size() < k + 1) {
                continue;
            }

            /// Computing basis for chain
            NumVec states;
            {
                states.push_back(node->state);
                auto curr_node = node->parent;
                while (curr_node != nullptr) {
                    if (states.empty() || curr_node->state != states.back()) {
                        states.push_back(curr_node->state);
                    }
                    curr_node = curr_node->parent;
                }

                std::reverse(states.begin(), states.end());

            }

            basis = getBasis(states);

            if (basis.size() + node->height > k) {
                found = true;
                witness = state;
                break;
            }


//            if (score < k + 1) {
//                continue;
//            } else {
//                found = true;
//                witness = state;
//                break;
//            }
//            NumVec states;
//            states.push_back(node->state);
//            auto curr_node = node->parent;
//            while (curr_node != nullptr) {
//                if (curr_node->state != states.back()) {
//                    states.push_back(curr_node->state);
//                }
//                curr_node = curr_node->parent;
//            }
//            basis = getBasis(states);

        }

        if (!found) {
            all_found = false;
            break;
        }

        std::unique_ptr<Certificate> cert = std::make_unique<Certificate>();
        cert->basis = std::move(basis);
        auto &search_node = maximal_chains[witness];
        assert(search_node);
//
//        NumVec states;
//        {
//            states.push_back(search_node->state);
//            auto curr_node = search_node->parent;
//            while (curr_node != nullptr) {
//                if (states.empty() || curr_node->state != states.back()) {
//                    states.push_back(curr_node->state);
//                }
//                curr_node = curr_node->parent;
//            }
//        }
//        std::reverse(states.begin(), states.end());
//
//
//        cert->basis = getBasis(states);

        NumVec& rev_positions= cert->chain.positions;
        NumVec& rev_states = cert->chain.states;

        rev_positions.resize(search_node->height);
        rev_states.resize(search_node->height);

        {
            auto curr_node = search_node;
            for (uint32_t i = 0; i < search_node->height; ++i) {
                rev_positions[i] = curr_node->position;
                rev_states[i] = curr_node->state;
                curr_node = curr_node->parent;
            }

            std::reverse(rev_positions.begin(), rev_positions.end());
            std::reverse(rev_states.begin(), rev_states.end());
        }


        assert(cert->chain.size() + cert->basis.size() > k);
        curr_frame.certificates.push_back(std::move(cert));
        auto cert_ptr = curr_frame.certificates.back().get();
        curr_frame.node_dict[a] = cert_ptr;
    }

    return all_found;
}


void AdvancedTester::saturateSequence(size_t initial_vertex) {
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
//        std::cout << " Certificate for state " << a << " at sequence: ";
//        for (auto i: current_seq) {
//            std::cout << i << " - ";
//        }
//        std::cout << "\n";
//        ptr->print();

        exploitCertificate(*ptr);
    }

}


const InputTree &AdvancedTester::getSuite(uint32_t k) {
    quotient.generateMinimalCover(suite, cover_data, rel);
    uint32_t cover_size = suite.size();

    for (uint32_t source_vertex = 0; source_vertex < cover_size; ++source_vertex) {
        if (!cover_data.is_vertex_in_cover[source_vertex]) continue;

        auto &source_macro_state = cover_data.cover_macro_states[source_vertex];
        NumVec source_inputs;

        {
            NumSet source_inputs_aux;
            for (auto a: source_macro_state.A_set) {
                for (auto in: A.getSymbols(a)) {
                    source_inputs_aux.insert(in);
                }
            }

            source_inputs.insert(source_inputs.begin(),
                                 source_inputs_aux.begin(), source_inputs_aux.end());
        }

        for (auto source_in: source_inputs) {
            auto initial_vertex = suite.getNext(source_vertex, source_in);
            if (initial_vertex < cover_size) continue;

            if (initial_vertex == InputTree::NO_SUCC) {
                initial_vertex = suite.addSymbol(source_in, source_vertex);
            }

            auto initial_A_set = A.propagate(source_macro_state.A_set, source_in);
            auto initial_s = M.getSucc(source_macro_state.s, source_in);

            assert(!initial_A_set.empty());

            current_seq.clear();

            vertex_seq = {initial_vertex};

            /// Building initial frame

            frames.clear();

            frames.emplace_back();

            auto &initial_frame = frames.back();

            initial_frame.s = initial_s;

            for (auto a: initial_A_set) {
                initial_frame.node_dict[a] = nullptr;
            }

            auto &initial_inputs = initial_frame.inputs;

            {
                NumSet initial_inputs_aux;
                for (auto a: initial_A_set) {
                    for (auto in: A.getSymbols(a)) {
                        initial_inputs_aux.insert(in);
                    }
                }

                initial_inputs.insert(initial_inputs.begin(),
                                      initial_inputs_aux.begin(), initial_inputs_aux.end());

            }


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

AdvancedTester::Basis AdvancedTester::getBasis(NumVec &states) {
    assert(states.size() > 0);

    if (states.size() > 1) {
        for (uint32_t i = 0; i + 1 < states.size(); ++i) {
            assert(rel.contains(states[i], states[i + 1]));
        }
    }

    Basis basis;
    std::vector<NumSet> visited_locations(states.size());
    for (uint32_t s = 0; s < M.size(); ++s) {
        for (uint32_t i = 0; i < states.size(); ++i) {
            auto a = states[i];
            auto t = quotient.getClass(s, a);
            if (visited_locations[i].count(t)) break;
            visited_locations[i].insert(t);
            if (quotient.isReachable(t, a)) {
                basis.emplace_back(t, a);
                break;
            }
        }
    }

    return basis;
}
