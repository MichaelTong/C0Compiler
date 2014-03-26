C0Compiler
==========

A basic compiler for C0 language without pointers


Grammer
=======
<Addition> ::= +｜-
<Multiplication>  ::= *｜/
<Relation>  ::=  <｜<=｜>｜>=｜!=｜==
<Alphabet>   ::= ＿｜a｜．．．｜z｜A｜．．．｜Z
<Digit>   ::= 0｜<Nondigit>
<Nondigit>  ::= 1｜...｜9
<Character>   ::=  '<Addition>'｜'<Multiplication>'｜'<Alphabet>'｜'<Digit>'
<Strings>   ::=  "{ASCII chars with decimal code with 32,33,35-126}"                              
<Program>    ::= ［<ConstDeclaration>］［<VariablesDeclaration>］{<ValuedFunctionDeclaration>|<NonValuedFunctionDeclaration>}<Main>
<ConstDeclaration> ::=  const<ConstDefinition>;{ const<ConstDefinition>;}
<ConstDefinition>   ::=   int<Identifier>＝<Int>{,<Identifier>＝<Int>}
                            | char<Identifier>＝<Character>{,<Identifier>＝<Character>}
<UnsignedInt>  ::= <Nondigit>{<Digit>}
<Int>        ::= ［＋｜－］<UnsignedInt>｜0
<Identifier>    ::=  <Alphabet>｛<Alphabet>｜<Digit>｝
<DeclarationHead>   ::=  int<Identifier>|char<Identifier>
<VariablesDeclaration>  ::= <VariableDefinition>;{<VariableDefinition>;}
<VariableDefinition>  ::= <TypeIdentifier>(<Identifier>|<Identifier>‘[’<UnsignedInt>‘]’){,<Identifier>|<Identifier>‘[’<UnsignedInt>‘]’ }
<TypeIdentifier>      ::=  int | char
<ValuedFunctionDeclaration>  ::=  <DeclarationHead>‘(’<Parameter>‘)’ ‘{’<ComplicatedSentence>‘}’
<NonValuedFunctionDeclaration>  ::= void<Identifier>‘(’<Parameter>‘)’‘{’<ComplicatedSentence>‘}’
<ComplicatedSentence>   ::=  ［<ConstDeclaration>］［<VariablesDeclaration>］<SentencesList>
<Parameter>    ::= <ParameterList>
<ParameterList>    ::=  <TypeIdentifier><Identifier>{,<TypeIdentifier><Identifier>}| <None>
<Main>    ::= void main‘(’‘)’ ‘{’<ComplicatedSentence>‘}’
<Expression>    ::= ［＋｜－］<Item>{<Addition><Item>}
<Item>     ::= <Factor>{<Multiplication><Factor>}
<Factor>    ::= <Identifier>｜<Identifier>‘[’<Expression>‘]’｜<Int>|<Character>｜<ValuedFunctionCall>|‘(’<Expression>‘)’
<Sentence>    ::= <ConditionSent>｜<LoopSent>｜‘{’<SentencesList>‘}’｜<ValuedFunctionCall>; 
                      |<NonValuedFunctionCall>;｜<Assinment>;｜<Read>;｜<Write>;｜<None>;｜<Return>;
<Assinment>   ::=  <Identifier>＝<Expression>|<Identifier>‘[’<Expression>‘]’=<Expression>
<ConditionSent>  ::=  if ‘(’<Condition>‘)’<Sentence>［else<Sentence>］
<Condition>    ::=  <Expression><Relation><Expression>｜<Expression> //False if expression is zero, otherwise true
<LoopSent>   ::=  do<Sentence>while ‘(’<Condition>‘)’ |for‘(’<Identifier>＝<Expression>;<Condition>;<Identifier>＝<Identifier>(+|-)<Step>‘)’<Sentence>
<Step>::= <UnsignedInt>  
<ValuedFunctionCall> ::= <Identifier>‘(’<ValueParameterList>‘)’
<NonValuedFunctionCall> ::= <Identifier>‘(’<ValueParameterList>‘)’
<ValueParameterList>   ::= <Expression>{,<Expression>}｜<None>
<SentencesList>   ::={<Sentence>}
<Read>    ::=  scanf ‘(’<Identifier>{,<Identifier>}‘)’
<Write>    ::=  printf‘(’<String>,<Expression>‘)’|printf ‘(’<String>‘)’|printf ‘(’<Expression>‘)’
<Return>   ::=  return[‘(’<Expression>‘)’]

