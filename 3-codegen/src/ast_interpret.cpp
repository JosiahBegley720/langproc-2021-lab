#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());
    }    
    // TODO : Check for things matching reId
    if( regex_match(program->type, reId) ){
        std::map<std::string, int32_t>::const_iterator got = context.bindings.find(program->type);
        return got->second;
    }
    else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;  
    }
    else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;
    }    
    // TODO: Handle other constructs

    else if(program->type=="Assign"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::map<std::string, int32_t>::const_iterator got = context.bindings.find(program->value);
        if (got == context.bindings.end()){
            context.bindings.insert({program->value, val});
        }
        else{
            context.bindings.at(program->value) = val;
        }
        return val;  
    }
    else if(program->type=="Seq"){
        for(int i = 0; i < program->branches.size() - 1; i++){
            Interpret(context, program->branches.at(i));
        }
        return Interpret(context, program->branches.at(program->branches.size() - 1));
    }
    else if(program->type=="Add"){
        int32_t val1=Interpret(context, program->branches.at(0));
        int32_t val2=Interpret(context, program->branches.at(1));
        
        return val1 + val2;
    }
    else if(program->type=="Sub"){
        int32_t val1=Interpret(context, program->branches.at(0));
        int32_t val2=Interpret(context, program->branches.at(1));
        
        return val1 - val2;
    }
    else if(program->type=="LessThan"){
        int32_t val1=Interpret(context, program->branches.at(0));
        int32_t val2=Interpret(context, program->branches.at(1));
        if(val1 < val2){
            return 1;
        }
        else{
            return 0;
        }
    }
    else if(program->type=="If"){
        int32_t val1=Interpret(context, program->branches.at(0));
        if(val1){
            int32_t val2=Interpret(context, program->branches.at(1));
            return val2;
        }
        return Interpret(context, program->branches.at(2));
    }
    else if(program->type=="While"){
        while(Interpret(context, program->branches.at(0))){
            Interpret(context, program->branches.at(1));
        }
        return 0;
    }
    else if(program->type=="Input"){
        int32_t input;
        std::cin >> input;
        return input;
    }
    else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
