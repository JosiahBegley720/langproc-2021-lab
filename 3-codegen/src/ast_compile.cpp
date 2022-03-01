#include "ast.hpp"

#include <string>
#include <regex>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}

void CompileRec(
    std::string destReg,    // The name of the register to put the result in
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match( program->type, reNum ) ){
        std::cout<<"const " <<destReg<<" "<<program->type<<std::endl;   
    }
    else if( regex_match( program->type, reId ) ){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->type<<" "<<zero<<std::endl;
    }
    else if(program->type=="Param"){
        std::cout<<"param "<<destReg<<" "<<program->value<<std::endl;
    }
    else if(program->type=="Seq"){
        for(unsigned i=0; i<program->branches.size(); i++){
            CompileRec(destReg, program->branches[i]);
        }
    }
    // TODO : handle the others
    else if(program->type=="Output"){
        CompileRec(destReg, program->branches[0]);
        std::cout << "output " << destReg << std::endl;
    }

    else if(program->type=="Add"){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        CompileRec(destReg, program->branches[0]);
        std::string srcReg1 = makeName("res");
        std::cout <<"add "<<srcReg1<<" "<<destReg<<" " << zero <<std::endl;
        CompileRec(destReg, program->branches[1]);
        std::string srcReg2 = makeName("res");
        std::cout <<"add "<<srcReg2<<" "<<destReg<<" " << zero <<std::endl;
        std::cout <<"add "<<destReg<<" "<<srcReg1<<" " << srcReg2 <<std::endl;
    }

    else if(program->type=="Sub"){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        CompileRec(destReg, program->branches[0]);
        std::string srcReg1 = makeName("res");
        std::cout <<"add "<<srcReg1<<" "<<destReg<<" " << zero <<std::endl;
        CompileRec(destReg, program->branches[1]);
        std::string srcReg2 = makeName("res");
        std::cout <<"sub "<<srcReg2<<" "<<destReg<<" " << zero <<std::endl;
        std::cout <<"sub "<<destReg<<" "<<srcReg1<<" " << srcReg2 <<std::endl;
    }

    else if(program->type=="Assign"){
        CompileRec(destReg, program->branches[0]);
        std::string srcReg = program->value;
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout << "add " << srcReg <<" "<< destReg << " " << zero << std::endl;
    }

    else if(program->type=="LessThan"){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        CompileRec(destReg, program->branches[0]);
        std::string srcReg1 = makeName("res");
        std::cout <<"add "<<srcReg1<<" "<<destReg<<" " << zero <<std::endl;
        CompileRec(destReg, program->branches[1]);
        std::string srcReg2 = makeName("res");
        std::cout <<"add "<<srcReg2<<" "<<destReg<<" " << zero <<std::endl;
        std::cout << "lt " << destReg << " " << srcReg1 << " " << srcReg2 << std::endl;
    }



    else if(program->type=="If"){
        CompileRec(destReg, program->branches[0]);
        std::string zero = makeName("zero");
        std::cout << "const " << zero << " 0 " << std::endl;
        std::string ELSE = makeName("ELSE");
        std::cout << "beq " << destReg << " " << zero << " " << ELSE << std::endl;
        CompileRec(destReg, program->branches[1]);
        std::string ENDIF = makeName("ENDIF");
        std::cout << "beq " << zero << " " << zero << " " << ENDIF << std::endl;
        std::cout << ":" << ELSE << std::endl;
        CompileRec(destReg, program->branches[2]);
        std::cout << ":" << ENDIF << std::endl;
    }

    else if(program->type=="While"){
        std::string LOOP = makeName("LOOP");
        std::cout << ":"<< LOOP << std::endl;
        CompileRec(destReg, program->branches[0]);
        std::string zero = makeName("zero");
        std::cout << "const " << zero << " 0 " << std::endl;
        std::string EXIT = makeName("EXIT");
        std::cout << "beq " << destReg << " " << zero << " " << EXIT << std::endl;
        CompileRec(destReg, program->branches[1]);
        std::cout << "beq " << zero << " " << zero << " " <<LOOP << std::endl;
        std::cout << ":" << EXIT << std::endl;
    }

    else if(program->type=="Input"){
        int32_t input;
        std::cin >> input;
        std::cout << "const " << destReg << " " << std::to_string(input) << std::endl;
        std::cout << "input " << destReg << std::endl;
    }

    else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}

void Compile(
    TreePtr program
){
    // Create a register to hold the final result
    std::string res=makeName("res");
    
    // Compile the whole thing
    CompileRec(res, program);
    
    // Put the result out
    std::cout<<"halt "<<res<<"\n";
}
