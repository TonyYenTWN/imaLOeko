// read and write files
#pragma once

// STL
#include <fstream>
#include <string>

class reader_class{
    private:

    public:
        std::string read_one_line(std::string filename){
            std::ifstream in(filename);
            std::string line;
            std::string total = "";

            if(in){
                while(std::getline(in, line)){
                    total += line;
                    total += " ";
                }
            }

            in.close();

            return total;
        }
};
