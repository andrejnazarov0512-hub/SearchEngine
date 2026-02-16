#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <iostream>
#include <vector>

int main(){
    ConverterJSON converter;
    std::vector<std::string> filepaths = converter.GetFilePaths();
    std::vector<std::string> textDocs = converter.GetTextDocuments(filepaths);
    
    
    InvertedIndex index;
    index.UpdateDocumentBase(textDocs);
    SearchServer server(index);
    converter.putAnswers(server.search(converter.GetRequests()));
   


}