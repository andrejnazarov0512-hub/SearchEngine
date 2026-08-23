#include "InvertedIndex.h"
#include <cctype>
#include <thread>
#include <algorithm>
#include <iostream>
std::string Normalize(std::string word){
    std::transform(word.begin(), word.end(), word.begin(),
    [](unsigned char c){return std::tolower(c);});
    return word;
}

const std::vector<Entry>& InvertedIndex::GetWordCount(const std::string& word)const{
    if ( auto it = freq_dictionary.find(Normalize(word)); it != freq_dictionary.end()){
        return it->second;
    }
    return  empty;
}

void LocalMap(const std::string& doc, std::map<std::string, int>& out_map){
    std::string word;
    for (auto& symbol : doc){
        if (std::isalnum(static_cast<unsigned char>(symbol))){
            word+=static_cast<unsigned char>(symbol);
        }
        else if (!word.empty() ){
           out_map[Normalize(word)]++;
           word.clear();
        }
    }
    if (!word.empty()){
        out_map[Normalize(word)]++;
        word.clear();
    }
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs){
freq_dictionary.clear();
std::vector<std::thread> threads;
std::vector<std::map<std::string, int>> maps(input_docs.size());
    for (size_t i = 0; i < input_docs.size(); i++){
        threads.emplace_back(LocalMap,  std::ref(input_docs[i]), std::ref(maps[i]));
    }   
    for (auto& t : threads){
        t.join();
    }
    for (size_t i = 0; i < maps.size(); i++){
        for (auto const& [word, count] : maps[i]){
            freq_dictionary[word].push_back({i, (size_t)count});
           
        }
    }
}
