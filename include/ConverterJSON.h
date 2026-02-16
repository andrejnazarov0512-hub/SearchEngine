#pragma once
#include <vector>
#include <nlohmann/json.hpp>
#include <string>
#include "SearchServer.h"


using json = nlohmann::json;

class ConverterJSON {
public:
ConverterJSON();

std::vector<std::string> GetTextDocuments(const std::vector<std::string>& file_paths);

int GetResponsesLimit();
std::vector<std::string>& GetFilePaths(){return filepaths;}
std::vector<std::string> GetRequests();

void putAnswers( std::vector<std::vector<RelativeIndex>>
answers);

private:
std::vector<std::string> filepaths;
json j_file;
size_t max_responses = 5;
const std::string APP_VERSION = "0.1";
};