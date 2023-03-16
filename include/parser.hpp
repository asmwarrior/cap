#ifndef CAP_PARSER_HPP
#define CAP_PARSER_HPP


#include <memory>
#include <ostream>
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
        TYPE_CHAR,
        TYPE_INT,
        TYPE_DOUBLE,
        TYPE_IDENTIFIER,
        TYPE_PTR,
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

        //print this ast node into the given stream
        virtual void print(size_t depth, std::basic_ostream<char>& stream) const = 0;
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

        void print(size_t depth, std::basic_ostream<char>& stream) const override {
            stream << value;
        }
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
        void print(size_t depth, std::basic_ostream<char>& stream) const override {
            stream << "type<void>";
        }
    };


    /**
     * Char type.
     */
    struct ASTTypeChar : public ASTTypename {
        void print(size_t depth, std::basic_ostream<char>& stream) const override {
            stream << "type<char>";
        }
    };


    /**
     * Int type.
     */
    struct ASTTypeInt : public ASTTypename {
        void print(size_t depth, std::basic_ostream<char>& stream) const override {
            stream << "type<int>";
        }
    };


    /**
     * Double type.
     */
    struct ASTTypeDouble : public ASTTypename {
        void print(size_t depth, std::basic_ostream<char>& stream) const override {
            stream << "type<double>";
        }
    };


    /**
     * Identifier type.
     */
    struct ASTTypeIdentifier : public ASTTypename {
        std::string name;

        void print(size_t depth, std::basic_ostream<char>& stream) const override {
            stream << "type_identifier<" << name << '>';
        }
    };


    /**
     * Ptr type.
     */
    struct ASTTypePtr : public ASTTypename {
        std::shared_ptr<ASTTypename> baseType;

        void print(size_t depth, std::basic_ostream<char>& stream) const override {
            stream << "type_ptr<";
            baseType->print(depth, stream);
            stream << '>';
        }
    };


    /**
     * AST enum member.
     */
    struct ASTEnumMember : ASTNode {
        //name 
        std::string name;

        void print(size_t depth, std::basic_ostream<char>& stream) const override {
            stream << "enum_member<"  << name << '>';
        }
    };


    /**
     * Enumeration.
     */
    struct ASTEnum : public ASTNode {
        //name 
        std::string name;

        //members
        std::vector<std::shared_ptr<ASTEnumMember>> members;

        void print(size_t depth, std::basic_ostream<char>& stream) const override {
            stream << "enum " << name << "{\n";
            for (const auto& member : members) {
                stream << std::string(depth, ' ');
                member->print(depth, stream);
            }
            stream << "}\n";
        }
    };


    /**
     * Struct member.
     */
    struct ASTStructMember : public ASTNode {
        //typename
        std::shared_ptr<struct ASTTypename> typename_;

        //name 
        std::string name;

        void print(size_t depth, std::basic_ostream<char>& stream) const override {
            stream << "struct_member<";
            typename_->print(depth, stream);
            stream << ' ' << name << ">; \n";
        }
    };


    /**
     * Struct.
     */
    struct ASTStruct : public ASTNode {
        //name 
        std::string name;

        //members
        std::vector<std::shared_ptr<ASTStructMember>> members;

        void print(size_t depth, std::basic_ostream<char>& stream) const override {
            stream << "struct " << name << " {\n";
            for (const auto& member : members) {
                stream << std::string(depth, ' ');
                member->print(depth, stream);
            }
            stream << "}\n";
        }
    };


    /**
     * Parse a series of tokens into an AST tree.
     */
    void parse(const std::vector<Token>& input, std::vector<ASTNodePtr>& output, std::vector<Error>& errors);


} //namespace cap


#endif //CAP_PARSER_HPP
