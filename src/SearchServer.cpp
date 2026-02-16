#include "SearchServer.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_set>
#include <set>
#include <algorithm>

struct WordData{
    WordData(std::string oth_word, std::vector<Entry> oth_vec) : word(oth_word), entries(oth_vec){}
    std::string word;
    std::vector<Entry> entries;
};

std::vector<std::string> GetUniqueWords(const std::string& query){
    std::unordered_set<std::string> unique_words;
    std::string word;

    for (auto& symbol : query){
         if (std::isalpha(symbol) || std::isdigit(symbol)){
            word+= static_cast<char>(std::tolower(static_cast<unsigned char>(symbol)));
        }
        else if (!word.empty()){
            unique_words.insert(word);
            word.clear();
        }
    }
    if (!word.empty()){
        unique_words.insert(word);
        word.clear();
    }
    return {unique_words.begin(), unique_words.end()};
}

void ProcessOneQuery(
    size_t max_responses,
    const std::string& query,
    std::vector<RelativeIndex>& ri,
    InvertedIndex& index)
{
    std::vector<std::string> uniqueVec = GetUniqueWords(query);
    std::vector<WordData> words;
    std::map<size_t, size_t> scores;

    for (auto& word : uniqueVec){
        words.emplace_back(word, index.GetWordCount(word));
    }
    std::sort(words.begin(), words.end(), [](const WordData& a, const WordData&b){
        return a.entries.size() < b.entries.size();
    });
    size_t max_abs = 0;
    for (auto& word : words){
        for (const auto& entry : word.entries){
            scores[entry.doc_ID] += entry.count;
            if (scores[entry.doc_ID] > max_abs){
                max_abs = scores[entry.doc_ID];
            }
        }
    }if (max_abs == 0){
        return;
    }
    for (std::map<size_t, size_t>::iterator it = scores.begin(); it != scores.end();
        it++){       
        ri.emplace_back(it->first, float(it->second)/float(max_abs));
    }

    std::sort(ri.begin(), ri.end(), [](const RelativeIndex& a, const RelativeIndex& b){
        return (a.rank != b.rank) ? a.rank > b.rank : a.doc_id < b.doc_id;
    });
    if (ri.size() > max_responses) ri.resize(max_responses);
   
}



std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> queries_input){
   
    std::vector<std::vector<RelativeIndex>> _search(queries_input.size());
    for (size_t i = 0; i < queries_input.size(); i+=8){
        std::vector<std::thread> threads;

        for (size_t j = i; j < i + 8 && j < queries_input.size(); j++){
            threads.emplace_back(ProcessOneQuery, max_responses, std::ref(queries_input[j]), std::ref(_search[j]), std::ref(_index));
        }

        for (std::thread& t : threads){
            t.join();
        }
    }
    
    std::cout<<"Counting rank..."<<std::endl;
    return _search;
    
}

