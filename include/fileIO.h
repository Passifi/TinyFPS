#include <fstream> 

std::string loadFile(std::string path) {
    std::ifstream file;
    file.open(path);
    std::string result; 
    std::string line;  
    if(file.is_open()) {
        while(getline(file,line)) {
            result += line + "\n";
        }
    }
    else {
        throw std::runtime_error("Couldn't fine file");
    }
    return result;
}