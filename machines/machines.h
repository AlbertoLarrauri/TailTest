//
// Created by llarrauriborroto on 13/01/2021.
//

#ifndef PMIN_MACHINES_H
#define PMIN_MACHINES_H

#include<iostream>
#include <vector>
#include <optional>
#include <unordered_map>
#include <cassert>
#include <memory>
#include <boost/container/flat_set.hpp>


namespace TailTest
{
//static char wrong_size_1[1 + sizeof(std::size_t) - sizeof(std::vector<size_t>::size_type)];
//
//static char wrong_size_2[1 - sizeof(std::size_t) + sizeof(std::vector<size_t>::size_type)];






    class FSM_old{
    protected:
        uint32_t size_impl=0;
        uint32_t in_alphabet_size;
        uint32_t out_alphabet_size;

        //CONSTRUCTORS

        FSM_old(uint32_t in_size, uint32_t out_size): in_alphabet_size(in_size), out_alphabet_size(out_size), size_impl(0){};


        virtual void reset(uint32_t in_size=0, uint32_t out_size=0){
            in_alphabet_size=in_size;
            out_alphabet_size=out_size;
            size_impl=0;
        }

    public:
        inline uint32_t size() const{return size_impl;}
        inline uint32_t numberOfInputs() const{return in_alphabet_size;}
        inline uint32_t numberOfOutputs() const{return out_alphabet_size;}



    };

    class OFA_old;
    class DFSM_old;

    class NFA{
    private:
        typedef boost::container::flat_set<uint32_t> flat_set;
//        typedef std::vector<std::optional<flat_set>> T_IMPL;
        typedef std::vector<flat_set> T_IMPL;
        T_IMPL impl;
        T_IMPL info_impl;
        T_IMPL r_impl;
        T_IMPL r_info_impl;

        inline size_t SIToID(const uint32_t state, const uint32_t in) const{
            return size_t(in_alphabet_size) * size_t(state) + size_t(in);
        }

    protected:
        uint32_t size_impl=0;
        uint32_t in_alphabet_size;

    public:

        inline NFA(uint32_t in_size): in_alphabet_size(in_size){};

        inline uint32_t size() const{
            return size_impl;
        }

        inline uint32_t numberOfInputs() const {
            return in_alphabet_size;
        }

        inline void addStates(uint32_t amount=1){
            size_impl+=amount;

            impl.resize(size_impl * in_alphabet_size);
            r_impl.resize(size_impl * in_alphabet_size);

            info_impl.resize(size_impl);
            r_info_impl.resize(size_impl);
        }



        inline void addSucc(uint32_t state, uint32_t in, uint32_t next){
            auto& symbols = info_impl.at(state);
            auto& succs = impl.at(SIToID(state, in));
            auto& r_symbols = r_info_impl.at(next);
            auto& r_succs = r_impl.at(SIToID(next, in));
            if(succs.empty()) {
                symbols.insert(in);
            }
            if(r_succs.empty()){
                r_symbols.insert(in);
            }
            succs.insert(next);
            r_succs.insert(state);
            return;
        }


        inline const flat_set& getSuccs(uint32_t state, uint32_t in)const{
            assert(!(impl.at(SIToID(state, in)).empty()));
            return impl.at(SIToID(state, in));
        }

        inline const flat_set& getSymbols(uint32_t state)const{
            return info_impl.at(state);
        }


        inline bool hasSymbol(uint32_t state, uint32_t in) const{
            auto& succs = impl.at(SIToID(state, in));
            return !succs.empty();
        }

        inline const flat_set& getRSuccs(uint32_t state, uint32_t in)const{
            assert(!(r_impl.at(SIToID(state, in)).empty()));
            return r_impl.at(SIToID(state, in));
        }

        inline const flat_set& getRSymbols(uint32_t state)const{
            return r_info_impl.at(state);
        }

        inline bool hasRSymbol(uint32_t state, uint32_t in) const{
            auto& succs = r_impl.at(SIToID(state, in));
            return !succs.empty();
        }



    };





    class DFSM{

    private:
        typedef boost::container::flat_set<uint32_t> flat_set;
        class DTrans{
        public:
            uint32_t out;
            uint32_t next;
        };

        typedef std::vector<DTrans> IMPL;
        typedef std::vector<flat_set> R_IMPL;

        IMPL impl;
        R_IMPL r_impl;
        R_IMPL r_info_impl;


        inline size_t SIToID(const uint32_t state, const uint32_t in) const{
            return size_t(in_alphabet_size) * size_t(state) + size_t(in);
        }



    protected:
        uint32_t size_impl=0;
        uint32_t in_alphabet_size;
        uint32_t out_alphabet_size;

    public:

        inline DFSM(uint32_t in_size, uint32_t out_size): in_alphabet_size(in_size),
        out_alphabet_size(out_size){}

        inline uint32_t size() const{
            return size_impl;
        }

        inline uint32_t numberOfInputs() const {
            return in_alphabet_size;
        }

        inline uint32_t numberOfOutputs() const{
            return out_alphabet_size;
        }

        inline void addStates(uint32_t amount=1){
            size_impl+=amount;

            impl.resize(size_impl * in_alphabet_size);
            r_impl.resize(size_impl * in_alphabet_size);
            r_info_impl.resize(size_impl);
        }

        inline void addTransition(uint32_t source, uint32_t in, uint32_t out, uint32_t target){

            auto& trans=impl.at(SIToID(source, in));
            auto& r_succs=r_impl.at(SIToID(target, in));
            auto& r_symbols=r_info_impl.at(source);

            if(!r_succs.empty()){
                assert(!r_succs.contains(source));
            } else {
                r_symbols.insert(in);
            }

            trans.next=target;
            trans.out=out;

            r_succs.insert(source);

            return;
        }

        inline void addTransitionUnchecked(uint32_t source, uint32_t in, uint32_t out, uint32_t target){

            auto& trans=impl.at(SIToID(source, in));
            auto& r_succs=r_impl.at(SIToID(target, in));
            auto& r_symbols=r_info_impl.at(source);


            trans.next=target;
            trans.out=out;

            if(r_succs.empty()) r_symbols.insert(in);
            r_succs.insert(r_succs.end(), source);

            return;
        }

        inline uint32_t getOut(uint32_t state, uint32_t in) const {
            return impl.at(SIToID(state, in)).out;
        }

        inline uint32_t getSucc(uint32_t state, uint32_t in) const {
            return impl.at(SIToID(state, in)).next;
        }

        inline const flat_set& getRSuccs(uint32_t state, uint32_t in) const{
            return r_impl.at(SIToID(state, in));
        }

        inline const flat_set& getRSymbols(uint32_t state) const {
            return r_info_impl.at(state);
        }




    };

















    ////// Old stuff




    class DFSM_old: public virtual FSM_old{
    private:

        class DTrans{
        public:
            uint32_t out;
            uint32_t next;
        };

        // Transition data:

        typedef std::vector<DTrans> IMPL;

        IMPL impl;

        // Helper function to get transition index;

        inline size_t SItoID(const uint32_t state, const uint32_t in) const{
            return size_t(in_alphabet_size) * size_t(state) + size_t(in);
        }



    public:

        inline  DFSM_old(uint32_t in_size, uint32_t out_size): FSM_old(in_size, out_size){
        }

        DFSM_old(const DFSM_old& other)=default;
        DFSM_old& operator=(const DFSM_old& other)=default;

        inline DFSM_old(DFSM_old&& other) noexcept:
        //Fist copies FSM_old parameters: dfsm_size, input alphabet dfsm_size, and output alphabet dfsm_size
                FSM_old(other),
                impl(std::move(other.impl))
        {
            other.reset();
        }

        inline DFSM_old& operator=(DFSM_old&& other){
            FSM_old::operator=(other);
            impl=std::move(other.impl);
            other.reset();
            return *this;
        }

        inline void reset(uint32_t in_size=0, uint32_t out_size=0) override{
            FSM_old::reset(in_size, out_size);
            impl.clear();
        }



        inline void addStates(uint32_t amount=1){
            size_impl+=amount;
            impl.resize(size_impl * in_alphabet_size);
        }


        inline const uint32_t& getOut(uint32_t state, uint32_t in) const{
            return impl.at(SItoID(state, in)).out;
        }

        inline const uint32_t& getSucc(uint32_t state, uint32_t in)const{
            return impl.at(SItoID(state, in)).next;
        }

        inline void setOut(uint32_t state, uint32_t in, uint32_t  out){
            impl.at(SItoID(state, in)).out=out;
        }

        inline void setSucc(uint32_t state, uint32_t in, uint32_t next){
            impl.at(SItoID(state, in)).next=next;
        }

        inline void setTrans(uint32_t state, uint32_t in , uint32_t out, uint32_t next){
            setOut(state,in, out);
            setSucc(state,in ,next);
        }


        void print() const;

    };

    class OFA_old: public virtual FSM_old{

    private:

        class NTrans{
        public:
            uint32_t out;
            std::vector<uint32_t> successors;

        };


        // Transition data. We keep track of getSources and targets of each transition.

        typedef std::vector<std::unordered_map<size_t, std::vector<uint32_t>>> SOURCE_IMPL;
        typedef std::vector<std::optional<NTrans>> IMPL;
        IMPL impl;
        SOURCE_IMPL source_data;

        // Helper functions to get indices in the data structures

        inline size_t SItoID(const uint32_t state, const uint32_t in) const{
            return size_t(in_alphabet_size) * size_t(state) + size_t(in);
        }



        inline size_t IOtoID(const uint32_t i, const uint32_t o) const{
            return size_t(in_alphabet_size)*size_t(o)+ size_t(i);
        }


    public:

        inline OFA_old(uint32_t in_size, uint32_t out_size): FSM_old(in_size, out_size){
        }

        size_t numberOfTransitions();

        OFA_old(const OFA_old& other)=default;

        OFA_old& operator=(const OFA_old& other)=default;

        inline OFA_old(OFA_old&& other):
                FSM_old(other),
                impl(std::move(other.impl)),
                source_data(std::move(other.source_data))
        {
            other.reset();
        }

        inline OFA_old& operator=(OFA_old&& other){
            FSM_old::operator=(other);
            impl=std::move(other.impl);
            source_data=std::move(other.source_data);
            other.reset();
            return *this;
        }



        inline void reset(uint32_t in_size=0, uint32_t out_size=0){
            FSM_old::reset(in_size, out_size);
            impl.clear();
            source_data.clear();
        }



        inline bool hasTransition(const uint32_t state, const uint32_t in) const {
            return impl[SItoID(state, in)].has_value();
        }



        inline const uint32_t  getOut(uint32_t state, uint32_t in) const {
            assert(impl[SItoID(state, in)].has_value());
            return impl[SItoID(state, in)]->out;

        }

        inline void setTransition(uint32_t state, uint32_t in, uint32_t out){
            impl[SItoID(state, in)].emplace();
            impl[SItoID(state, in)]->out=out;
        }

        inline void addSucc(uint32_t state, uint32_t in, uint32_t succ){
            assert(impl[SItoID(state, in)]);
            impl[SItoID(state, in)]->successors.push_back(succ);
            source_data[succ][IOtoID(in, getOut(state, in))].emplace_back(state);
        }

        inline const std::vector<uint32_t> &
        getSuccs(const uint32_t state, const uint32_t in) const {
            assert(impl[SItoID(state, in)]);
            return (impl[SItoID(state, in)]->successors);
        }

        inline void addStates(uint32_t amount=1){
            size_impl+=amount;
            impl.resize(size_impl * in_alphabet_size);
            source_data.resize(size_impl * in_alphabet_size * out_alphabet_size);
        }

        inline bool hasSources(uint32_t state, uint32_t in, uint32_t out){
            return source_data[state].count(IOtoID(in, out));
        }


        inline const std::vector<uint32_t>& getSources(uint32_t state, uint32_t in, uint32_t out){
            assert(hasSources(state, in, out));
            return source_data[state][IOtoID(in, out)];
        }

        inline const std::unordered_map<size_t, std::vector<uint32_t>> & sourceData(uint32_t state) const {
            return source_data[state];
        }

        inline const auto& getSourceData(uint32_t state){
            return source_data[state];
        }



        void print();
    };





    bool areEquivalent(const DFSM_old& A, const DFSM_old& B);
}





#endif //PMIN_MACHINES_H
