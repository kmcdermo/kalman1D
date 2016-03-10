#include <sstream>
#include <list>
#include "Matrix.h"
#include "Event.h"
#include "ROOTValidation.h"

typedef std::list<std::string> lStr_t;
typedef lStr_t::iterator       lStr_i;

void next_arg_or_die(lStr_t& args, lStr_i& i, bool allow_single_minus=false)
{
  lStr_i j = i;
  if (++j == args.end() ||
      ((*j)[0] == '-' && ! (*j == "-" && allow_single_minus)))
  {
    std::cerr <<"Error: option "<< *i <<" requires an argument.\n";
    exit(1);
  }
  i = j;
}

int main(int argc, const char* argv[])
{
  int none = 0;

  lStr_t mArgs; 
  for (int i = 1; i < argc; ++i)
  {
    mArgs.push_back(argv[i]);
  }

  lStr_i i  = mArgs.begin();
  while (i != mArgs.end())
  {
    lStr_i start = i;
    if (*i == "-h" || *i == "-help" || *i == "--help")
    {
      printf(
        "Usage: %s [options]\n"
        "Options:\n"
	"  --none       <num>    number of none    (def: %d)\n"
        ,
        argv[0],
	none
      );
      exit(0);
    }
    else if (*i == "--none")
    {
      next_arg_or_die(mArgs, i);
      none = atoi(i->c_str());
    }
    else
    {
      fprintf(stderr, "Error: Unknown option/argument '%s'.\n", i->c_str());
      exit(1);
    }
    mArgs.erase(start, ++i);
  }

  // make one instance of the validation for all events to use
  Validation val("validation.root");

  for (int i = 0; i < Config::nEvents; i++){
    Event ev(i,val);
    std::cout << "Event #" << ev.evtID() << std::endl;
    ev.Simulate();
    ev.Fit();
    ev.Validate();
  }

  val.fillConfigTree();
  val.saveValidation();
  return 0;
}
