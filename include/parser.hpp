#ifndef CAP_PARSER_HPP
#define CAP_PARSER_HPP


#include <memory>
#include "lexer.hpp"


namespace parserlib {
    template <typename Input, typename Tag, typename Output> class parse_context;
}


namespace cap {


    /**
     * AST type enumeration.
     */
    enum class AST {
        TYPE_VOID,
        TYPE_INT,
        TYPE_DOUBLE,
        NAME,
        ENUM_MEMBER,
        ENUM,
        STRUCT_MEMBER,
        STRUCT
    };


    /**
     * Parser parse context.
     */
    using parse_context = parserlib::parse_context<std::vector<Token>, AST, std::vector<Token>>;


    /**
     * AST node base.
     */
    struct ASTNode {
        //position into the original text.
        Position position;

        //virtual destructor due to inheritance.
        virtual ~ASTNode() {}
    };


    /**
     * AST node ptr type.
     */
    using ASTNodePtr = std::shared_ptr<ASTNode>;


    /**
     * AST name.
     */
    struct ASTName : ASTNode {
    public:
        std::string value;
    };


    /**
     * Base class for typenames.
     */
    struct ASTTypename : ASTNode {
    };


    /**
     * Void type.
     */
    struct ASTTypeVoid : ASTTypename {
    };


    /**
     * Int type.
     */
    struct ASTTypeInt : public ASTTypename {
    };


    /**
     * Double type.
     */
    struct ASTTypeDouble : public ASTTypename {
    };


    /**
     * AST enum member.
     */
    struct ASTEnumMember : ASTNode {
        //name 
        std::string name;
    };


    /**
     * Enumeration.
     */
    struct ASTEnum : public ASTNode {
        //name 
        std::string name;

        //members
        std::vector<std::shared_ptr<ASTEnumMember>> members;
    };


    /**
     * Struct member.
     */
    struct ASTStructMember : public ASTNode {
        //typename
        std::shared_ptr<struct ASTTypename> typename_;

        //name 
        std::string name;
    };


    /**
     * Struct.
     */
    struct ASTStruct : public ASTNode {
        //name 
        std::string name;

        //members
        std::vector<std::shared_ptr<ASTStructMember>> members;
    };


    /**
     * Parse a series of tokens into an AST tree.
     */
    void parse(const std::vector<Token>& input, std::vector<ASTNodePtr>& output, std::vector<Error>& errors);


} //namespace cap


#endif //CAP_PARSER_HPP
