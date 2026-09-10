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
  "Shader",
  "ShaderProgram",
  "Type",
  "Uniforms",
  "Source",  
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
  
  public:

    TokenValue value;
    TinyTokenType type;
    
    TinyToken(TinyTokenType type, TokenValue value) : type(type), value(value) {}
    TinyToken(TinyTokenType type) : type(type), value(nullptr) {}
};

class Lexer {
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
      tokens.push_back({FileEnd});
      return tokens;
    }
  
};

class Parser {
    std::vector<ShaderConfig> shaderConfigs; 
    std::vector<ShaderProgramConfig> shaderProgramConfigs;
    std::vector<TinyToken> _tokens; 
    unsigned int current = 0;
    bool isAtEnd() {
      if(current >= _tokens.size() || _tokens[current].type == FileEnd ) {
        return true;
      }  
      else {
        return false;
      }
    }
    TinyToken peek() {
      if(!isAtEnd()) 
      {
        return _tokens[current];
      }
      else {
        return TinyToken(FileEnd);
      }
    }
    TinyToken advance() {
      if(!isAtEnd()) {
        return _tokens[current++];
      }
      else {
        return TinyToken(FileEnd);
      }
    }

    void processKeyword(TinyToken t) {
      std::string* value = nullptr; 
      value = std::get_if<std::string>(&t.value);
      if(value == nullptr) {
        throw new std::exception;
      }

    }
    void parse(std::vector<TinyToken> tokens) {
        _tokens = tokens;
        auto token = advance();
        switch(token.type) {
          case Keyword:
            processKeyword();
            break; 
          case 
        }
    }
  };