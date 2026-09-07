#pragma once
#include <string>
#include <map>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <variant>
#include "shader.h"



std::vector<std::string> keywords = {
  "ShaderConfig",
  "ShaderProgramConfig"
};

enum TinyTokenType {
  Keyword,
  Seperator,
  Left_Bracket,
  Right_Bracket,
  Left_Curly,
  Righ_Curly,
  Comma,
  FileEnd,
  String,
  Int,
  Float,
  Double
};

using TokenValue =   std::variant<int,float,bool,std::string>;

class TinyToken {
  
  TokenValue value;
  public:
    TinyTokenType type;
    
    TinyToken(TinyTokenType type, TokenValue value) : type(type), value(value) {}
    TinyToken(TinyTokenType type) : type(type), value(nullptr) {}
};

class Parser {
    private:
      int current = 0; 
    public: 
        std::string data;
    bool isAtEnd() {
      return current < data.size();
    }
    char peek() {
      return data[current];
    }
    char advance() {
      return data[current++];
    }


    std::vector<TinyToken> lexing(std::string data) {
      std::vector<TinyToken> tokens;
      while(!isAtEnd()) {
        char c = advance();
        switch(c) {
          case '{':
            tokens.push_back({Left_Curly});
            break;
          case '}':
            tokens.push_back(Righ_Curly);
            break;
          case '[':
            tokens.push_back({Left_Bracket});
            break;
          case ']':
            tokens.push_back({Right_Bracket});
            break;
          case '\n':

            break;
          case ':':
            tokens.push_back({Seperator});
          case '"': {
            std::string value;
            while(isalpha(peek())) {
              value.push_back(advance());
            }
            tokens.push_back({String,value});
          }
          default:
            if(isalpha(c)) {
              std::string keyvalue{c};
              while(isalpha(peek())) {
                keyvalue.push_back(advance());
              }
              tokens.push_back({TinyTokenType::Keyword,keyvalue});
            }
            if(isalnum(c) || c == '.') {
              std::string value{c};
              while(isdigit(peek())) {
                value.push_back(advance());   
              }   
              if(peek() == '.') {
                value.push_back(advance());
                while(isdigit(peek())) {
                  value.push_back(advance());   
                }   
                tokens.push_back({TinyTokenType::Float,stof(value)});
              }
              else {
                tokens.push_back({TinyTokenType::Int,stoi(value)});
              }
        }
      }}
      return tokens;
    }
};