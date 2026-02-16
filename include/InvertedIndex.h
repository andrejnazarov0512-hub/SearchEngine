#pragma once
#include <string>
#include <map>
#include <vector>


struct Entry{
size_t doc_ID, count;
// Данный оператор необходим для проведения тестовых сценариев
bool operator ==(const Entry& other) const {
return (doc_ID == other.doc_ID &&
count == other.count);
}
};


class InvertedIndex{
public:
InvertedIndex() = default;

void UpdateDocumentBase(std::vector<std::string> input_docs);

const std::vector<Entry>& GetWordCount(const std::string& word)const;
private:
std::vector<std::string> docs; // список содержимого документов
std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный
const std::vector<Entry> empty;
};

