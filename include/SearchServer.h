#pragma once
#include <string>
#include "InvertedIndex.h"
#include <vector>
#include <map>

struct RelativeIndex{
    size_t doc_id;
    float rank;
    RelativeIndex(){}
    RelativeIndex(size_t id, float r) : doc_id(id), rank(r) {}
    bool operator ==(const RelativeIndex& other) const {
    return (doc_id == other.doc_id && rank == other.rank);
}
};
class SearchServer {
public:
    SearchServer(InvertedIndex& idx) : _index(idx){};
    std::vector<std::vector<RelativeIndex>> search(const
    std::vector<std::string> queries_input);
private:
    size_t max_responses = 5;
    InvertedIndex& _index;
    std::map<int, std::string> uniq_map;
};