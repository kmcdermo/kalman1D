#include "../interface/Event.hh"
#include "../interface/ROOTValidation.hh"
#include "../interface/Config.hh"

#include <string>
#include <sstream>
#include <list>
#include <cstdlib>

typedef std::list<std::string> lStr_t;
typedef lStr_t::iterator       lStr_i;

void next_arg_or_die(lStr_t & args, lStr_i & i, bool allow_single_minus = false)
{
  lStr_i j = i;
  if (++j == args.end() ||
      ((*j)[0] == '-' && ! (*j == "-" && allow_single_minus))) 
  {
    std::cerr << "Error: option " << *i << " requires an argument." << std::endl;
    exit(1);
  }
  i = j;
}

void parse_options(const std::string & argv0, lStr_t & mArgs)
{
  lStr_i i  = mArgs.begin();
  while (i != mArgs.end())
  {
    lStr_i start = i;
    if (*i == "-h" || *i == "-help" || *i == "--help")
    {
      printf(
	     "Usage: %s [options]\n"
	     "Options:\n"
	     "  --debug               run code in debug mode (default: %s)\n"
	     "  --mc-init             use MC truth info as first input into Kalman filter (default: %s)\n"
	     "  --no-smoother         do not use Kalman smoothing after Kalman filtering  (default: %s)\n"
	     "  --nEvents      <int>  number of events to simulate (default: %i)\n"
	     "  --nTracks      <int>  number of tracks per event (default: %i)\n"
	     "  --nHits        <int>  number of hist per track (default: %i)\n"
	     "  --startpos     <flt>  max starting position of tracks [cm] (default: %5.2f)\n"
	     "  --startvel     <flt>  max starting velocity of tracks [cm/s] (default: %5.2f)\n"
	     "  --deltaT       <flt>  time between measurements [s] (default: %5.2f)\n"
	     "  --diff-ticks   <int>  when using line estimator, length of lever arm in measurements (default: %i)\n"
	     "  --pos-unc      <flt>  process noise uncertainty on position propagation [cm] (default: %5.2f)\n"
	     "  --vel-unc      <flt>  process noise uncertainty on velocity propagation [cm] (default: %5.2f)\n" 
	     "  --meas-unc     <flt>  uncertainty on position measurement [cm] (default: %5.2f)\n"
	     "  --pos-unc-SF   <flt>  fudge factor on position process noise [cm] (default: %5.2f)\n"
	     "  --vel-unc-SF   <flt>  fudge factor on velocity process noise [cm] (default: %5.2f)\n"
	     "  --meas-unc-SF  <flt>  fudge factor on measurement uncertainty [cm] (default: %5.2f)\n"
	     ,
	     argv0.c_str(),
	     (Config::debug       ? "true" : "false"),
	     (Config::useLineEst  ? "false" : "true"),
	     (Config::useSmoother ? "false" : "true"),
	     Config::nEvents,
	     Config::nTracks,
	     Config::nHits,
	     Config::startpos,
	     Config::startvel,
	     Config::deltaT,
	     Config::diff_ticks,
	     Config::processNoisePos,
	     Config::processNoiseVel,
	     Config::measNoisePos,
	     Config::processNoisePosSF,
	     Config::processNoiseVelSF,
	     Config::measNoisePosSF
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
    else if (*i == "--nEvents")
    {
      next_arg_or_die(mArgs, i);
      Config::nEvents = std::atoi(i->c_str());
    }
    else if (*i == "--nTracks")
    {
      next_arg_or_die(mArgs, i);
      Config::nTracks = std::atoi(i->c_str());
    }
    else if (*i == "--nHits")
    {
      next_arg_or_die(mArgs, i);
      Config::nHits = std::atoi(i->c_str());
    }
    else if (*i == "--startpos")
    {
      next_arg_or_die(mArgs, i);
      Config::startpos = std::atof(i->c_str());
    }
    else if (*i == "--startvel")
    {
      next_arg_or_die(mArgs, i);
      Config::startvel = std::atof(i->c_str());
    }
    else if (*i == "--deltaT")
    {
      next_arg_or_die(mArgs, i);
      Config::deltaT = std::atof(i->c_str());
    }
    else if (*i == "--diff-ticks")
    {
      next_arg_or_die(mArgs, i);
      Config::diff_ticks = std::atoi(i->c_str());
    }
    else if (*i == "--pos-unc")
    {
      next_arg_or_die(mArgs, i);
      Config::processNoisePos = std::atof(i->c_str());
    }
    else if (*i == "--vel-unc")
    {
      next_arg_or_die(mArgs, i);
      Config::processNoiseVel = std::atof(i->c_str());
    }
    else if (*i == "--meas-unc")
    {
      next_arg_or_die(mArgs, i);
      Config::measNoisePos = std::atof(i->c_str());
    }
    else if (*i == "--pos-unc-SF")
    {
      next_arg_or_die(mArgs, i);
      Config::processNoisePosSF = std::atof(i->c_str());
    }
    else if (*i == "--vel-unc-SF")
    {
      next_arg_or_die(mArgs, i);
      Config::processNoiseVelSF = std::atof(i->c_str());
    }
    else if (*i == "--meas-unc-SF")
    {
      next_arg_or_die(mArgs, i);
      Config::measNoisePosSF = std::atof(i->c_str());
    }
    else
    {
      fprintf(stderr, "Error: Unknown option/argument '%s'.\n", i->c_str());
      exit(1);
    }
    mArgs.erase(start, ++i);
  }
}

int main(int argc, const char* argv[])
{
  // Read in command line into a list
  lStr_t mArgs; 
  for (int i = 1; i < argc; i++)
  {
    mArgs.push_back(argv[i]);
  }

  // Parse options
  parse_options(argv[0],mArgs);

  // Once all command line options are read, assign values to dependent variables
  Config::AssignDependentConfig();

  // make one instance of the validation for all events to use
  ROOTValidation val("valtree.root");

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
