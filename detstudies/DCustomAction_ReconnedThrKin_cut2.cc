// $Id$
//
//    File: DCustomAction_ReconnedThrKin_cut2.cc
// Created: Thu Feb  4 14:11:54 EST 2016
// Creator: mpatsyuk (on Linux hidsk0001.cmsaf.mit.edu 2.6.32-358.el6.x86_64 i686)
//

#include "DCustomAction_ReconnedThrKin_cut2.h"

void DCustomAction_ReconnedThrKin_cut2::Initialize(JEventLoop* locEventLoop)
{
	/*
	//Optional: Create histograms and/or modify member variables.
	//Create any histograms/trees/etc. within a ROOT lock. 
		//This is so that when running multithreaded, only one thread is writing to the ROOT file at a time. 
	//NEVER: Get anything from the JEventLoop while in a lock: May deadlock

	//When creating a reaction-independent action, only modify member variables within a ROOT lock. 
		//Objects created within a plugin (such as reaction-independent actions) can be accessed by many threads simultaneously. 

	japp->RootWriteLock(); //ACQUIRE ROOT LOCK!!
	{
		//Required: Create a folder in the ROOT output file that will contain all of the output ROOT objects (if any) for this action.
			//If another thread has already created the folder, it just changes to it. 
		CreateAndChangeTo_ActionDirectory();

		// Optional: Create a ROOT subfolder.
			//If another thread has already created the folder, it just changes to it. 
		// CreateAndChangeTo_Directory("MyDirName", "MyDirTitle");
			//make sub-directory content here
		// gDirectory->cd(".."); //return to the action directory

		//	(Optional) Example: Create a histogram. 
			// This function will return the histogram if already created by another thread. If not pre-existing, it will create and return it. 
			// Function arguments are identical to those used for the histogram constructors
		// dMyHist = GetOrCreate_Histogram<TH1I>("MyHistName", "MyHistTitle", 100, 0.0, 1.0);
	}
	japp->RootUnLock(); //RELEASE ROOT LOCK!!
	*/
	string locHistName, locHistTitle, locParticleName, locParticleROOTName;
	Particle_t locPID;

	const DAnalysisUtilities* locAnalysisUtilities = NULL;
	locEventLoop->GetSingle(locAnalysisUtilities);

	//CREATE THE HISTOGRAMS
	japp->RootWriteLock(); //ACQUIRE ROOT LOCK!!
	{
		dAnalysisUtilities = locAnalysisUtilities;
		//Required: Create a folder in the ROOT output file that will contain all of the output ROOT objects (if any) for this action.
			//If another thread has already created the folder, it just changes to it.
		CreateAndChangeTo_ActionDirectory();

		// Beam Particle
		locPID = Gamma;
		locParticleName = string("Beam_") + ParticleType(locPID);
		locParticleROOTName = ParticleName_ROOT(locPID);
		CreateAndChangeTo_Directory(locParticleName, locParticleName);

		locHistName = "Momentum";
		locHistTitle = string("Thrown Beam ") + locParticleROOTName + string(";p (GeV/c)");
		dBeamParticle_P = GetOrCreate_Histogram<TH1I>(locHistName, locHistTitle, dNumPBins, dMinP, dMaxP);
		gDirectory->cd("..");

		//PID
		CreateAndChangeTo_Directory("PID", "PID");
		{
			//beta vs p
			locHistName = "BetaVsP_Q+";
			locHistTitle = "q^{+};p (GeV/c);#beta";
			dHistMap_QBetaVsP[1] = GetOrCreate_Histogram<TH2I>(locHistName, locHistTitle, dNum2DPBins, dMinP, dMaxP, dNumBetaBins, dMinBeta, dMaxBeta);

			locHistName = "BetaVsP_Q-";
			locHistTitle = "q^{-};p (GeV/c);#beta";
			dHistMap_QBetaVsP[-1] = GetOrCreate_Histogram<TH2I>(locHistName, locHistTitle, dNum2DPBins, dMinP, dMaxP, dNumBetaBins, dMinBeta, dMaxBeta);
		}
		gDirectory->cd("..");

		for(size_t loc_i = 0; loc_i < dFinalStatePIDs.size(); ++loc_i)
		{
			locPID = dFinalStatePIDs[loc_i];
			locParticleName = ParticleType(locPID);
			locParticleROOTName = ParticleName_ROOT(locPID);
			CreateAndChangeTo_Directory(locParticleName, locParticleName);

			// Momentum
			locHistName = "Momentum";
			locHistTitle = string("Thrown ") + locParticleROOTName + string(";p (GeV/c)");
			dHistMap_P[locPID] = GetOrCreate_Histogram<TH1I>(locHistName, locHistTitle, dNumPBins, dMinP, dMaxP);

			// Theta
			locHistName = "Theta";
			locHistTitle = string("Thrown ") + locParticleROOTName + string(";#theta#circ");
			dHistMap_Theta[locPID] = GetOrCreate_Histogram<TH1I>(locHistName, locHistTitle, dNumThetaBins, dMinTheta, dMaxTheta);

			// Phi
			locHistName = "Phi";
			locHistTitle = string("Thrown ") + locParticleROOTName + string(";#phi#circ");
			dHistMap_Phi[locPID] = GetOrCreate_Histogram<TH1I>(locHistName, locHistTitle, dNumPhiBins, dMinPhi, dMaxPhi);

			// P Vs Theta
			locHistName = "PVsTheta";
			locHistTitle = string("Thrown ") + locParticleROOTName + string(";#theta#circ;p (GeV/c)");
			dHistMap_PVsTheta[locPID] = GetOrCreate_Histogram<TH2I>(locHistName, locHistTitle, dNum2DThetaBins, dMinTheta, dMaxTheta, dNum2DPBins, dMinP, dMaxP);

			// Phi Vs Theta
			locHistName = "PhiVsTheta";
			locHistTitle = string("Thrown ") + locParticleROOTName + string(";#theta#circ;#phi#circ");
			dHistMap_PhiVsTheta[locPID] = GetOrCreate_Histogram<TH2I>(locHistName, locHistTitle, dNum2DThetaBins, dMinTheta, dMaxTheta, dNum2DPhiBins, dMinPhi, dMaxPhi);

			// Vertex-Z
			locHistName = "VertexZ";
			locHistTitle = string("Thrown ") + locParticleROOTName + string(";Vertex-Z (cm)");
			dHistMap_VertexZ[locPID] = GetOrCreate_Histogram<TH1I>(locHistName, locHistTitle, dNumVertexZBins, dMinVertexZ, dMaxVertexZ);

			// Vertex-Y Vs Vertex-X
			locHistName = "VertexYVsX";
			locHistTitle = string("Thrown ") + locParticleROOTName + string(";Vertex-X (cm);Vertex-Y (cm)");
			dHistMap_VertexYVsX[locPID] = GetOrCreate_Histogram<TH2I>(locHistName, locHistTitle, dNumVertexXYBins, dMinVertexXY, dMaxVertexXY, dNumVertexXYBins, dMinVertexXY, dMaxVertexXY);

			// Vertex-T
			locHistName = "VertexT";
			locHistTitle = string("Thrown ") + locParticleROOTName + string(";Vertex-T (ns)");
			dHistMap_VertexT[locPID] = GetOrCreate_Histogram<TH1I>(locHistName, locHistTitle, dNumTBins, dMinT, dMaxT);

			gDirectory->cd("..");
		}

		//Return to the base directory
		ChangeTo_BaseDirectory();
	}
	japp->RootUnLock(); //RELEASE ROOT LOCK!!
}

bool DCustomAction_ReconnedThrKin_cut2::Perform_Action(JEventLoop* locEventLoop, const DParticleCombo* locParticleCombo)
{
	//Write custom code to perform an action on the INPUT DParticleCombo (DParticleCombo)
	//NEVER: Grab DParticleCombo or DAnalysisResults objects (of any tag!) from the JEventLoop within this function
	//NEVER: Grab objects that are created post-kinfit (e.g. DKinFitResults, etc.) from the JEventLoop if Get_UseKinFitResultsFlag() == false: CAN CAUSE INFINITE DEPENDENCY LOOP
	//NEVER: Get anything from the JEventLoop while in a lock: May deadlock


	//Expect locParticleCombo to be NULL since this is a reaction-independent action.


	// Optional: Useful utility functions.
	// const DAnalysisUtilities* locAnalysisUtilities;
	// locEventLoop->GetSingle(locAnalysisUtilities);

	//Optional: check whether the user wanted to use the kinematic fit results when performing this action
	// bool locUseKinFitResultsFlag = Get_UseKinFitResultsFlag();


	//Optional: Quit the action if it has already been executed this event (else may result in double-counting when filling histograms)
	// if(Get_NumPreviousParticleCombos() != 0)
	//		return true;

	/*
	//Optional: Fill histograms
	japp->RootWriteLock(); //ACQUIRE ROOT LOCK!!
	{
		// Fill any histograms here
	}
	japp->RootUnLock(); //RELEASE ROOT LOCK!!
	*/

	 vector<const DMCThrown*> locMCThrowns, locMCThrowns_Decaying;
	locEventLoop->Get(locMCThrowns, "FinalState");
	locEventLoop->Get(locMCThrowns_Decaying, "Decaying");
	locMCThrowns.insert(locMCThrowns.begin(), locMCThrowns_Decaying.begin(), locMCThrowns_Decaying.end());
	if(locMCThrowns.empty())
		return true; //e.g. non-simulated event

	if(Get_NumPreviousParticleCombos() != 0)
		return true; //else double-counting!

	const DMCThrownMatching* locMCThrownMatching = NULL;
	locEventLoop->GetSingle(locMCThrownMatching);

	vector<const DBeamPhoton*> locBeamPhotons;
	locEventLoop->Get(locBeamPhotons);
	japp->RootWriteLock();
	{
		for(size_t loc_i = 0; loc_i < locBeamPhotons.size(); ++loc_i)
			dBeamParticle_P->Fill(locBeamPhotons[loc_i]->energy());
	}
	japp->RootUnLock();

	for(size_t loc_i = 0; loc_i < locMCThrowns.size(); ++loc_i)
	{
		const DMCThrown* locMCThrown = locMCThrowns[loc_i];
		Particle_t locPID = (Particle_t)locMCThrown->type;

		double locMatchFOM = 0.0;
		double locBeta_Timing = 0.0;
		if(ParticleCharge(locPID) != 0)
		{
			const DChargedTrackHypothesis* locChargedTrackHypothesis = locMCThrownMatching->Get_MatchingChargedHypothesis(locMCThrown, locMatchFOM);
			if(locChargedTrackHypothesis == NULL)
				continue; //not reconstructed
			// track fit FOM cut!!!
			const DTrackTimeBased* locTrackTimeBased = NULL;
			locChargedTrackHypothesis->GetSingle(locTrackTimeBased);
			if(locTrackTimeBased->FOM < 6.e-7) //2.7e-3)
				continue;
			locBeta_Timing = locChargedTrackHypothesis->measuredBeta();
		}
		else
		{
			const DNeutralParticleHypothesis* locNeutralParticleHypothesis = locMCThrownMatching->Get_MatchingNeutralHypothesis(locMCThrown, locMatchFOM);
			if(locNeutralParticleHypothesis == NULL)
				continue; //not reconstructed
			locBeta_Timing = locNeutralParticleHypothesis->measuredBeta();
		}

		DVector3 locMomentum = locMCThrown->momentum();
		double locPhi = locMomentum.Phi()*180.0/TMath::Pi();
		double locTheta = locMomentum.Theta()*180.0/TMath::Pi();
		double locP = locMomentum.Mag();
		int locCharge = ParticleCharge(locPID);

		japp->RootWriteLock();
		{
			if(dHistMap_QBetaVsP.find(locCharge) != dHistMap_QBetaVsP.end())
				dHistMap_QBetaVsP[locCharge]->Fill(locP, locBeta_Timing);
			if(dHistMap_P.find(locPID) != dHistMap_P.end())
			{
				dHistMap_P[locPID]->Fill(locP);
				dHistMap_Phi[locPID]->Fill(locPhi);
				dHistMap_Theta[locPID]->Fill(locTheta);
				dHistMap_PVsTheta[locPID]->Fill(locTheta, locP);
				dHistMap_PhiVsTheta[locPID]->Fill(locTheta, locPhi);
				dHistMap_VertexZ[locPID]->Fill(locMCThrown->position().Z());
				dHistMap_VertexYVsX[locPID]->Fill(locMCThrown->position().X(), locMCThrown->position().Y());
				dHistMap_VertexT[locPID]->Fill(locMCThrown->time());
			}
		}
		japp->RootUnLock();
	}
	return true; //return false if you want to use this action to apply a cut (and it fails the cut)

}
