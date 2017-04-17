/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void) {
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  } 
  else compileBlock2();
  assert("Block parsed!");
}

void compileBlock2(void) {
  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  } 
  else compileBlock3();
}

void compileBlock3(void) {
  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  } 
  else compileBlock4();
}

void compileBlock4(void) {
  compileSubDecls();
  compileBlock5();
}

void compileBlock5(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileConstDecls(void) {
  /* Tien */
  if (lookAhead->tokenType == TK_IDENT)
  {
    compileConstDecl();
    compileConstDecls();
  }
  /* Tien */
}

void compileConstDecl(void) {
  /* Tien */
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
  /* Tien */
}

void compileTypeDecls(void) {
  /* Tien */
  if (lookAhead->tokenType == TK_IDENT)
  {
    compileTypeDecl();
    compileTypeDecls();
  }
  /* Tien */
}

void compileTypeDecl(void) {
  /* Tien */
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
  /* Tien */
}

void compileVarDecls(void) {
  /* Tien */
  if (lookAhead->tokenType == TK_IDENT)
  {
    compileVarDecl();
    compileVarDecls();
  }
  /* Tien */
}

void compileVarDecl(void) {
  /* Tien */
  eat(TK_IDENT);
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
  /* Tien */
}

void compileSubDecls(void) {
  assert("Parsing subroutines ....");
  /* Quynh */
  /*if (lookAhead->tokenType == KW_FUNCTION)
  {
    compileFuncDecl();
    if (lookAhead->tokenType == KW_FUNCTION || lookAhead->tokenType == KW_PROCEDURE)
      compileSubDecls();
  }
  else if (lookAhead->tokenType == KW_PROCEDURE)
  {
    compileProcDecl();
    if (lookAhead->tokenType == KW_FUNCTION || lookAhead->tokenType == KW_PROCEDURE)
      compileSubDecls();
  }*/
  while (lookAhead->tokenType == KW_FUNCTION || lookAhead->tokenType == KW_PROCEDURE)
    if (lookAhead->tokenType == KW_FUNCTION)
      compileFuncDecl();
    else if (lookAhead->tokenType == KW_PROCEDURE)
      compileProcDecl();
  /* Quynh */
  assert("Subroutines parsed ....");
}

void compileFuncDecl(void) {
  assert("Parsing a function ....");
  /* Quynh */
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  /* Quynh */
  assert("Function parsed ....");
}

void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  /* Quynh */
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  /* Quynh */
  assert("Procedure parsed ....");
}

void compileUnsignedConstant(void) {
  /* Quynh */
  if (lookAhead->tokenType == TK_NUMBER)
    eat(TK_NUMBER);
  else if (lookAhead->tokenType == TK_IDENT)
    eat(TK_IDENT);
  else if (lookAhead->tokenType == TK_CHAR)
    eat(TK_CHAR);
  /* Quynh */
}

void compileConstant(void) {
  /* Quynh */
  if (lookAhead->tokenType == SB_PLUS)
  {
    eat(SB_PLUS);
    compileConstant2();
  }
  else if (lookAhead->tokenType == SB_MINUS)
  {
    eat(SB_MINUS);
    compileConstant2();
  }
  else if (lookAhead->tokenType == TK_CHAR)
    eat(TK_CHAR);
  else
    compileConstant2();
  /* Quynh */
}

void compileConstant2(void) {
  /* Quynh */
  if (lookAhead->tokenType == TK_IDENT)
    eat(TK_IDENT);
  else if (lookAhead->tokenType == TK_NUMBER)
    eat(TK_NUMBER);
  /* Quynh */
}

void compileType(void) {
  /* Huyen */
  if (lookAhead->tokenType == KW_INTEGER)
    eat(KW_INTEGER);
  else if (lookAhead->tokenType == KW_CHAR)
    eat(KW_CHAR);
  else if (lookAhead->tokenType == TK_IDENT)
    eat(TK_IDENT);
  else if (lookAhead->tokenType == KW_ARRAY)
  {
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);
    eat(SB_RSEL);
    eat(KW_OF);
    compileType();
  }
  /* Huyen */
}

void compileBasicType(void) {
  /* Huyen */
  if (lookAhead->tokenType == KW_INTEGER)
    eat(KW_INTEGER);
  else if (lookAhead->tokenType == KW_CHAR)
    eat(KW_CHAR);
  /* Huyen */
}

void compileParams(void) {
  /* Huyen */
  if (lookAhead->tokenType == SB_LPAR)
  {
    eat(SB_LPAR);
    compileParam();
    compileParams2();
    eat(SB_RPAR);
  }
  /* Huyen */
}

void compileParams2(void) {
  /* Huyen */
  if (lookAhead->tokenType == SB_SEMICOLON)
  {
    eat(SB_SEMICOLON);
    compileParam();
    compileParams2();
  }
  /* Huyen */
}

void compileParam(void) {
  /* Huyen */
  if (lookAhead->tokenType == TK_IDENT)
  {
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
  }
  else if (lookAhead->tokenType == KW_VAR)
  {
    eat(KW_VAR);
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
  }
  /* Huyen */
}

void compileStatements(void) {
  /* Huyen */
  compileStatement();
  compileStatements2();
  /* Huyen */
}

void compileStatements2(void) {
  /* Vu Anh */
  if (lookAhead->tokenType == SB_SEMICOLON)
  {
    eat(SB_SEMICOLON);
    compileStatement();
    compileStatements2();
  }
  /* Vu Anh */
}

void compileStatement(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  /* Vu Anh */
  if (lookAhead->tokenType == TK_IDENT)
  {
    eat(TK_IDENT);
    if (lookAhead->tokenType == SB_LSEL)
      compileIndexes();
    eat(SB_ASSIGN);
    compileExpression();
  }
  /* Vu Anh */
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  /* Vu Anh */
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments();
  /* Vu Anh */
  assert("Call statement parsed ....");
}

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  /* Vu Anh */
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  /* Vu Anh */
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE)
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  /* Vu Anh */
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  /* Vu Anh */
  assert("While statement parsed ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  /* Vu Anh */
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  /* Vu Anh */
  assert("For statement parsed ....");
}

void compileArguments_BNF(void) {
  /* Minh */
  if (lookAhead->tokenType == SB_LPAR)
  {
    eat(SB_LPAR);
    compileExpression();
    compileArguments2();
    eat(SB_RPAR);
  }
  /* Minh */
}

void compileArguments(void) {
  /* Hung */
  if (lookAhead->tokenType == SB_LPAR)
  {
    eat(SB_LPAR);
    compileExpression();
    while (lookAhead->tokenType == SB_COMMA)
    {
      eat(SB_COMMA);
      compileExpression();
    }
    eat(SB_RPAR);
  }
  /* Hung */
}

void compileArguments2(void) {
  /* Hung */
  if (lookAhead->tokenType == SB_COMMA)
  {
    eat(SB_COMMA);
    compileExpression();
    compileArguments2();
  }
  /* Hung */
}

void compileCondition(void) {
  /* Hung */
  compileExpression();
  compileCondition2();
  /* Hung */
}

void compileCondition2(void) {
  /* Hung */
  switch (lookAhead->tokenType)
  {
    case SB_EQ:
      eat(SB_EQ);
      break;
    case SB_NEQ:
      eat(SB_NEQ);
      break;
    case SB_LE:
      eat(SB_LE);
      break;
    case SB_LT:
      eat(SB_LT);
      break;
    case SB_GE:
      eat(SB_GE);
      break;
    case SB_GT:
      eat(SB_GT);
      break;
    default: break;
  }
  compileExpression();
  /* Hung */
}

void compileExpression_BNF(void) {
  assert("Parsing an expression");
  /* Minh */
  if (lookAhead->tokenType == SB_PLUS)
  {
    eat(SB_PLUS);
    compileExpression2();
  }
  else if (lookAhead->tokenType == SB_MINUS)
  {
    eat(SB_MINUS);
    compileExpression2();
  }
  else
    compileExpression2();
  /* Minh */
  assert("Expression parsed");
}

void compileExpression(void) {
  assert("Parsing an expression");
  /* Minh */
  if (lookAhead->tokenType == SB_PLUS)
    eat(SB_PLUS);
  else if (lookAhead->tokenType == SB_MINUS)
    eat(SB_MINUS);
  compileTerm();
  while (lookAhead->tokenType == SB_PLUS || lookAhead->tokenType == SB_MINUS)
  {
    if (lookAhead->tokenType == SB_PLUS)
      eat(SB_PLUS);
    else if (lookAhead->tokenType == SB_MINUS)
      eat(SB_MINUS);
    compileTerm();
  }
  /* Minh */
  assert("Expression parsed");
}

void compileExpression2(void) {
  /* Hung */
  compileTerm();
  compileExpression3();
  /* Hung */
}


void compileExpression3(void) {
  /* Hung */
  if (lookAhead->tokenType == SB_PLUS)
  {
    eat(SB_PLUS);
    compileTerm();
    compileExpression3();
  }
  else if (lookAhead->tokenType == SB_MINUS)
  {
    eat(SB_MINUS);
    compileTerm();
    compileExpression3();
  }
  /* Hung */
}

void compileTerm(void) {
  /* Minh */
  compileFactor();
  compileTerm2();
  /* Minh */
}

void compileTerm2(void) {
  /* Minh */
  if (lookAhead->tokenType == SB_TIMES)
  {
    eat(SB_TIMES);
    compileFactor();
    compileTerm2();
  }
  else if (lookAhead->tokenType == SB_SLASH)
  {
    eat(SB_SLASH);
    compileFactor();
    compileTerm2();
  }
  /* Minh */
}

void compileFactor(void) {
  /* Minh */
  if (lookAhead->tokenType == SB_LPAR)
  {
    eat(SB_LPAR);
    compileExpression();
    eat(SB_RPAR);
  }
  else if (lookAhead->tokenType == TK_IDENT)
  {
    eat(TK_IDENT);
    if (lookAhead->tokenType == SB_LPAR)
      compileArguments();
    else if (lookAhead->tokenType == SB_LSEL)
      compileIndexes();
  }
  else if (lookAhead->tokenType == TK_NUMBER)
    eat(TK_NUMBER);
  else if (lookAhead->tokenType == TK_CHAR)
    eat(TK_CHAR);
  /* Minh */
}

void compileIndexes(void) {
  /* Minh */
  if (lookAhead->tokenType == SB_LSEL)
  {
    eat(SB_LSEL);
    compileExpression();
    eat(SB_RSEL);
    compileIndexes();
  }
  /* Minh */
}

int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;
}
