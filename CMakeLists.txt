cmake_minimum_required(VERSION 3.28)
project(estimatingStringPatternAlgorithms)

set(CMAKE_CXX_STANDARD 20)

add_executable(estimatingStringPatternAlgorithms main.cpp
        include/AUTOMATON.h
        include/timer.h
        include/stats.h
        include/log2.h
        include/GRAPH.h
        include/define.h
        kr.h
        include/main_header.h
        kmp.h
        Algorithm.h
        Algorithms.h
        bm.h
        bndmq2.h
        bxs.h
        bom.h
        skip.h
        hash3.h
        fs.h
        include/cell.h
        bsdm.h
        ssm.h
        sbndm.h
        python_scripts/newscript.py
        ssef.h
)
