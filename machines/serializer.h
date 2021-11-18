//
// Created by llarrauriborroto on 05/03/2021.
//

#ifndef PMIN_SERIALIZER_H
#define PMIN_SERIALIZER_H

#include<filesystem>

namespace TailTest{

    void serializeDFSM(const DFSM_old& dfsm, std::string name="dfsm0", std::filesystem::path dir= "./benchmarks/");
    void serializeDFSM(const DFSM_old& dfsm, std::ostream& ostream);

    DFSM_old deserializeDFSM(const std::filesystem::path& file_dir);
    DFSM_old deserializeDFSM(const std::string& str);


    void serializeCascade(const DFSM_old &driver, const DFSM_old& driven, std::string name= "cascade0", std::filesystem::path dir= "./benchmarks/");

    std::pair<DFSM_old,DFSM_old> deserializeCascade(const std::filesystem::path& file_dir);








}


#endif //PMIN_SERIALIZER_H
