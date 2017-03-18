#include "SimForth.hpp"
#include "Config.hpp"
#include "MapEditor.hpp"
#include <string>

void SimForth::boot()
{
  LOGI("Booting SimForth");

  // Start the Forth core
  Forth::boot();

  // Add specialized words for SimTaDyn
  m_dictionary.add(SIMFORTH_PRIMITIVE_TOTO, FORTH_DICO_ENTRY("TOTO"), 0);

  // Initialize basic Forth system
  std::pair<bool, std::string> res;

  res = Forth::interpreteFile(Config::instance().data_path("forth/system.fs"));
  ok(res);
}

bool SimForth::isACell(std::string const& word, Cell32& number)
{
  // Map exists ?
  SimTaDynMap *map = MapEditor::instance().map();
  if (nullptr == map)
    return false;

  // Forth word starting by A# or N# or Z#
  if (('A' != word[0]) && ('N' != word[0]) && ('Z' != word[0]))
    return false;

  if ('#' != word[1])
    return false;

  // Get the unique ID
  Key id;
  try
    {
      std::size_t sz;
      id = std::stoul(word.substr(2, word.length() - 2), &sz);
      if (sz + 2 != word.length())
        return false;
    }
  catch (const std::invalid_argument& ia)
    {
      return false;
    }
  catch (const std::out_of_range& oor)
    {
      return false;
    }

  // If exists on the map, get the cost value
  // FIXME: let try to get an ancestor
  // FIXME: temporaire car on ne va pas que gerer la fonction cout
  SimTaDynNode *node;

  switch (word[0])
    {
      // Node
    case 'N':
      map->removeNode(id);
      node = map->getNode(id);
      if (nullptr == node)
        return false;
      number = node->m_cost;
      break;

      // Not a SimTaDyn cell
    default:
      return false;
    }

  return true;
}

void SimForth::interpreteWordCaseInterprete(std::string const& word)
{
  Cell32 number;

  if (isACell(word, number))
    {
      DPUSH(number);
      isStackUnderOverFlow(forth::DataStack);
    }
  else
    {
      Forth::interpreteWordCaseInterprete(word);
    }
}

void SimForth::interpreteWordCaseCompile(std::string const& word)
{
  Cell32 number;

  if (isACell(word, number))
    {
      // FIXME: temporaire car on ne va pas que gerer la fonction cout
      if (number <= 65535U)
        {
          m_dictionary.appendCell16(FORTH_PRIMITIVE_LITERAL_16);
          m_dictionary.appendCell16(number);
        }
      else
        {
          m_dictionary.appendCell16(FORTH_PRIMITIVE_LITERAL_32);
          m_dictionary.appendCell32(number);
        }
    }
  else
    {
      Forth::interpreteWordCaseInterprete(word);
    }
}

std::pair<bool, std::string> SimForth::interpreteCell(std::string const& code_forth,
                                                      std::string const& name,
                                                      Cell32& tos)
{
  m_err_stream = 0;
  STREAM.loadString(code_forth, name);

  // FIXME: should be called outside each cell: optimisation
  // Disable compilation mode
  //dictionary().smudge(":");
  //dictionary().smudge("INCLUDE");

LOGD("Avant Depth: %d", stackDepth(forth::DataStack));

  // Run the Forth interpreter
  int32_t depth_before = stackDepth(forth::DataStack);
  auto res = parseStream();

  // Enable compilation mode
  //dictionary().smudge("INCLUDE");
  //dictionary().smudge(":");

  // Check the data stack depth does not change or is 1 after the
  // end of the execution of the script.
  if (res.first)
    {
LOGD("Apres Depth: %d", stackDepth(forth::DataStack));

      int32_t depth_after = stackDepth(forth::DataStack);
      int32_t depth = depth_after;// - depth_before;
      if (1 == depth)
        {
          DPOP(tos);
        }
      else
        {
          std::string msg = "Stack depth changed during the interpretation of the cell '" + name;
          if (depth > 0)
            {
              msg += "' probably you forget to store the result in the cell field";
            }
          else
            {
              msg += "' probably you poped the data stack too much";
            }

          res = std::make_pair(false, msg);
        }
    }

  // FIXME ne pas laisser faire un std::cout
  //ok(res);
  return res;//.first;
}
