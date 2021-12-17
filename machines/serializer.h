//
// Created by llarrauriborroto on 05/03/2021.
//

#ifndef PMIN_SERIALIZER_H
#define PMIN_SERIALIZER_H

#include<filesystem>

namespace TailTest{

    void serializeDFSM(const DFSM &dfsm, std::string name= "dfsm0", std::filesystem::path dir= "./benchmarks/");
    void serializeDFSM(const DFSM &dfsm, std::ostream& ostream);



    DFSM deserializeDFSM(const std::filesystem::path& file_dir);
    DFSM deserializeDFSM(const std::string& str);


    void serializeCascade(const DFSM &driver, const DFSM &driven, std::string name= "cascade0", std::filesystem::path dir= "./benchmarks/");

    std::pair<DFSM, DFSM> deserializeCascade(const std::filesystem::path& file_dir);


    void serializeDFSM_FSMlib(const DFSM &dfsm, std::ostream& ostream, bool reduced= false);

    void serializeDFSM_FSMlib(const DFSM &dfsm, std::string name= "dfsm0", std::filesystem::path dir= "./benchmarks/", bool reduced= false);








}


#endif //PMIN_SERIALIZER_H
