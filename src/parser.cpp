#include "parser.hpp"
#include "parserlib.hpp"


namespace cap {


    /**************************************************************************
       GRAMMAR
     **************************************************************************/


    using namespace parserlib;


    using rule = parserlib::rule<parse_context>;


    static auto name = terminal(TOKEN::IDENTIFIER) == AST::NAME;


    static auto type_void = terminal(TOKEN::VOID) == AST::TYPE_VOID;


    static auto type_int = terminal(TOKEN::INT) == AST::TYPE_INT;


    static auto type_double = terminal(TOKEN::DOUBLE) == AST::TYPE_DOUBLE;


    static auto typename_ = type_void
                          | type_int
                          | type_double;


    static auto enum_member = (name) == AST::ENUM_MEMBER; //TODO expression


    static auto enum_ = (terminal(TOKEN::ENUM) >> 
                        name >>
                        terminal(TOKEN::OPENING_CURLY_BRACKET) >> 
                        -(enum_member >> *(terminal(TOKEN::COMMA) >> enum_member)) >>
                        terminal(TOKEN::CLOSING_CURLY_BRACKET)) == AST::ENUM;


    static auto struct_member = (typename_ >> 
                                name >>
                                terminal(TOKEN::SEMICOLON)) == AST::STRUCT_MEMBER; //TODO init expression


    static auto struct_ = (terminal(TOKEN::STRUCT) >>
                          name >>
                          terminal(TOKEN::OPENING_CURLY_BRACKET) >>
                          *struct_member >>
                          terminal(TOKEN::CLOSING_CURLY_BRACKET)) == AST::STRUCT;


    static auto declaration = enum_
                            | struct_;


    static auto grammar = *declaration;


    /**************************************************************************
       PARSER
     **************************************************************************/


    using ASTNodeStack = std::vector<ASTNodePtr>;
    using ParseIterator = typename std::vector<parse_context::match>::const_iterator;


    template <class T> std::shared_ptr<T> pop_node(const ParseIterator& it, ASTNodeStack& stack, const char* tag) {
        if (stack.empty()) {
            throw Error{it->begin->position, std::string("Expected ") + tag};
        }
        
        std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(stack.back());
        if (!result) {
            throw Error{ it->begin->position, std::string("Expected ") + tag };
        }

        stack.pop_back();
        return result;
    }


    template <class T> std::shared_ptr<T> pop_node_opt(const ParseIterator& it, ASTNodeStack& stack, const char* tag) {
        if (stack.empty()) {
            throw Error{ it->begin->position, std::string("Expected ") + tag };
        }

        std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(stack.back());
        if (!result) {
            return nullptr;
        }

        stack.pop_back();
        return result;
    }


    template <class T> std::vector<std::shared_ptr<T>> pop_vector(const ParseIterator& it, ASTNodeStack& stack, const char* tag) {
        std::vector<std::shared_ptr<T>> result;
        while (auto elem = pop_node_opt<T>(it, stack, tag)) {
            result.push_back(elem);
        }
        return result;
    }


    static void create_ast_type_void(const ParseIterator& it, ASTNodeStack& stack) {
        stack.push_back(std::make_shared<ASTTypeVoid>());
    }


    static void create_ast_type_int(const ParseIterator& it, ASTNodeStack& stack) {
        stack.push_back(std::make_shared<ASTTypeInt>());
    }


    static void create_ast_type_double(const ParseIterator& it, ASTNodeStack& stack) {
        stack.push_back(std::make_shared<ASTTypeDouble>());
    }


    static void create_ast_name(const ParseIterator& it, ASTNodeStack& stack) {
        std::shared_ptr<ASTName> result{std::make_shared<ASTName>()};

        result->value = it->begin->content;
        
        stack.push_back(result);
    }


    static void create_ast_enum_member(const ParseIterator& it, ASTNodeStack& stack) {
        std::shared_ptr<ASTEnumMember> result{ std::make_shared<ASTEnumMember>() };        

        result->name = pop_node<ASTName>(it, stack, "enum member name")->value;
        //TODO enum member expression.
        
        stack.push_back(result);
    }


    static void create_ast_enum(const ParseIterator& it, ASTNodeStack& stack) {
        std::shared_ptr<ASTEnum> result{ std::make_shared<ASTEnum>() };
        
        result->members = pop_vector<ASTEnumMember>(it, stack, "enum member");        
        result->name = pop_node<ASTName>(it, stack, "enum name")->value;

        stack.push_back(result);
    }


    static void create_ast_struct_member(const ParseIterator& it, ASTNodeStack& stack) {
        std::shared_ptr<ASTStructMember> result{ std::make_shared<ASTStructMember>() };

        //TODO enum member expression.
        result->name = pop_node<ASTName>(it, stack, "struct member name")->value;
        result->typename_ = pop_node<ASTTypename>(it, stack, "struct member type");

        stack.push_back(result);
    }


    static void create_ast_struct(const ParseIterator& it, ASTNodeStack& stack) {
        std::shared_ptr<ASTStruct> result{ std::make_shared<ASTStruct>() };

        result->members = pop_vector<ASTStructMember>(it, stack, "struct member");
        result->name = pop_node<ASTName>(it, stack, "struct name")->value;

        stack.push_back(result);
    }


    void parse(const std::vector<Token>& input, std::vector<ASTNodePtr>& output, std::vector<Error>& errors) {
        //reset the output variable
        output.clear();

        //create the parse context
        auto pc = parse_context(input);

        //parse 
        const bool ok = parse(grammar, pc);

        //ast nod stack
        ASTNodeStack ast_stack;

        //process matches
        try {
            for (auto it = pc.matches.begin(); it != pc.matches.end(); ++it) {
                switch (it->tag) {
                    case AST::TYPE_VOID:
                        create_ast_type_void(it, ast_stack);
                        break;

                    case AST::TYPE_INT:
                        create_ast_type_int(it, ast_stack);
                        break;

                    case AST::TYPE_DOUBLE:
                        create_ast_type_double(it, ast_stack);
                        break;

                    case AST::NAME:
                        create_ast_name(it, ast_stack); 
                        break;

                    case AST::ENUM_MEMBER: 
                        create_ast_enum_member(it, ast_stack); 
                        break;

                    case AST::ENUM: 
                        create_ast_enum(it, ast_stack); 
                        break;

                    case AST::STRUCT_MEMBER:
                        create_ast_struct_member(it, ast_stack);
                        break;

                    case AST::STRUCT:
                        create_ast_struct(it, ast_stack);
                        break;

                    default: 
                        throw Error{ it->begin->position, "Invalid declaration" };

                }
            }
        }
        catch (const Error& error) {
            errors.push_back(error);
        }

        int x = 0;
    }


} //namespace cap
