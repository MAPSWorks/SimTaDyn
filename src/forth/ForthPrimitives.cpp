#include "ForthInner.hpp"

#  define BINARY_OP(op) { m_tos = DDROP() op m_tos; } // Pop a value, apply it the operation op with the content of the register tos (Top Of Stack)
#  define LOGICAL_OP(op) { m_tos = -1 * (DDROP() op m_tos); }

// **************************************************************
//
// **************************************************************
std::string Forth::getWord()
{
  if (!READER.hasMoreWords())
    {
      ForthReaderTruncatedFile e(READER.file());
      throw e;
    }
  return READER.nextWord();
}

// **************************************************************
//
// **************************************************************
void Forth::execPrimitive(const Cell16 idPrimitive)
{
  //std::cout << "execPrimitive " << idPrimitive << std::endl;
  switch (idPrimitive)
    {
      // Dummy word / No operation
    case FORTH_PRIMITIVE_NOP:
      std::cout << "NOP\n";
      break;

      // Begin of commentary
    case FORTH_PRIMITIVE_LPARENTHESIS:
      m_saved_state = m_state;
      m_state = COMMENT_STATE;
      break;

      // End of commentary
    case FORTH_PRIMITIVE_RPARENTHESIS:
      m_state = m_saved_state;
      break;

      // Line of commentary
    case FORTH_PRIMITIVE_COMMENTARY:
      READER.skipLine();
      break;

      // Begin the definition of a new word
    case FORTH_PRIMITIVE_COLON:
      {
        // Extract next word
        m_creating_word = getWord();
        if (m_dico.exists(m_creating_word))
          {
            std::cout << YELLOW << "[WARNING] Redefining '" << m_creating_word << "'" << DEFAULT << std::endl;
          }

        // Save informations which will be checked
        // when executing the SEMI_COLON primitive.
        m_last_at_colon = m_dico.m_last;
        m_here_at_colon = m_dico.m_here;
        m_depth_at_colon = DStackDepth();

        // Add it in the dictionary
        Cell16 token = m_dico.here() + m_creating_word.size() + 1U + 2U; // 1: flags, 2: NFA
        m_dico.add(token, m_creating_word, 0);

        // Compilation mode
        m_state = COMPILATION_STATE;
      }
      break;

      // End the definition of a new word
    case FORTH_PRIMITIVE_SEMICOLON:
      m_dico.appendCell16(FORTH_PRIMITIVE_EXIT); // FIXME check size word definition
      m_state = EXECUTION_STATE;
      if (m_depth_at_colon != DStackDepth())
        {
          m_dico.m_last = m_last_at_colon;
          m_dico.m_here = m_here_at_colon;
          ForthUnbalancedDef e(m_creating_word); throw e;
        }
      break;

      // Set immediate the last word
    case FORTH_PRIMITIVE_IMMEDIATE:
      m_dico.m_dictionary[m_dico.m_last] |= FLAG_IMMEDIATE;
      break;

    case FORTH_PRIMITIVE_INCLUDE:
      // Restore TOS register (because execToken() poped TOS)
      DPUSH(m_tos);

      includeFile(getWord());

      DPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_SMUDGE:
      {
        std::string word = getWord();
        if (false == m_dico.smudge(word))
          {
            std::cout << YELLOW << "[WARNING] Unknown word '"
                      << word << "'. Word SMUDGE Ignored !"
                      << DEFAULT << std::endl;
          }
      }
      break;

    case FORTH_PRIMITIVE_TICK:
      {
        std::string word = getWord();
        Cell16 token;
        bool immediate;
        if (false == m_dico.find(word, token, immediate))
          {
            token = 0;
            std::cout << YELLOW << "[WARNING] Unknown word '"
                      << word << "'. Word TICK Ignored !"
                      << DEFAULT << std::endl;
          }
        DPUSH(m_tos);
        m_tos = token;
      }
      break;

     case FORTH_PRIMITIVE_COMPILE:
       m_ip += 2;
       m_dico.appendCell16(m_dico.read16at(m_ip));
       break;

     case FORTH_PRIMITIVE_EXECUTE:
       execToken(m_tos);
       break;

     case FORTH_PRIMITIVE_LBRACKET:
       m_state = EXECUTION_STATE;
       break;

    case FORTH_PRIMITIVE_RBRACKET:
       m_state = COMPILATION_STATE;
       break;

    case FORTH_PRIMITIVE_HERE:
      DPUSH(m_tos);
      m_tos = m_dico.here();
      break;

    case FORTH_PRIMITIVE_ALLOT:
      m_dico.allot((int32_t) m_tos);
      break;

      // Reserve one cell of data space and store x in the cell.
    case FORTH_PRIMITIVE_COMMA:
      m_dico.appendCell16(m_tos);
      DPOP(m_tos);
      break;

      // ( a-addr -- x )
      // x is the value stored at a-addr.
    case FORTH_PRIMITIVE_FETCH:
      m_tos = m_dico.read32at(m_tos);
      break;

      // Store x at a-addr.
      // ( x a-addr -- )
    case FORTH_PRIMITIVE_STORE:
      DPOP(m_tos1);
      m_dico.write32at(m_tos, m_tos1);
      break;

      // Restore the IP when interpreting the definition
      // of a non primitive word
    case FORTH_PRIMITIVE_EXIT:
      RPOP(m_ip);
      break;

      // Change IP
    case FORTH_PRIMITIVE_BRANCH:
      m_ip += m_dico.read16at(m_ip + 2U);
      // Do not forget that m_ip will be += 2 next iteration
      break;

      // Change IP if top of stack is 0
    case FORTH_PRIMITIVE_0BRANCH:
      if (0 == m_tos)
        {
          m_ip += m_dico.read16at(m_ip + 2U);
        }
      else
        {
          m_ip += 2U;
        }
      // Do not forget that m_ip will be += 2 next iteration
      DPOP(m_tos);
      break;

      // Move x to the return stack.
      // ( x -- ) ( R: -- x )
    case FORTH_PRIMITIVE_TO_RSTACK:
      RPUSH(m_tos);
      DPOP(m_tos);
      break;

      // Transfer cell pair x1 x2 to the return stack
      // ( x1 x2 -- ) ( R: -- x1 x2 )
    case FORTH_PRIMITIVE_2TO_RSTACK:
      DPOP(m_tos1);
      RPUSH(m_tos1);
      RPUSH(m_tos);
      DPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_FROM_RSTACK:
      DPUSH(m_tos);
      RPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_2FROM_RSTACK:
      DPUSH(m_tos);
      RPOP(m_tos);
      RPOP(m_tos1);
      DPUSH(m_tos1);
      break;

    case FORTH_PRIMITIVE_CELL:
      DPUSH(m_tos);
      m_tos = sizeof (Cell32);
      break;

    case FORTH_PRIMITIVE_CELLS:
      m_tos = m_tos * sizeof (Cell32);
      break;

    case FORTH_PRIMITIVE_LITERAL_16:
      DPUSH(m_tos);
      m_ip += 2U; // Skip primitive LITTERAL
      m_tos = m_dico.read16at(m_ip);
      break;

    case FORTH_PRIMITIVE_LITERAL_32:
      DPUSH(m_tos);
      m_ip += 2U; // Skip primitive LITTERAL
      m_tos = m_dico.read32at(m_ip);
      m_ip += 2U; // Skip the number - 2 because of next ip
      break;

    case FORTH_PRIMITIVE_1MINUS:
      m_tos--;
      break;
    case FORTH_PRIMITIVE_1PLUS:
      m_tos++;
      break;
    case FORTH_PRIMITIVE_2MINUS:
      m_tos -= 2;
      break;
    case FORTH_PRIMITIVE_2PLUS:
      m_tos += 2;
      break;

      // drop ( a -- )
    case FORTH_PRIMITIVE_DROP:
      DPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_DEPTH:
      DPUSH(m_tos);
      m_tos = DStackDepth();
      break;

      // nip ( a b -- b ) swap drop ;
    case FORTH_PRIMITIVE_NIP:
      DPOP(m_tos);
      break;

      // ( ... n -- sp(n) )
    case FORTH_PRIMITIVE_PICK:
      m_tos = DPICK(m_tos);
      break;

      // dup ( a -- a a )
    case FORTH_PRIMITIVE_DUP:
      DPUSH(m_tos);
      break;

    case FORTH_PRIMITIVE_QDUP:
      if (m_tos)
        {
          DPUSH(m_tos);
        }
      break;

      // swap ( a b -- b a )
    case FORTH_PRIMITIVE_SWAP:
      m_tos2 = m_tos;
      DPOP(m_tos);
      DPUSH(m_tos2);
      break;

      // over ( a b -- a b a )
    case FORTH_PRIMITIVE_OVER:
      DPUSH(m_tos);
      m_tos = DPICK(1);
      break;

      // rot ( a b c -- b c a )
    case FORTH_PRIMITIVE_ROT:
      DPOP(m_tos2);
      DPOP(m_tos3);
      DPUSH(m_tos2);
      DPUSH(m_tos);
      m_tos = m_tos3;
      break;

      // tuck ( a b -- b a b ) swap over ;
    case FORTH_PRIMITIVE_TUK:
      DPOP(m_tos2);
      DPUSH(m_tos);
      DPUSH(m_tos2);
      break;

      // 2dup ( a b -- a b a b ) over over ;
    case FORTH_PRIMITIVE_2DUP:
      DPUSH(m_tos);
      m_tos2 = DPICK(1);
      DPUSH(m_tos2);
      break;

      // 2over ( a b c d -- a b c d a b )
    case FORTH_PRIMITIVE_2OVER:
      DPUSH(m_tos);
      m_tos2 = DPICK(3);
      DPUSH(m_tos2);
      m_tos = DPICK(3);
      break;

      // 2swap ( a b c d -- c d a b )
    case FORTH_PRIMITIVE_2SWAP:
      DPOP(m_tos2);
      DPOP(m_tos3);
      DPOP(m_tos4);
      DPUSH(m_tos3);
      DPUSH(m_tos4);
      DPUSH(m_tos2);
      break;

      // 2drop ( a b -- ) drop drop ;
    case FORTH_PRIMITIVE_2DROP:
      DPOP(m_tos);
      DPOP(m_tos);
      break;

      //
    case FORTH_PRIMITIVE_BINARY:
      changeDisplayBase(2U);
      break;
    case FORTH_PRIMITIVE_OCTAL:
      changeDisplayBase(8U);
      break;
    case FORTH_PRIMITIVE_HEXADECIMAL:
      changeDisplayBase(16U);
      break;
    case FORTH_PRIMITIVE_DECIMAL:
      changeDisplayBase(10U);
      break;

      // gforth 16 BASE !
      // A BASE !
      // Mais SimForth: 16 BASE A BASE
    case FORTH_PRIMITIVE_BASE:// FIXME USER VARIABLE
      if (false == changeDisplayBase(m_tos))
        {
          std::cerr << YELLOW << "[WARNING] '"
                    << m_tos << "' is an invalid base and shall be [2..36]. Ignored !"
                    << DEFAULT << std::endl;
        }
      break;

    case FORTH_PRIMITIVE_PLUS:
      BINARY_OP(+);
      break;
    case FORTH_PRIMITIVE_MINUS:
      BINARY_OP(-);
      break;
    case FORTH_PRIMITIVE_DIV:
      BINARY_OP(/);
      break;
    case FORTH_PRIMITIVE_TIMES:
      BINARY_OP(*);
      break;
    case FORTH_PRIMITIVE_RSHIFT:
      BINARY_OP(>>);
      break;
    case FORTH_PRIMITIVE_LSHIFT:
      BINARY_OP(<<);
      break;
    case FORTH_PRIMITIVE_FALSE:
      m_tos = 0;
      DPUSH(m_tos);
      break;
    case FORTH_PRIMITIVE_TRUE:
      m_tos = -1;
      DPUSH(m_tos);
      break;
    case FORTH_PRIMITIVE_GREATER_EQUAL:
      LOGICAL_OP(>=);
      break;
    case FORTH_PRIMITIVE_LOWER_EQUAL:
      LOGICAL_OP(<=);
      break;
    case FORTH_PRIMITIVE_GREATER:
      LOGICAL_OP(>);
      break;
    case FORTH_PRIMITIVE_LOWER:
      LOGICAL_OP(<);
      break;
    case FORTH_PRIMITIVE_EQUAL:
      LOGICAL_OP(==);
      break;
    case FORTH_PRIMITIVE_0EQUAL:
      DPUSH(0U);
      LOGICAL_OP(==);
      break;
    case FORTH_PRIMITIVE_NOT_EQUAL:
      LOGICAL_OP(!=);
      break;
    case FORTH_PRIMITIVE_AND:
      BINARY_OP(&);
      break;
    case FORTH_PRIMITIVE_OR:
      BINARY_OP(|);
      break;
    case FORTH_PRIMITIVE_XOR:
      BINARY_OP(^);
      break;
    case FORTH_PRIMITIVE_MIN:
      DPOP(m_tos1);
      m_tos = (m_tos < m_tos1) ? m_tos : m_tos1;
      break;
    case FORTH_PRIMITIVE_MAX:
      DPOP(m_tos1);
      m_tos = (m_tos > m_tos1) ? m_tos : m_tos1;
      break;
    case FORTH_PRIMITIVE_DISP:
      std::cout << std::setbase(m_base) << (int32_t) m_tos << " ";
      DPOP(m_tos);
      break;
    case FORTH_PRIMITIVE_UDISP:
      std::cout << std::setbase(m_base) << (uint32_t) m_tos << " ";
      DPOP(m_tos);
      break;
    case FORTH_PRIMITIVE_CARRIAGE_RETURN:
      std::cout << std::endl;
      break;
    case FORTH_PRIMITIVE_DISPLAY_DSTACK:
      DPUSH(m_tos);
      displayDStack();
      DPOP(m_tos);
      break;
    default:
      ForthUnknownPrimitive e(idPrimitive, __PRETTY_FUNCTION__); throw e;
      break;
    }
}
