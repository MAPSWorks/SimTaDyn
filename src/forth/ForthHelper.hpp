#ifndef FORTH_HELPER_HPP_
#  define FORTH_HELPER_HPP_

#  include "Logger.hpp"
#  include <cstdint>
#  include <cassert>
#  include <string>
#  include <iomanip>

// **************************************************************
//
// Virtual machine
//
// **************************************************************

// Forth Type
typedef uint32_t       Cell32;
typedef uint16_t       Cell16;
typedef uint8_t        Cell8;

// Memory cast
#  define ADDR8(x)       (reinterpret_cast<Cell8*>(x))
#  define CELL8(x)       (reinterpret_cast<Cell8>(x))
#  define CELL16(x)      (reinterpret_cast<Cell16>(x))
#  define CELL32(x)      (reinterpret_cast<Cell32>(x))
#  define CADDR(x)       (reinterpret_cast<Cell16*>(x))

#  define TOKEN2ADDR(a) (CADDR(a + dictionary))

//! Memory for data stack, return stack
#  define STACK_SIZE     (1024U) // of Cells

//! Rerserve additional bytes to detect stack underflows and prevent
//! data corruption.
#  define STACK_UNDERFLOW_MARGIN (8U) // bytes
//ASSERT_COMPILE_TIME(STACK_UNDERFLOW_MARGIN < STACK_SIZE)

// Memory for the dictionnary
// 64 Ko is the maximal reachable address with a 16-bits word
// FIXME: Maybe we can divide / 2 addresses because they are aligned.
#  define DICTIONARY_SIZE (64U * 1024U) // of bytes

//
#  define CELL16_MAX_VALUE (65535U)
// FIXME: COMPIL_ASSERT_TIME((DICTIONARY_SIZE - 1U) <= CELL16_MAX_VALUE)

// Used for aligning 32-bits addresses
#  define NEXT_MULTIPLE_OF_4(x) (((x) + 3) & ~0x03)

// Used for aligning 16-bits addresses
#  define NEXT_MULTIPLE_OF_2(x) (((x) + 1) & ~0x01)

// Padding
#define UINT32_MASK  ((sizeof (Cell32) - 1))
#define UINT16_MASK  ((sizeof (Cell16) - 1))

// **************************************************************
//
// Stack manipulation
//
// **************************************************************

// Data stack (function parameters manipulation)
#  define DPUSH(n) (*(m_dsp++) = CELL32(n))  // Store the number n on the top of stack
#  define DDROP()  (*(--m_dsp))              // Discard the top of the stack
#  define DPOP(r)  (r = DDROP())             // Discard the top of the stack, save its value in the register r
#  define DPICK(n) (*(m_dsp - n - 1))        // Look at the nth element (n >= 1) of the stack from the top (1 = 1st element)

// Alternative data stack (function parameters manipulation)
#  define APUSH(n) (*(m_asp++) = CELL32(n))  // Store the number n on the top of stack
#  define ADROP()  (*(--m_asp))              // Discard the top of the stack
#  define APOP(r)  (r = ADROP())             // Discard the top of the stack, save its value in the register

// Return stack (store calling functions (tokens))
#  define RPUSH(a)  (*(m_rsp++) = CELL32(a)) // Store an address a on the top of stack
#  define RPOP(r)   (r = *(--m_rsp))         // Discard the top of the stack
#  define RPICK(n)  (*(m_rsp - n - 1))       // Look at the nth element (n >= 1) of the stack from the top (1 = 1st element)

// **************************************************************
//
// Forth word flags
//
// **************************************************************

#  define FLAG_PRECEDENCE (0x80) // A word has always this bit set (historical)
#  define FLAG_IMMEDIATE  (0x40) // A word immediate is interpreted during the compilation
#  define FLAG_SMUDGE     (0x20) // When set this word is forgotten by the dictionnary

#  define MASK_FORTH_NAME_SIZE  (0x1F) // 5 first bits are used to store the word name length
#  define MASK_FORTH_FLAGS      (0x7F)

// **************************************************************
//
// Enums
//
// **************************************************************

namespace forth
{
  //! \brief Stack names: primary and secondary parameter stack, CFA stack, floating point stack.
  enum StackID { DataStack, ReturnStack, AuxStack, FloatStack, MaxStackID = FloatStack };
  //! \brief Forth interpreter mode
  enum ForthState { Interprete, Compile, Comment, MaxForthStates = Comment };
}


// **************************************************************
// Stream
// **************************************************************
#define STREAM m_streams_stack[m_opened_streams] // current stream parsed
#define MAX_OPENED_STREAMS   (16U)    // depth of INCLUDE calls

// Dictionnary display
#  define WORD_GROUPING 6

//#  define DEBUG(txt) do { std::cout << txt << std::endl; } while (0)
//#  define DEBUG3(a, txt, b) do { std::cout << a << txt << b << std::endl; } while (0)

// **************************************************************
//
// **************************************************************
#define SWAP32(x) (((x) & 0xff) << 24 | ((x) & 0xff00) << 8 | ((x) & 0xff0000) >> 8 | ((x) >> 24) & 0xff)
#define SWAP16(x) (((x) & 0xff) << 8 | (((x) & 0xff00) >> 8))

class SimString : public std::string
{
public:
  SimString()
    : std::string()
    { printf("STRING::STRING empty ctr\n"); }

  SimString(const std::string& str)
    : std::string(str)
    { printf("STRING::STRING copy '%s'\n", str.c_str()); }

  SimString(const std::string& str, size_t pos, size_t len = npos)
    : std::string(str, pos, len)
    { printf("STRING::STRING 3\n"); }

  SimString(const char* s)
    : std::string(s)
    { printf("STRING::STRING copy char '%s'\n", s); }

  SimString(const char* s, size_t n)
    : std::string(s, n)
    { printf("STRING::STRING 5\n"); }

  SimString(size_t n, char c)
    : std::string(n, c)
    { printf("STRING::STRING 6\n"); }

  template <class InputIterator>
  SimString(InputIterator first, InputIterator last)
    : std::string(first, last)
  { printf("STRING::STRING 7\n"); }
};

#endif /* FORTH_HELPER_HPP_ */