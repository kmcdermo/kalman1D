#include "../interface/Event.hh"
#include "../interface/ROOTValidation.hh"
#include "../interface/Config.hh"

#include <sstream>
#include <list>

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
	     "  --debug           run code in debug mode (default: %s)\n"
	     "  --mc-init         use MC truth info as first input into Kalman filter (default: %s)\n"
	     "  --no-smoother     do not use Kalman smoothing after Kalman filtering  (default: %s)\n"
	     ,
	     argv[0],
	     (Config::debug       ? "true" : "false"),
	     (Config::useLineEst  ? "false" : "true"),
	     (Config::useSmoother ? "false" : "true")
	     );
      exit(0);
    }
    else if (*i == "--debug")
    {
      Config::debug = true;
    }
    else if (*i == "--mc-init")
    {
      Config::useLineEst = false;
    }
    else if (*i == "--no-smoother")
    {
      Config::useSmoother = false;
    }
    else
    {
      fprintf(stderr, "Error: Unknown option/argument '%s'.\n", i->c_str());
      exit(1);
    }
    mArgs.erase(start, ++i);
  }

  // make one instance of the validation for all events to use
  Validation val("valtree.root");

  // set values of special matrices once (same for all events) -- SMatrix really finicky
  Config::defineSpecialMatrices();

  for (int i = 0; i < Config::nEvents; i++)
  {
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
