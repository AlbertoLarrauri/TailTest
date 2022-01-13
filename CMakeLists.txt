cmake_minimum_required(VERSION 3.19)
project(TailTest)

set(CMAKE_CXX_STANDARD 17)

include_directories(machines)
include_directories(testers)
include_directories(Learning)
include_directories(FSMlib)


add_library( TailTest SHARED
        machines/machines.cpp
        machines/machines.h
        machines/builders.h
        machines/builders.cpp
        machines/serializer.cpp
        machines/serializer.h
        machines/containment_relation.h
        machines/containment_relation.cpp
        machines/incompatibility_data.cpp
        machines/incompatibility_data.h
        machines/input_tree.cpp
        machines/input_tree.h
        machines/quotient.cpp
        machines/quotient.h
        testers/basic.cpp
        testers/basic.h
        testers/advanced.cpp
        testers/advanced.h
        )

add_library(FSMlib SHARED
        FSMlib/Learning/BlackBox.h
        FSMlib/Learning/BlackBoxDFSM.cpp
        FSMlib/Learning/BlackBoxDFSM.h
        FSMlib/Learning/DiscriminationTreeAlgorithm.cpp
        FSMlib/Learning/FSMlearning.h
        FSMlib/Learning/GoodSplit.cpp
        FSMlib/Learning/H-learner.cpp
        FSMlib/Learning/Lstar.cpp
        FSMlib/Learning/ObservationPackAlgorithm.cpp
        FSMlib/Learning/ObservationTreeAlgorithmADS.cpp
        FSMlib/Learning/QuotientAlgorithm.cpp
        FSMlib/Learning/S-learner.cpp
        FSMlib/Learning/SPY-learner.cpp
        FSMlib/Learning/Teacher.h
        FSMlib/Learning/TeacherBB.cpp
        FSMlib/Learning/TeacherBB.h
        FSMlib/Learning/TeacherDFSM.cpp
        FSMlib/Learning/TeacherDFSM.h
        FSMlib/Learning/TeacherRL.cpp
        FSMlib/Learning/TeacherRL.h
        FSMlib/Learning/TTT.cpp
        FSMlib/Model/DFA.cpp
        FSMlib/Model/DFA.h
        FSMlib/Model/DFSM.cpp
        FSMlib/Model/DFSM.h
        FSMlib/Model/FSM.h
        FSMlib/Model/FSMmodel.cpp
        FSMlib/Model/FSMmodel.h
        FSMlib/Model/FSMtypes.h
        FSMlib/Model/Mealy.cpp
        FSMlib/Model/Mealy.h
        FSMlib/Model/Moore.cpp
        FSMlib/Model/Moore.h
        FSMlib/Sequences/AdaptiveDistinguishingSequence.cpp
        FSMlib/Sequences/CharacterizingSequences.cpp
        FSMlib/Sequences/DistinguishingSequences.cpp
        FSMlib/Sequences/FSMcovers.cpp
        FSMlib/Sequences/FSMsequence.h
        FSMlib/Sequences/HomingSequence.cpp
        FSMlib/Sequences/PresetDistinguishingSequence.cpp
        FSMlib/Sequences/SeparatingSequences_Parallel.cu
        FSMlib/Sequences/SplittingTree.cpp
        FSMlib/Sequences/StateVerifyingSequence.cpp
        FSMlib/Sequences/SynchronizingSequence.cpp
        FSMlib/Testing/ADS-method.cpp
        FSMlib/Testing/C-method.cpp
        FSMlib/Testing/FaultCoverageChecker.cpp
        FSMlib/Testing/FaultCoverageChecker.h
        FSMlib/Testing/FSMtesting.h
        FSMlib/Testing/H-method.cpp
        FSMlib/Testing/HSI-method.cpp
        FSMlib/Testing/Ma-method.cpp
        FSMlib/Testing/Mg-method.cpp
        #        FSMlib/Testing/Mstar-method.cpp
        FSMlib/Testing/PDS-method.cpp
        FSMlib/Testing/S-method.cpp
        FSMlib/Testing/SPY-method.cpp
        FSMlib/Testing/SPYH-method.cpp
        FSMlib/Testing/SVS-method.cpp
        FSMlib/Testing/W-method.cpp
        FSMlib/Testing/Wp-method.cpp
        #        dllmain.cpp
        FSMlib/FSMlib.cpp
        FSMlib/FSMlib.h
        FSMlib/PrefixSet.cpp
        FSMlib/PrefixSet.h
        FSMlib/prescan.cu
        #        stdafx.cpp
        #        stdafx.h
        FSMlib/targetver.h
        FSMlib/UnionFind.cpp
        FSMlib/UnionFind.h)

add_executable(main main.cpp)
add_executable(comparing comparing.cpp)

target_link_libraries(main PUBLIC TailTest)
target_link_libraries(comparing PUBLIC TailTest FSMlib)