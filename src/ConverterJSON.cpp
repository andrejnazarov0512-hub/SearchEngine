#include <nlohmann/json.hpp>
#include "ConverterJSON.h"
#include <fstream>
#include <exception>
#include <iostream>

ConverterJSON::ConverterJSON(){
    std::ifstream file("../resources/config.json");
    if (!file.is_open()){
        throw std::runtime_error ("Config file is missing");
    }
    else {
        file >> j_file;
        if (!j_file.contains("config")){
            throw std::invalid_argument ("Config file is empty");
        }
        if (j_file["config"]["version"].get<std::string>() != APP_VERSION){
            std::cerr<<"Unespected version: ";
        }
        if (j_file["config"].contains("name")){
            std::cout<<"Starting "<<j_file["config"]["name"]<<std::endl;
        }
        else{
            std::cout<<"Starting SearchEngine\n";
        }
        
        max_responses = j_file.value("max_responses", 5);
        if (!j_file["config"].contains("files") || !j_file["config"]["files"].is_array()){
             throw std::runtime_error ("Missing files");
        }
        else{
            for (const auto& item : j_file["config"]["files"]){
                 if (!item.is_string()){
                throw std::runtime_error("Invalid file entry in config: expected string, got " + std::string(item.type_name()));
                }
                else{
                filepaths.push_back(item.get<std::string>());
                }    
            }  
        }
    }
}


std::vector<std::string> ConverterJSON::GetTextDocuments(const std::vector<std::string>& file_paths){
    std::string word;
    std::string text;
    std::vector<std::string> textDocuments;
    for (const auto& filepath : file_paths){
        std::ifstream file(filepath);
        text.clear();
        if (!file.is_open()){
            std::cerr<<"Wrong filepath: "<<filepath<<std::endl;
        }
        else {
            size_t count = 0;
            while (file>>word){
                if (word.length() > 100){
                    throw std::out_of_range("To long word in file: " + filepath);
                }
                text.append(word).append(" ");
                count++;
                if (count > 1000){
                    throw std::out_of_range("To much words in file: " + filepath);
                }

            }
        textDocuments.push_back(text);
        }
    }
return textDocuments;
}

std::vector<std::string> ConverterJSON::GetRequests(){
    std::vector<std::string> requests;
    std::ifstream file("../resources/requests.json");
    if (!file.is_open()){
        throw std::runtime_error ("Requests file is missing");
    }
    else {
        j_file.clear();
        file >> j_file;
        if (!j_file.contains("requests")){
             throw std::invalid_argument ("Requests file is empty");
        }
        else {
            std::cout<<"Getting requests...\n";
          
             for (const auto& item : j_file["requests"]){
                 if (!item.is_string()){
                throw std::runtime_error("Invalid file entry in requests: expected string, got " + std::string(item.type_name()));
                }
                else{
                requests.push_back(item.get<std::string>());
                }    
            }  
        }
    }
    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers){
    j_file.clear();
    
    
    std::ofstream file ("../resources/answers.json");
    if (file.is_open()){
        for (size_t i = 0; i < answers.size(); i++){
            std::string number = std::to_string(i);
            while (number.length() < 3){
                number = '0' + number;
            }
            std::string request = "request" + number;
            nlohmann::json one_request;
            if (answers[i].empty()){
                one_request["result"] = false;
            }
            else{
                one_request["result"] = true;

                size_t limit = std::min(answers[i].size(), static_cast<size_t>(max_responses));
                for (size_t j = 0; j < limit; j ++){
                    nlohmann::json req;
                    req["docid"] = answers[i][j].doc_id;
                    req["rank"] = answers[i][j].rank;
                    one_request["relevance"].push_back(req);
                }
            }
            j_file["answers"][request] = one_request;
           
        }
        file<<j_file.dump(4);
    }
}