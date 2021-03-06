//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef PARSER_H
#define PARSER_H

#include <Ice/Ice.h>
#include <Filesystem.h>
#include <list>
#include <stdio.h>

//
// Stuff for flex and bison
//

#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wconversion"
#endif

#define YYSTYPE std::list<std::string>
#define YY_DECL int yylex(YYSTYPE* yylvalp)
YY_DECL;
int yyparse();

//
// I must set the initial stack depth to the maximum stack depth to
// disable bison stack resizing. The bison stack resizing routines use
// simple malloc/alloc/memcpy calls, which do not work for the
// YYSTYPE, since YYSTYPE is a C++ type, with constructor, destructor,
// assignment operator, etc.
//
#define YYMAXDEPTH  10000
#define YYINITDEPTH YYMAXDEPTH // Initial depth is set to max depth, for the reasons described above.

//
// Newer bison versions allow to disable stack resizing by defining
// yyoverflow.
//
#define yyoverflow(a, b, c, d, e, f) yyerror(a)

class Parser
{
public:

    Parser(const std::shared_ptr<Filesystem::DirectoryPrx>&);

    void usage();
    void list(bool);
    void list(const std::shared_ptr<Filesystem::DirectoryPrx>&, bool, size_t);
    void createFile(const std::list<std::string>&);
    void createDir(const std::list<std::string>&);
    void pwd();
    void cd(const std::string&);
    void cat(const std::string&);
    void write(std::list<std::string>&);
    void destroy(const std::list<std::string>&);

    //
    // With older flex version <= 2.5.35 YY_INPUT second
    // paramenter is of type int&, in newer versions it
    // changes to size_t&
    //
    void getInput(char*, int&, size_t);
    void getInput(char*, size_t&, size_t);
    void continueLine();
    const char* getPrompt();

    void error(const char*);
    void error(const std::string&);

    void warning(const char*);
    void warning(const std::string&);

    int parse(bool = false);

private:

    std::list<std::shared_ptr<Filesystem::DirectoryPrx>> _dirs;

    bool _continue;
    int _errors;
};

extern Parser* parser; // The current parser for bison/flex

#endif
