#!/usr/bin/env perl

use Env;
use File::Copy;
Env::import();

$PluginName = "__nada__";
$ReactionName = "__nada__";
$ReactionFactoryTag = "__nada__";

if($#ARGV < 0)
{
	&Usage();
	exit;
}
elsif($#ARGV == 0)
{
	$PluginName = $ARGV[0];
	$ReactionName = $PluginName;
	$ReactionFactoryTag = $ReactionName;
}
elsif($#ARGV == 1)
{
	$PluginName = $ARGV[0];
	$ReactionName = $ARGV[1];
	$ReactionFactoryTag = $ReactionName;
}
else
{
	$PluginName = $ARGV[0];
	$ReactionName = $ARGV[1];
	$ReactionFactoryTag = $ARGV[2];
}

print "\n";
print "Generating files for plugin \"$PluginName\", with DReaction \"$ReactionName\" defined in DReaction_factory_${ReactionFactoryTag} \n";

# Create directory to hold source
mkdir($PluginName);

# Create C++ Header File for Factory Class
$dhfile = $fname = "DReaction_factory_${ReactionFactoryTag}.h";
open(FILE, ">$PluginName/$dhfile");
&PrintFileHeader();
&PrintFactoryClass();
close(FILE);
print " - $dhfile\n";

# Create C++ Implementation file for Factory Class
$ccfile = $fname = "DReaction_factory_${ReactionFactoryTag}.cc";
open(FILE, ">$PluginName/$ccfile");
&PrintFileHeader();
&PrintFactoryMethods();
close(FILE);
print " - $ccfile\n";

# Create Makefile
open(FILE, ">$PluginName/Makefile");
&PrintMakefile();
close(FILE);
print " - Makefile\n";

# Create SConscript
# Not really needed unless included in GlueX Build
#open(FILE, ">$PluginName/SConscript");
#&PrintSConscript();
#close(FILE);
#print " - SConscript\n";
#
# Create SConstruct
open(FILE, ">$PluginName/SConstruct");
&PrintSConstruct();
close(FILE);
print " - SConstruct\n";

# Copy SConstruct from build
#copy("${HALLD_HOME}/src/SBMS/SConstruct.plugin","${PluginName}/SConstruct") or die "Unable to copy SConstruct check HALLD_HOME is set correctly\r";
#print " - SConstruct\n";


# Create C++ Header File for Factory Generator Class
$dhfile = $fname = "DFactoryGenerator_${PluginName}.h";
open(FILE, ">$PluginName/$dhfile");
&PrintFileHeader();
&PrintFactoryGeneratorClass();
close(FILE);
print " - $dhfile\n";

# Create C++ Header File for Event Processor Class
$dhfile = $fname = "DEventProcessor_${PluginName}.h";
open(FILE, ">$PluginName/$dhfile");
&PrintFileHeader();
&PrintEventProcessorClass();
close(FILE);
print " - $dhfile\n";

# Create C++ Implementation file for Event Processor Class
$ccfile = $fname = "DEventProcessor_${PluginName}.cc";
open(FILE, ">$PluginName/$ccfile");
&PrintFileHeader();
&PrintEventProcessorMethods();
close(FILE);
print " - $ccfile\n";

###############
# PrintFileHeader
###############
sub PrintFileHeader()
{
	# print a few lines at the very top of the file
	$uname = `uname -nprs`;
	chomp($uname);
	print FILE "// \$Id\$\n";
	print FILE "//\n";
	print FILE "//    File: $fname\n";
	print FILE "// Created: ".`date`;
	print FILE "// Creator: ".$ENV{"USER"}." (on $uname)\n";
	print FILE "//\n";
}

###############
# PrintFactoryClass
###############
sub PrintFactoryClass()
{
	$content = "
\#ifndef _DReaction_factory_${ReactionFactoryTag}_
\#define _DReaction_factory_${ReactionFactoryTag}_

\#include <iostream>
\#include <iomanip>

\#include <JANA/JFactory.h>
\#include <ANALYSIS/DReaction.h>
\#include <ANALYSIS/DHistogramActions.h>
\#include <ANALYSIS/DCutActions.h>

using namespace std;
using namespace jana;

class DReaction_factory_${ReactionFactoryTag} : public jana::JFactory<DReaction>
{
	public:
		DReaction_factory_${ReactionFactoryTag}()
		{
			// This is so that the created DReaction objects persist throughout the life of the program instead of being cleared each event. 
			SetFactoryFlag(PERSISTANT);
		}
		const char* Tag(void){return \"$ReactionFactoryTag\";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		deque<DReactionStep*> dReactionStepPool; //to prevent memory leaks
};

\#endif // _DReaction_factory_${ReactionFactoryTag}_

";
	print FILE $content;
}

###############
# PrintFactoryMethods
###############
sub PrintFactoryMethods()
{
	$content = "

\#include \"DReaction_factory_${ReactionFactoryTag}.h\"

//------------------
// init
//------------------
jerror_t DReaction_factory_${ReactionFactoryTag}::init(void)
{
	// Make as many DReaction objects as desired
	DReactionStep* locReactionStep = NULL;
	DReaction* locReaction = new DReaction(\"${ReactionName}\"); //needs to be a unique name for each DReaction object, CANNOT (!) be \"Thrown\"

	// DOCUMENTATION:
	// ANALYSIS library: https://halldweb1.jlab.org/wiki/index.php/GlueX_Analysis_Software
	// DReaction factory: https://halldweb1.jlab.org/wiki/index.php/Analysis_DReaction

	/**************************************************** ${ReactionName} Reaction Steps ****************************************************/

	/* 
	//Required: DReactionSteps to specify the channel and decay chain you want to study
		//Particles are of type Particle_t, an enum defined in sim-recon/src/libraries/include/particleType.h

	//Example: g, p -> pi+, pi-, pi0, (p)
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Gamma);
	locReactionStep->Set_TargetParticleID(Proton);
	locReactionStep->Add_FinalParticleID(PiPlus);
	locReactionStep->Add_FinalParticleID(PiMinus);
	locReactionStep->Add_FinalParticleID(Pi0);
	locReactionStep->Add_FinalParticleID(Proton, true); //true: proton missing
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	//Example: pi0 -> g, g
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Pi0);
	locReactionStep->Add_FinalParticleID(Gamma);
	locReactionStep->Add_FinalParticleID(Gamma);

	//optional: in a p4 kinematic fit, this will disable the constraint on the mass of the pi0
		//default is enabled, but you don't need to worry about setting false for the beam particle step
		//note that the omega and phi will always be false, regardless of what you set below
	//locReactionStep->Set_ApplyKinFitMassConstraintOnInitialParticleFlag(false);

	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak
	*/

	/**************************************************** ${ReactionName} Control Settings ****************************************************/

	// Recommended: Type of kinematic fit to perform (default is d_NoFit)
		//fit types are of type DKinFitType, an enum defined in sim-recon/src/libraries/ANALYSIS/DKinFitResults.h
	// locReaction->Set_KinFitType(d_P4AndVertexFit); //simultaneously constrain apply four-momentum conservation, invariant masses, and common-vertex constraints

	// Highly Recommended: When generating particle combinations, reject all photon candidates with a PID confidence level < 5.73303E-7 (+/- 5-sigma)
	locReaction->Set_MinPhotonPIDFOM(5.73303E-7);

	// Highly Recommended: When generating particle combinations, reject all charged track candidates with a PID confidence level < 5.73303E-7 (+/- 5-sigma)
	locReaction->Set_MinChargedPIDFOM(5.73303E-7);

	// Highly Recommended: When generating particle combinations, reject all beam photons that match to a different RF bunch (delta_t > 1.002 ns)
	locReaction->Set_MaxPhotonRFDeltaT(0.5*4.008); //beam bunches are every 4.008 ns (2.004 should be minimum cut value)

	// Optional, use with caution: When generating particle combinations, but after the Photon/RF Delta-t cut, reject all combos with more than this # of beam photons
		//useful for missing-particle studies when you need a very pure sample of events
		//however, this can cut away a lot of signal events too
	// locReaction->Set_MaxNumBeamPhotonsInBunch(1);

	// Highly Recommended: Cut on number of extra \"good\" tracks. \"Good\" tracks are ones that survive the \"PreSelect\" (or user custom) factory.
		// Current (09/26/2014): \"Good\" tracks have a detector-hit match, and tracking FOM > 0.0027 (+/- 3 sigma). 
		// Important: Keep cut large: Can have many ghost and accidental tracks that look \"good\"
	locReaction->Set_MaxExtraGoodTracks(4);

	// Highly Recommended: Enable ROOT TTree output for this DReaction
	// locReaction->Enable_TTreeOutput(\"tree_${ReactionName}.root\"); //string is file name (must end in \".root\"!!): doen't need to be unique, feel free to change

	/************************************************** ${ReactionName} Pre-Combo Custom Cuts *************************************************/

	// Highly Recommended: Very loose invariant mass cuts, applied during DParticleComboBlueprint construction
	// Example: pi0 -> g, g cut
	// locReaction->Set_InvariantMassCut(Pi0, 0.08, 0.19);

	// Highly Recommended: Very loose DAnalysisAction cuts, applied just after creating the combination (before saving it)
	// Example: Missing mass squared of proton
	// locReaction->Add_ComboPreSelectionAction(new DCutAction_MissingMassSquared(locReaction, false, -0.1, 2.56));

	/**************************************************** ${ReactionName} Analysis Actions ****************************************************/

	/*
	// Recommended: Analysis actions automatically performed by the DAnalysisResults factories to histogram useful quantities.
		//These actions are executed sequentially, and are executed on each surviving (non-cut) particle combination 
		//Pre-defined actions can be found in ANALYSIS/DHistogramActions.h and ANALYSIS/DCutActions.h

	// PID & Kinematics
	locReaction->Add_AnalysisAction(new DHistogramAction_PID(locReaction));
	locReaction->Add_AnalysisAction(new DHistogramAction_TruePID(locReaction)); //momentum distributions of tracks with true/false PID (if thrown data available)

	// Transverse Momentum //Recommended for no-missing-particle reactions only!
	// locReaction->Add_AnalysisAction(new DHistogramAction_MissingTransverseMomentum(locReaction, false, 1000, 0.0, 1.0)); //false: fill histograms with measured particle data
	// locReaction->Add_AnalysisAction(new DCutAction_TransverseMomentum(locReaction, 0.4)); //Max Missing Pt of 0.4 GeV

	// Kinematic Fit Results
	// locReaction->Add_AnalysisAction(new DHistogramAction_KinFitResults(locReaction, 0.05)); //5% confidence level cut on pull histograms only
	// locReaction->Add_AnalysisAction(new DCutAction_KinFitFOM(locReaction, 0.0)); //0% confidence level cut //require kinematic fit converges

	// Kinematics
	locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, false)); //false: fill histograms with measured particle data
	// locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, true, \"KinFit\")); //true: fill histograms with kinematic-fit particle data //\"KinFit\": unique name since action type is repeated
	locReaction->Add_AnalysisAction(new DHistogramAction_TrackVertexComparison(locReaction));
	*/

	_data.push_back(locReaction); //Register the DReaction with the factory

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DReaction_factory_${ReactionFactoryTag}::fini(void)
{
	for(size_t loc_i = 0; loc_i < dReactionStepPool.size(); ++loc_i)
		delete dReactionStepPool[loc_i]; //cleanup memory
	return NOERROR;
}

";
	print FILE $content;
}

###############
# PrintFactoryGeneratorClass
###############
sub PrintFactoryGeneratorClass()
{
	$content = "
\#ifndef _DFactoryGenerator_${PluginName}_
\#define _DFactoryGenerator_${PluginName}_

\#include <JANA/jerror.h>
\#include <JANA/JFactoryGenerator.h>

\#include \"DReaction_factory_${ReactionFactoryTag}.h\"

class DFactoryGenerator_${PluginName} : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return \"DFactoryGenerator_${PluginName}\";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_${ReactionFactoryTag}());
			return NOERROR;
		}
};

\#endif // _DFactoryGenerator_${PluginName}_

";
	print FILE $content;
}

###############
# PrintEventProcessorClass
###############
sub PrintEventProcessorClass()
{
	$content = "
\#ifndef _DEventProcessor_${PluginName}_
\#define _DEventProcessor_${PluginName}_

\#include <JANA/JEventProcessor.h>
\#include <JANA/JApplication.h>

\#include <ANALYSIS/DEventWriterROOT.h>
\#include <HDDM/DEventWriterREST.h>
\#include <ANALYSIS/DHistogramActions.h>

\#include \"DFactoryGenerator_${PluginName}.h\"

using namespace jana;
using namespace std;

class DEventProcessor_${PluginName} : public jana::JEventProcessor
{
	public:
		const char* className(void){return \"DEventProcessor_${PluginName}\";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop* locEventLoop, int32_t locRunNumber);	///< Called every time a new run number is detected.
		jerror_t evnt(jana::JEventLoop* locEventLoop, uint64_t locEventNumber);	///< Called every event.
		jerror_t erun(void);						///< Called every time run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		//For non-custom reaction-independent histograms, it is recommended that you simultaneously run the monitoring_hists plugin instead of defining them here
};

\#endif // _DEventProcessor_${PluginName}_

";
	print FILE $content;
}

###############
# PrintEventProcessorMethods
###############
sub PrintEventProcessorMethods
{
	$content = "
\#include \"DEventProcessor_${PluginName}.h\"

// Routine used to create our DEventProcessor

extern \"C\"
{
	void InitPlugin(JApplication *locApplication)
	{
		InitJANAPlugin(locApplication);
		locApplication->AddProcessor(new DEventProcessor_${PluginName}()); //register this plugin
		locApplication->AddFactoryGenerator(new DFactoryGenerator_${PluginName}()); //register the factory generator
	}
} // \"C\"

//------------------
// init
//------------------
jerror_t DEventProcessor_${PluginName}::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... create historgrams or trees ...
	// japp->RootUnLock();
	//

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t DEventProcessor_${PluginName}::brun(jana::JEventLoop* locEventLoop, int32_t locRunNumber)
{
	// This is called whenever the run number changes

	/*
	//Recommended: Create output ROOT TTrees (nothing is done if already created)
	const DEventWriterROOT* locEventWriterROOT = NULL;
	locEventLoop->GetSingle(locEventWriterROOT);
	locEventWriterROOT->Create_DataTrees(locEventLoop);
	*/

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DEventProcessor_${PluginName}::evnt(jana::JEventLoop* locEventLoop, uint64_t locEventNumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// locEventLoop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	//
	// Here\'s an example:
	//
	// vector<const MyDataClass*> mydataclasses;
	// locEventLoop->Get(mydataclasses);
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();

	// DOCUMENTATION:
	// ANALYSIS library: https://halldweb1.jlab.org/wiki/index.php/GlueX_Analysis_Software

	/*********************************************************** REQUIRED ***********************************************************/

	//REQUIRED: To run an analysis, You MUST call one at least of the below code fragments. 
		//JANA is on-demand, so if you don't call one of these, then your analysis won't run. 

	/*
	//Recommended: Write surviving particle combinations (if any) to output ROOT TTree
		//If no cuts are performed by the analysis actions added to a DReaction, then this saves all of its particle combinations. 
		//The event writer gets the DAnalysisResults objects from JANA, performing the analysis. 
	// string is DReaction factory tag: will fill trees for all DReactions that are defined in the specified factory
	const DEventWriterROOT* locEventWriterROOT = NULL;
	locEventLoop->GetSingle(locEventWriterROOT);
	locEventWriterROOT->Fill_DataTrees(locEventLoop, \"${ReactionFactoryTag}\");
	*/

	/*
	//Optional: Get the analysis results for all DReactions. 
		//Getting these objects triggers the analysis, if it wasn't performed already. 
		//These objects contain the DParticleCombo objects that survived the DAnalysisAction cuts that were added to the DReactions
	vector<const DAnalysisResults*> locAnalysisResultsVector;
	locEventLoop->Get(locAnalysisResultsVector);
	*/

	/*********************************************************** OPTIONAL ***********************************************************/

	/*
	//Optional: Get all particle combinations for all DReactions. 
		//If kinematic fits were requested, these contain both the measured and kinematic-fit track parameters
		//No cuts from DAnalysisActions are placed on these combos
	vector<const DParticleCombo*> locParticleCombos;
	locEventLoop->Get(locParticleCombos);
	for(size_t loc_i = 0; loc_i < locParticleCombos.size(); ++loc_i)
	{
		const DParticleCombo* locParticleCombo = locParticleCombos[loc_i];
		if(locParticleCombo->Get_Reaction()->Get_ReactionName() != \"${ReactionName}\")
			continue; // particle combination was for a different reaction
		//perform further analysis steps here...
	}
	*/

	/*
	//Optional: Perform further cuts on the particle combos in the analysis results. 
	for(size_t loc_i = 0; loc_i < locAnalysisResultsVector.size(); ++loc_i)
	{
		const DAnalysisResults* locAnalysisResults = locAnalysisResultsVector[loc_i];
		if(locAnalysisResults->Get_Reaction()->Get_ReactionName() != \"${ReactionName}\")
			continue; // analysis results were for a different reaction
		//get the DParticleCombo objects for this DReaction that survived all of the DAnalysisAction cuts
		deque<const DParticleCombo*> locPassedParticleCombos;
		locAnalysisResults->Get_PassedParticleCombos(locPassedParticleCombos);
		for(size_t loc_j = 0; loc_j < locPassedParticleCombos.size(); ++loc_j)
		{
			const DParticleCombo* locPassedParticleCombo = locPassedParticleCombos[loc_j];
			//perform further analysis steps here...
		}
	}
	*/

	/*
	//Optional: Save event to output REST file. Use this to create skims.
	const DEventWriterREST* locEventWriterREST = NULL;
	locEventLoop->GetSingle(locEventWriterREST);
	locEventLoop->Write_RESTEvent(locEventLoop, \"${PluginName}\"); //string is part of output file name
	*/

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t DEventProcessor_${PluginName}::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DEventProcessor_${PluginName}::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

";
	print FILE $content;
}

$PluginName = "__nada__";
$ReactionName = "__nada__";
$ReactionFactoryTag = "__nada__";

###############
# PrintMakefile
###############
sub PrintMakefile()
{
	# Contents of default Makefile for plugins
	print FILE "\n";
	print FILE "PACKAGES = ROOT:JANA\n";
	print FILE "\n";
	print FILE "include \$(HALLD_HOME)/src/BMS/Makefile.shlib\n";
	print FILE "\n";
}

###############
# PrintSConscript
###############
sub PrintSConscript()
{
	# Contents of default SConscript file for plugins
	print FILE "\n";
	print FILE "import sbms\n";
	print FILE "Import('*')\n";
	print FILE "env = env.Clone()\n";
	print FILE "sbms.AddDANA(env)\n";
	print FILE "sbms.AddROOT(env)\n";
	print FILE "sbms.plugin(env)\n";
	print FILE "\n";
}

###############
# Print SConstruct
###############

sub PrintSConstruct()
{
	# Contents of default SConstruct file for plugins
        $content = "

import os
import sys
import subprocess
import glob

# Get HALLD_HOME environment variable, verifying it is set
halld_home = os.getenv('HALLD_HOME')
if(halld_home == None):
        print 'HALLD_HOME environment variable not set!'
        exit(-1)

# Get HALLD_MY if it exists. Otherwise use HALLD_HOME
halld_my = os.getenv('HALLD_MY', halld_home)

# Get plugin name
pluginname = os.path.split(os.getcwd())[1]

# Add SBMS directory to PYTHONPATH
sbmsdir = \"%s/src/SBMS\" % halld_home
sys.path.append(sbmsdir)

import sbms

# Get command-line options
SHOWBUILD = ARGUMENTS.get('SHOWBUILD', 0)

# Get platform-specific name
osname = os.getenv('BMS_OSNAME', 'build')

# Get architecture name
arch = subprocess.Popen([\"uname\"], stdout=subprocess.PIPE).communicate()[0].strip()

# Setup initial environment
plugininstalldir = halld_my
gluexinstalldir = \"%s/%s\" %(halld_home, osname)
include = \"%s/include\" % (gluexinstalldir)
bin = \"%s/bin\" % (plugininstalldir)
lib = \"%s/lib\" % (plugininstalldir)
plugins = \"%s/%s/plugins\" % (plugininstalldir,osname)
env = Environment(    CPPPATH = [include],
                      LIBPATH = [\"%s/%s/lib\" %(halld_home, osname)],  # n.b. add HALLD_HOME here and prepend HALLD_MY below
                  variant_dir = \".%s\" % (osname))

# Only add HALLD_MY library search path if it already exists
# since we'll get a warning otherwise
if (os.path.exists(lib)): env.PrependUnique(lib)

# These are SBMS-specific variables (i.e. not default scons ones)
env.Replace(INSTALLDIR    = plugininstalldir,
                                OSNAME        = osname,
                                INCDIR        = include,
                                BINDIR        = bin,
                                LIBDIR        = lib,
                                PLUGINSDIR    = plugins,
                                ALL_SOURCES   = [],        # used so we can add generated sources
                                SHOWBUILD     = SHOWBUILD,
                 COMMAND_LINE_TARGETS = COMMAND_LINE_TARGETS)
# Use terse output unless otherwise specified
if SHOWBUILD==0:
        env.Replace(   CCCOMSTR       = \"Compiling  [\$SOURCE]\",
                                  CXXCOMSTR       = \"Compiling  [\$SOURCE]\",
                                  FORTRANPPCOMSTR = \"Compiling  [\$SOURCE]\",
                                  FORTRANCOMSTR   = \"Compiling  [\$SOURCE]\",
                                  SHCCCOMSTR      = \"Compiling  [\$SOURCE]\",
                                  SHCXXCOMSTR     = \"Compiling  [\$SOURCE]\",
                                  LINKCOMSTR      = \"Linking    [\$TARGET]\",
                                  SHLINKCOMSTR    = \"Linking    [\$TARGET]\",
                                  INSTALLSTR      = \"Installing [\$TARGET]\",
                                  ARCOMSTR        = \"Archiving  [\$TARGET]\",
                                  RANLIBCOMSTR    = \"Ranlib     [\$TARGET]\")
# Get compiler from environment variables (if set)
env.Replace( CXX = os.getenv('CXX', 'g++'),
             CC  = os.getenv('CC' , 'gcc'),
             FC  = os.getenv('FC' , 'gfortran') )

# Add local directory, directories from HALLD_MY and HALLD_HOME to include search path
#env.PrependUnique(CPPPATH = ['#'])
env.PrependUnique(CPPPATH = ['%s/src' % halld_my, '%s/src/libraries' % halld_my, '%s/src/libraries/include' % halld_my])
env.PrependUnique(CPPPATH = ['%s/src' % halld_home, '%s/src/libraries' % halld_home, '%s/src/libraries/include' % halld_home])

# Turn on debug symbols and warnings
env.PrependUnique(      CFLAGS = ['-g', '-fPIC', '-Wall'])
env.PrependUnique(    CXXFLAGS = ['-g', '-fPIC', '-Wall'])
env.PrependUnique(FORTRANFLAGS = ['-g', '-fPIC', '-Wall'])

# Apply any platform/architecture specific settings
sbms.ApplyPlatformSpecificSettings(env, arch)
sbms.ApplyPlatformSpecificSettings(env, osname)


# Make plugin from source in this directory
sbms.AddDANA(env)
sbms.AddROOT(env)
sbms.plugin(env,\".%s/%s\" % (osname,pluginname))

# Make install target
env.Alias('install', env.Install(plugins, \".%s/%s.so\" % (osname,pluginname)))

";
        print FILE $content;
}

###############
# Usage
###############
sub Usage()
{
	print "\n";
	print "Usage:\n\n    MakeReactionPlugin plugin_name [reaction_name [reaction_factory_tag]] \n";
	print "\n    NOTE: The last two arguments are optional; they will be set to \"plugin_name\" and \n";
	print "    \"reaction_name\" (respectively) if not specified.\n";
	print "\n";
	print "Generate the C++ source and header files to implement a new analysis\n";
	print "plugin for use with the JANA framework. The plugin will implement\n";
	print "a DReaction factory (complete with factory generator).\n";
	print "\n";
	print "This will create a directory and generate a few files in it, including\n";
	print "a default Makefile and SConstruct so that one can use to immediately compile the new plugin.\n";
	print "\n";

	print "\n";
	print "The C++ files generated will define classes based on JFactory,\n";
	print "JFactoryGenerator, and JEventProcessor. The \"plugin_name\" parameter is also\n";
	print "the name of the directory created (which can be changed).\n";
	print "\n";	
	print "\n";
}

