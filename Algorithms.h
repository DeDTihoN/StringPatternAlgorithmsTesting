//
// Created by pc on 5/30/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_ALGORITHMS_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_ALGORITHMS_H

#include<iostream>
#include "Algorithm.h"
#include "KMP.h"
#include "kr.h"
#include "bndmq2.h"
#include "bm.h"
#include "bxs.h"
#include "bom.h"
#include "skip.h"
#include "hash3.h"
#include "fs.h"
#include "sbndm.h"
#include "ssm.h"
#include "bsdm.h"
#include "ssef.h"
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

const bool checkAnswer = false;

class Algorithms {
    public:
    std::vector<Algorithm*> algorithms;
    const int RoundsRandom = 30;
    const int RoundsUnique = 30;
    const int StartM = 2;
    Algorithms() {
        algorithms.push_back(new KR());
        algorithms.push_back(new KMP());
        algorithms.push_back(new bndmq2());
        algorithms.push_back(new bm());
        algorithms.push_back(new bxs());
        algorithms.push_back(new bom());
        algorithms.push_back(new skip());
        algorithms.push_back(new hash3());
        algorithms.push_back(new fs());
        algorithms.push_back(new ssm());
        algorithms.push_back(new sbndm());
        algorithms.push_back(new bsdm());
//        algorithms.push_back(new SSEF());
    }

    vector<double> add (const vector<double> &a, const vector<double> &b) {
        // Убедимся, что оба вектора одинакового размера
        if (a.size() != b.size()) {
            throw invalid_argument("Vectors must be of the same length.");
        }

        vector<double> c(a.size());
        for (size_t i = 0; i < a.size(); i++) {
            c[i] = a[i] + b[i];
        }
        return c;
    }

    vector<vector<double>> add(const vector<vector<double>> &a, const vector<vector<double>> &b) {
        if (a.size() != b.size()) {
            throw invalid_argument("Matrix must be of the same length.");
        }
        vector<vector<double>> c;
        for (size_t i = 0; i < a.size(); i++) {
            if (a[i].size() != b[i].size()) {
                cout<<a[i].size()<<" "<<b[i].size()<<endl;
                for (size_t j = 0; j < a[i].size(); j++) {
                    cout<<a[i][j]<<" ";
                }
                cout<<endl;
                for (size_t j = 0; j < b[i].size(); j++) {
                    cout<<b[i][j]<<" ";
                }
                cout<<endl;
                throw invalid_argument("Vectors must be of the same length.");
            }
            c.push_back(add(a[i], b[i]));
        }
        return c;
    }

    void test(unsigned char *x, int m, unsigned char *y, int n) {
        int answer=-1;
        //std::cout << "test "<< m<<" "<<n<<std::endl;
        for (auto& algorithm : algorithms) {
            int cur_ans = algorithm->search(x, m, y, n);
            //std::cout << algorithm->name() << ": " << cur_ans << std::endl;
            if (answer == -1) {
                answer = cur_ans;
            } else {
                if (answer != cur_ans && cur_ans != -1) {
                    std::cout << "Error: " << algorithm->name() << std::endl;
                    throw "error in test";
                }
            }
        }
//        std::cout<<"Pattern x: "<<x<<std::endl;
//        std::cout << "Test passed" << " Answer: " << answer << std::endl;
    }

    std::vector<long long > testAll(unsigned char *x, int m, unsigned char *y, int n) {
        std::vector<long long > answers;
        for (auto& algorithm : algorithms) {
            answers.push_back(algorithm->search_with_time(x,m,y,n));
        }
        return answers;
    }

    static unsigned char* getSubstrOfLen(std::vector<unsigned char> y, int m){
        int n = y.size();
        unsigned char* x = new unsigned char[m];
        int ind = rand() % (n - m + 1);
        for (int i = 0; i < m; ++i) {
            x[i] = y[ind + i];
        }
        return x;
    }

    void saveMatrixResultsToFile(std::string file_name, std::vector<std::vector<double>> matrix,string suffix) {
        string res = "";

        for (auto &algorithm : algorithms) {
            res+=algorithm->name();
            res+=" ";
        }
        res+="\n";
        int m = StartM;
        for (auto &row : matrix) {
            if (row.size()!= algorithms.size()){
                std::cerr << "Wrong matrix size" << std::endl;
                return;
            }
            res+=to_string(m);
            for (auto &el : row) {
                res+=" ";
                res+=to_string(el);
            }
            m*=2;
            res+="\n";
        }
        saveResultToFile(file_name,res,suffix);
    }

    std::vector<std::vector<double>> matrixToRelation(std::vector<std::vector<double>> matrix) {
        std::vector<std::vector<double>> res;
        for (auto &row : matrix) {
            if (row.size()!= algorithms.size()){
                std::cerr << "Wrong matrix size" << std::endl;
                return res;
            }
            vector<double> newRow = vectorToRelation(row);
            res.push_back(newRow);
        }
        return res;
    }

    vector<double> vectorToRelation(vector<double> vec) {
        vector<double>tmp(vec);
        sort(tmp.begin(), tmp.end());
        double middle = tmp[0];
        vector<double> res;
        for (auto &el : vec) {
            res.push_back((el-middle)/middle);
        }
        return res;
    }

    std::pair<std::vector<std::vector<double>>, std::vector<std::vector<double>> >  makeRandomTestFromFile(std::string file_name,bool check = checkAnswer) {
        std::ifstream file(file_name);
        if (!file) {
            std::cerr << "Cannot open the file: " << file_name << std::endl;
            return {std::vector<std::vector<double>>(), std::vector<std::vector<double>>()};
        }

        std::string s((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        int n = s.length();

        // Преобразование строки в массив unsigned char
        std::vector<unsigned char> y(s.begin(), s.end());
        y.resize(n + XSIZE); // Для дополнительной памяти

        std::vector<std::vector<double>> res;
        for (int m = StartM; m <= n && m <= XSIZE; m *= 2) {
            // Генерация случайного шаблона длины m

            std::vector<long long > answers(algorithms.size());
            for (int i = 0; i < RoundsRandom; ++i) {
                unsigned char *x = getSubstrOfLen(y, m);
                if (check) {
                    test(x, m, y.data(), n);
                }
                std::vector<long long > cur_answers = testAll(x, m, y.data(), n);
                for (int j = 0; j < algorithms.size(); ++j) {
                    answers[j] += cur_answers[j];
                }
            }
            std::vector<double>row;
            for (int i = 0; i < answers.size(); ++i) {
                row.push_back((double)((long long)((double )answers[i]/(double)1e4))/100.0);
            }
            res.push_back(row);
        }
        saveMatrixResultsToFile(file_name,res,"RandomPatternFromTextAbsoluteResults");
        saveMatrixResultsToFile(file_name,matrixToRelation(res),"RandomPatternFromTextRelativeResults");
        return {res, matrixToRelation(res)};
    }

    pair<vector<vector<double>>, vector<vector<double>>> makeFullRandomTestFromFile(std::string file_name,bool check = checkAnswer) {
        std::ifstream file(file_name);
        if (!file) {
            std::cerr << "Cannot open the file: " << file_name << std::endl;
            return {vector<vector<double>>(), vector<vector<double>>()};
        }

        std::string s((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        int n = s.length();
        // Преобразование строки в массив unsigned char
        std::vector<unsigned char> y(s.begin(), s.end());
        y.resize(n + XSIZE); // Для дополнительной памяти

        std::vector<std::vector<double>> res;
        // Тестирование для m степеней двойки от 4 до 1024
        for (int m = StartM; m <= n && m <= XSIZE; m *= 2) {
            // Генерация случайного шаблона длины m

            std::vector<long long> answers(algorithms.size());
            for (int i = 0; i < RoundsRandom; ++i) {
                unsigned char x[m];
                for (int j = 0; j < m; ++j) {
                    x[j] = rand() % 256;
                }
                if (check) {
                    test(x, m, y.data(), n);
                }
                std::vector<long long> cur_answers = testAll(x, m, y.data(), n);
                for (int j = 0; j < algorithms.size(); ++j) {
                    answers[j] += cur_answers[j];
                }
            }
            std::vector<double> row;
            for (int i = 0; i < answers.size(); ++i) {
                row.push_back((double) ((long long) ((double) answers[i] / (double) 1e4)) / 100.0);
            }
            res.push_back(row);
        }
        saveMatrixResultsToFile(file_name, res, "FullRandomPatternsResultsAbsolute");
        saveMatrixResultsToFile(file_name, matrixToRelation(res), "FullRandomPatternsResultsRelative");
        return {res, matrixToRelation(res)};
    }

    void saveVectorToFile(std::string file_name, std::vector<double> vec,string suffix) {
        string res = "";
        for (auto &algorithm : algorithms) {
            res+=algorithm->name();
            res+=" ";
        }
        res+="\n";
        for (int i = 0; i < vec.size(); ++i) {
            res += to_string(vec[i]) + " ";
        }
        saveResultToFile(file_name, res, suffix);
        return ;
    }

    pair<vector<double>, vector<double>> makeUniqueTestFromFile(std::string file_name, std::string patterns_file_name, bool check = checkAnswer) {
        std::ifstream file(file_name);
        if (!file) {
            std::cerr << "Cannot open the file: " << file_name << std::endl;
            return pair<vector<double>, vector<double>>();
        }
        std::string s((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        int n = s.length();

        // Преобразование строки в массив unsigned char
        std::vector<unsigned char> y(s.begin(), s.end());
        y.resize(n + XSIZE); // Для дополнительной памяти

        // Чтение паттернов из patterns_file_name
        std::ifstream patterns_file(patterns_file_name);
        if (!patterns_file) {
            std::cerr << "Cannot open the patterns file: " << patterns_file_name << std::endl;
            return pair<vector<double>, vector<double>>();
        }

        std::vector<std::string> patterns;
        std::string pattern;
        while (patterns_file >> pattern) {
            patterns.push_back(pattern);
        }

        // Преобразование паттернов в массивы unsigned char
        std::vector<std::vector<unsigned char>> patterns_uc;
        for (const auto& pat : patterns) {
            std::vector<unsigned char> pat_uc(pat.begin(), pat.end());
            patterns_uc.push_back(pat_uc);
        }

        std::vector<long long> answers(algorithms.size());
        // Тестирование для каждого паттерна
        int cnt_tests=0;
        while(true){
            int ind = rand() % patterns_uc.size();
            std::vector<unsigned char> pat_uc = patterns_uc[ind];
            int m = pat_uc.size();
            if (m==1){
                continue;
            }
            ++cnt_tests;
            if (cnt_tests==RoundsUnique){
                break;
            }
            unsigned char *x = pat_uc.data();
            if (check) {
                test(x, m, y.data(), n);
            }
            std::vector<long long> cur_answers = testAll(x, m, y.data(), n);
            for (int j = 0; j < algorithms.size(); ++j) {
                answers[j] += cur_answers[j];
            }
        }
        vector<double>res;
        for (int i = 0; i < answers.size(); ++i) {
            res.push_back((double)((long long)((double)answers[i] / (double)1e4)) / 100.0);
        }
        saveVectorToFile(file_name, res, "UniquePatternsResultsAbsolute");
        saveVectorToFile(file_name, vectorToRelation(res), "UniquePatternsResultsRelative");
        return {res, vectorToRelation(res)};
    }

    void saveResultToFile(std::string file_name, std::string res, std::string suffix) {
        // Определяем расположение точки в названии файла
        std::size_t pos = file_name.find_last_of(".");
        if (pos == std::string::npos) {
            std::cerr << "Invalid file name format: " << file_name << std::endl;
            return;
        }

        // Получаем имя файла без расширения
        std::string file_base_name = file_name.substr(0, pos);

        // Создаем новое имя файла
        std::string new_file_name = file_base_name + suffix + ".txt";

        // Открываем файл для записи
        std::ofstream output_file(new_file_name);
        if (!output_file) {
            std::cerr << "Error opening the file: " << new_file_name << std::endl;
            return;
        }

        // Записываем данные в файл
        output_file << res;

        // Закрываем файл
        output_file.close();

//        std::cout << "Results saved to file: " << new_file_name << std::endl;
    }

    void divide (vector<double>&vec, double n) {
        for (int i = 0; i < vec.size(); ++i) {
            vec[i] /= n;
        }
    }

    void divide(vector<vector<double>>&vec, double n) {
        for (int i = 0; i < vec.size(); ++i) {
            divide(vec[i], n);
        }
    }

    vector<double> matrixToVector(vector<vector<double>> mat) {
        if (mat.size()==0){
            return vector<double>();
        }
        vector<double> res;
        int m = mat[0].size();
        for (int j=0;j<m;++j){
            double sum = 0;
            for (int i=0;i<mat.size();++i){
                sum += mat[i][j];
            }
            res.push_back(sum);
        }
        return res;
    }

    void testFullRandomPatterns() {
        vector<string>file_names;
        file_names.push_back("tests/literature/LOTR/The Fellowship.txt");
        file_names.push_back("tests/literature/Ukraine/Lisova_pisnya.txt");
        file_names.push_back("tests/json/accounts.json");
        file_names.push_back("tests/rand2/rand2.txt");
        file_names.push_back("tests/rand32/rand32.txt");
        file_names.push_back("tests/rand64/rand64.txt");
        file_names.push_back("tests/genome/genome.txt");
        file_names.push_back("tests/html/largeHtml.html");
        file_names.push_back("tests/cfile/example.c");
        vector<vector<double>> absolute_results, relative_results;
        for (int i = 0; i < file_names.size(); ++i) {
            cout<<"File: "<<file_names[i]<<endl;
            if (i==0){
                pair<vector<vector<double>>, vector<vector<double>>>res = makeFullRandomTestFromFile(file_names[i], checkAnswer);
                absolute_results = res.first;
                relative_results = res.second;
            } else {
                pair<vector<vector<double>>, vector<vector<double>>>res = makeFullRandomTestFromFile(file_names[i], checkAnswer);
                absolute_results = add (absolute_results, res.first);
                relative_results = add (relative_results, res.second);
            }
        }
        divide(relative_results, file_names.size());
        saveMatrixResultsToFile("tests/final_results/Final.txt", absolute_results,"FullRandomPatternsResultsAbsolute");
        saveMatrixResultsToFile("tests/final_results/Final.txt", relative_results,"FullRandomPatternsResultsRelative");
        saveVectorToFile("tests/superfinal_results/SuperFinal.txt", matrixToVector(absolute_results), "FullRandomPatternsResultsAbsolute");
        vector<double>relativeVector = matrixToVector(relative_results);
        divide(relativeVector,relative_results.size());
        saveVectorToFile("tests/superfinal_results/SuperFinal.txt", relativeVector, "FullRandomPatternsResultsRelative");
    }

    void testRandomPatternFromText(){
        vector<string>file_names;
        file_names.push_back("tests/literature/LOTR/The Fellowship.txt");
        file_names.push_back("tests/literature/Ukraine/Lisova_pisnya.txt");
        file_names.push_back("tests/json/accounts.json");
        file_names.push_back("tests/rand2/rand2.txt");
        file_names.push_back("tests/rand32/rand32.txt");
        file_names.push_back("tests/rand64/rand64.txt");
        file_names.push_back("tests/genome/genome.txt");
        file_names.push_back("tests/html/largeHtml.html");
        file_names.push_back("tests/cfile/example.c");
        vector<vector<double>> absolute_results, relative_results;
        for (int i = 0; i < file_names.size(); ++i) {
            cout<<"File: "<<file_names[i]<<endl;
            if (i==0){
                pair<vector<vector<double>>, vector<vector<double>>>res = makeRandomTestFromFile(file_names[i], checkAnswer);
                absolute_results = res.first;
                relative_results = res.second;
            } else {
                pair<vector<vector<double>>, vector<vector<double>>>res = makeRandomTestFromFile(file_names[i], checkAnswer);
                absolute_results = add (absolute_results, res.first);
                relative_results = add (relative_results, res.second);
            }
        }
        divide(relative_results, file_names.size());
        saveMatrixResultsToFile("tests/final_results/Final.txt", absolute_results,"RandomPatternsResultsAbsolute");
        saveMatrixResultsToFile("tests/final_results/Final.txt", relative_results,"RandomPatternsResultsRelative");
        saveVectorToFile("tests/superfinal_results/SuperFinal.txt", matrixToVector(absolute_results), "RandomPatternsResultsAbsolute");
        vector<double>relativeVector = matrixToVector(relative_results);
        divide(relativeVector,relative_results.size());
        saveVectorToFile("tests/superfinal_results/SuperFinal.txt", relativeVector, "RandomPatternsResultsRelative");
    }

    void testUniquePatternFromText() {
        vector<pair<string, string>> file_names;
        file_names.push_back(
                {"tests/literature/LOTR/The Fellowship.txt", "tests/patternWordsDatasets/google-10000-english.txt"});
        file_names.push_back(
                {"tests/literature/Ukraine/Lisova_pisnya.txt", "tests/patternWordsDatasets/ukrainianWords.txt"});
        file_names.push_back(
                {"tests/json/accounts.json", "tests/patternWordsDatasets/jsonUniqueWords.txt"});
        file_names.push_back({"tests/genome/genome.txt", "tests/patternWordsDatasets/genomeUniqueWords.txt"});
        file_names.push_back({"tests/html/largeHtml.html", "tests/patternWordsDatasets/htmlUniqueWords.txt"});
        file_names.push_back({"tests/cfile/example.c", "tests/patternWordsDatasets/cUniqueWords.txt"});
        vector<double> absolute_results, relative_results;
        for (int i = 0; i < file_names.size(); ++i) {
            cout << "File: " << file_names[i].first << endl;
            cout<<"Subset: "<<file_names[i].second<<endl;
            if (i == 0) {
                pair<vector<double>, vector<double> > res = makeUniqueTestFromFile(file_names[i].first,
                                                                                   file_names[i].second, checkAnswer);
                absolute_results = res.first;
                relative_results = res.second;
            } else {
                pair<vector<double>, vector<double> > res = makeUniqueTestFromFile(file_names[i].first,
                                                                                   file_names[i].second, checkAnswer);
                absolute_results = add(absolute_results, res.first);
                relative_results = add(relative_results, res.second);
            }
        }
        divide(relative_results, file_names.size());
        saveVectorToFile("tests/superfinal_results/SuperFinal.txt", absolute_results,"UniquePatternsResultsAbsolute");
        saveVectorToFile("tests/superfinal_results/SuperFinal.txt", relative_results,"UniquePatternsResultsRelative");
    }

    string testWithMandSigmaSize(int m,int alphabestSize,int len = 1e5){
        vector<unsigned char> y(len+XSIZE);
        for (int i = 0; i < len; ++i) {
            y[i]=rand()%alphabestSize;
        }
        vector<long long > workingTimes(algorithms.size());
        for (int c = 0;c<RoundsRandom;++c){
            unsigned char *x= getSubstrOfLen(y, m);
            for (int i=0;i<algorithms.size();++i){
                long long cur = algorithms[i]->search_with_time(x,m,y.data(),len);
                workingTimes[i]+=cur;
            }
        }
        long long best =1e10,ind =-1;
        for (int i=0;i<algorithms.size();++i){
            if (workingTimes[i]<best){
                best = workingTimes[i];
                ind = i;
            }
        }
        return algorithms[ind]->name();
    }

    void testSigma(string outputFile){
        ofstream output(outputFile);
        if (!output.is_open()) {
            std::cerr << "Error opening file: " << outputFile << std::endl;
            return;
        }
        for (int i=0;i<algorithms.size();++i){
            output<<algorithms[i]->name()<<" ";
        }
        output<<"\n";
        for (int sigma = StartM; sigma <= SIGMA; sigma*=2) {
            output<<sigma<<" ";
        }
        output<<"\n";
        for (int m=StartM;m<=XSIZE;m*=2){
            output<<m<<" ";
            for (int sigma = StartM; sigma <= SIGMA; sigma*=2) {
                output<<testWithMandSigmaSize(m,sigma)<<" ";
            }
            output<<"\n";
        }
        output.close();
    }

    void TestAllAlgorithms() {
        testFullRandomPatterns();
        testRandomPatternFromText();
        testUniquePatternFromText();
        testSigma("tests/sigmaResults/sigma.txt");
    }
};

#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_ALGORITHMS_H
