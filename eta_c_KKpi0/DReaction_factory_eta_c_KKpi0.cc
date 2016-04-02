// $Id$
//
//    File: DReaction_factory_eta_c_KKpi0.cc
// Created: Wed Feb  3 11:04:34 EST 2016
// Creator: mpatsyuk (on Linux hidsk0001.cmsaf.mit.edu 2.6.32-358.el6.x86_64 x86_64)
//


#include "DReaction_factory_eta_c_KKpi0.h"

//------------------
// init
//------------------
jerror_t DReaction_factory_eta_c_KKpi0::init(void)
{
	// Make as many DReaction objects as desired
	DReactionStep* locReactionStep = NULL;
	DReaction* locReaction = new DReaction("eta_c_KKpi0"); //needs to be a unique name for each DReaction object, CANNOT (!) be "Thrown"

	// DOCUMENTATION:
	// ANALYSIS library: https://halldweb1.jlab.org/wiki/index.php/GlueX_Analysis_Software
	// DReaction factory: https://halldweb1.jlab.org/wiki/index.php/Analysis_DReaction

	/**************************************************** eta_c_KKpi0 Reaction Steps ****************************************************/

	 
	//Required: DReactionSteps to specify the channel and decay chain you want to study
		//Particles are of type Particle_t, an enum defined in sim-recon/src/libraries/include/particleType.h

	////Example: g, p -> eta_c, (p)
	//locReactionStep = new DReactionStep();
	//locReactionStep->Set_InitialParticleID(Gamma);
	//locReactionStep->Set_TargetParticleID(Proton);
        //locReactionStep->Add_FinalParticleID(Eta_c);
	//locReactionStep->Add_FinalParticleID(Proton, false); //true: proton missing
        //locReaction->Add_ReactionStep(locReactionStep);
        //dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	// eta_c -> K+ K- pi0
        //locReactionStep = new DReactionStep();
	//locReactionStep->Set_InitialParticleID(Eta_c);
	//locReactionStep->Add_FinalParticleID(KPlus);
	//locReactionStep->Add_FinalParticleID(KMinus);
	//locReactionStep->Add_FinalParticleID(Pi0);
	//locReactionStep->Add_FinalParticleID(Proton, true); //true: proton missing
	//locReaction->Add_ReactionStep(locReactionStep);
	//dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	//Example: g, p -> K+, K-, pi0, (p)
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Gamma);
	locReactionStep->Set_TargetParticleID(Proton);
	locReactionStep->Add_FinalParticleID(KPlus);
	locReactionStep->Add_FinalParticleID(KMinus);
	locReactionStep->Add_FinalParticleID(Pi0);
	locReactionStep->Add_FinalParticleID(Proton, false); //true: proton missing
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	//Example: pi0 -> g, g
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Pi0);
	locReactionStep->Set_KinFitConstrainInitMassFlag(false);
	locReactionStep->Add_FinalParticleID(Gamma);
	locReactionStep->Add_FinalParticleID(Gamma);

	//optional: in a p4 kinematic fit, this will disable the constraint on the mass of the pi0
		//default is enabled, but you don't need to worry about setting false for the beam particle step
		//note that the omega and phi will always be false, regardless of what you set below
	//locReactionStep->Set_ApplyKinFitMassConstraintOnInitialParticleFlag(false);

	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak
	

	/**************************************************** eta_c_KKpi0 Control Settings ****************************************************/

	// Recommended: Type of kinematic fit to perform (default is d_NoFit)
		//fit types are of type DKinFitType, an enum defined in sim-recon/src/libraries/ANALYSIS/DKinFitResults.h
	locReaction->Set_KinFitType(d_P4AndVertexFit); //simultaneously constrain apply four-momentum conservation, invariant masses, and common-vertex constraints

	// Highly Recommended: When generating particle combinations, reject all photon candidates with a PID confidence level < 5.73303E-7 (+/- 5-sigma)
	//locReaction->Set_MinPhotonPIDFOM(5.73303E-7);

	// Highly Recommended: When generating particle combinations, reject all charged track candidates with a PID confidence level < 5.73303E-7 (+/- 5-sigma)
	//locReaction->Set_MinChargedPIDFOM(5.73303E-7);

	// Highly Recommended: When generating particle combinations, reject all beam photons that match to a different RF bunch (delta_t > 1.002 ns)
	//locReaction->Set_MaxPhotonRFDeltaT(0.5*4.008); //beam bunches are every 4.008 ns (2.004 should be minimum cut value)

	// Optional, use with caution: When generating particle combinations, but after the Photon/RF Delta-t cut, reject all combos with more than this # of beam photons
		//useful for missing-particle studies when you need a very pure sample of events
		//however, this can cut away a lot of signal events too
	// locReaction->Set_MaxNumBeamPhotonsInBunch(1);

	// Highly Recommended: Cut on number of extra "good" tracks. "Good" tracks are ones that survive the "PreSelect" (or user custom) factory.
		// Current (09/26/2014): "Good" tracks have a detector-hit match, and tracking FOM > 0.0027 (+/- 3 sigma). 
		// Important: Keep cut large: Can have many ghost and accidental tracks that look "good"
	//locReaction->Set_MaxExtraGoodTracks(4);

	// Highly Recommended: Enable ROOT TTree output for this DReaction
	//locReaction->Enable_TTreeOutput("tree_eta_c_KKpi0.root"); //string is file name (must end in ".root"!!): doen't need to be unique, feel free to change

	/************************************************** eta_c_KKpi0 Pre-Combo Custom Cuts *************************************************/

	// Highly Recommended: Very loose invariant mass cuts, applied during DParticleComboBlueprint construction
	// Example: pi0 -> g, g cut
	 locReaction->Set_InvariantMassCut(Pi0, 0.,0.3);//0.08, 0.19);

	// Highly Recommended: Very loose DAnalysisAction cuts, applied just after creating the combination (before saving it)
	// Example: Missing mass squared of proton
	// locReaction->Add_ComboPreSelectionAction(new DCutAction_MissingMassSquared(locReaction, false, -0.1, 2.56));

	/**************************************************** eta_c_KKpi0 Analysis Actions ****************************************************/

	
	// Recommended: Analysis actions automatically performed by the DAnalysisResults factories to histogram useful quantities.
		//These actions are executed sequentially, and are executed on each surviving (non-cut) particle combination 
		//Pre-defined actions can be found in ANALYSIS/DHistogramActions.h and ANALYSIS/DCutActions.h

	// PID & Kinematics
	//locReaction->Add_AnalysisAction(new DHistogramAction_PID(locReaction));
	//locReaction->Add_AnalysisAction(new DHistogramAction_TruePID(locReaction)); //momentum distributions of tracks with true/false PID (if thrown data available)

	// Transverse Momentum //Recommended for no-missing-particle reactions only!
	// locReaction->Add_AnalysisAction(new DHistogramAction_MissingTransverseMomentum(locReaction, false, 1000, 0.0, 1.0)); //false: fill histograms with measured particle data
	// locReaction->Add_AnalysisAction(new DCutAction_TransverseMomentum(locReaction, 0.4)); //Max Missing Pt of 0.4 GeV

	// Kinematic Fit Results
	// locReaction->Add_AnalysisAction(new DHistogramAction_KinFitResults(locReaction, 0.05)); //5% confidence level cut on pull histograms only
	// locReaction->Add_AnalysisAction(new DCutAction_KinFitFOM(locReaction, 0.0)); //0% confidence level cut //require kinematic fit converges

	// Kinematics
	//locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, false)); //false: fill histograms with measured particle data
	// locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, true, "KinFit")); //true: fill histograms with kinematic-fit particle data //"KinFit": unique name since action type is repeated
	//locReaction->Add_AnalysisAction(new DHistogramAction_TrackVertexComparison(locReaction));
	locReaction->Add_AnalysisAction(new DHistogramAction_KinFitResults(locReaction,0.05));
	locReaction->Add_AnalysisAction(new DCutAction_KinFitFOM(locReaction, 0.1));
	
	locReaction->Add_AnalysisAction(new DCutAction_InvariantMass(locReaction, Pi0, false, 0.12, 0.15));
	locReaction->Add_AnalysisAction(new DCutAction_ProtonPiPlusdEdx(locReaction, 2.2, false));	
	locReaction->Add_AnalysisAction(new DCustomAction_DalitzPlots(locReaction, false));//, "NoKinFit_Measured"));

	_data.push_back(locReaction); //Register the DReaction with the factory

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DReaction_factory_eta_c_KKpi0::fini(void)
{
	for(size_t loc_i = 0; loc_i < dReactionStepPool.size(); ++loc_i)
		delete dReactionStepPool[loc_i]; //cleanup memory
	return NOERROR;
}

