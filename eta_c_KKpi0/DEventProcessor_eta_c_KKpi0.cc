// $Id$
//
//    File: DEventProcessor_eta_c_KKpi0.cc
// Created: Wed Feb  3 11:04:34 EST 2016
// Creator: mpatsyuk (on Linux hidsk0001.cmsaf.mit.edu 2.6.32-358.el6.x86_64 x86_64)
//

#include "DEventProcessor_eta_c_KKpi0.h"

#include <vector>
#include <deque>
#include <string>
#include <iostream>

// Routine used to create our DEventProcessor

extern "C"
{
	void InitPlugin(JApplication *locApplication)
	{
		InitJANAPlugin(locApplication);
		locApplication->AddProcessor(new DEventProcessor_eta_c_KKpi0()); //register this plugin
		locApplication->AddFactoryGenerator(new DFactoryGenerator_eta_c_KKpi0()); //register the factory generator
	}
} // "C"

//------------------
// init
//------------------
jerror_t DEventProcessor_eta_c_KKpi0::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	 japp->RootWriteLock();
	{
		TDirectory* dir = new TDirectoryFile("DalitzPlotsThrown","DalitzPlotsThrown");
		dir->cd();

                dDalitz1_thrown = new TH2F("Dalitz1_thrown", "Dalitz; M^{2}_{K^{+}#pi0}; M^{2}_{K^{-}#pi0}", 200, 0.0, 8.0, 200, 0., 8.);
                dDalitz2_thrown = new TH2F("Dalitz2_thrown", "Dalitz; M^{2}_{K^{+}K^{-}}; M^{2}_{K^{+}#pi^{0}}", 200, 0.0, 8.0, 200, 0., 8.);
		dpi0Mass_thrown = new TH1F("dpi0Mass_thrown", "mass of #pi^{0} thrown", 300, 0.0, 0.3);
                dKplusMass_thrown = new TH1F("dKplusMass_thrown", "mass of K^{+} thrown", 300, 0.3, 0.6);
                dKminusMass_thrown = new TH1F("dKminusMass_thrown", "mass of K^{-} thrown", 300, 0.3, 0.6);
                dPhiMass_thrown = new TH1F("dPhiMass_thrown", "mass of fake #phi thrown", 250, 0.5, 3.);
                dEtaMass_thrown = new TH1F("dEtaMass_thrown", "mass of #eta_{c} thrown", 400, 0., 4.);
		
                dir->cd("../"); //return to the action directory

		dHist_IsEvent = new TH1D("IsEvent", "Is the event an event?", 2, -0.5, 1.5);
                dHist_IsEvent->GetXaxis()->SetBinLabel(1, "False");
                dHist_IsEvent->GetXaxis()->SetBinLabel(2, "True");
                //gDirectory->cd("..");
	}		
	 japp->RootUnLock();
	//

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t DEventProcessor_eta_c_KKpi0::brun(jana::JEventLoop* locEventLoop, int32_t locRunNumber)
{
	// This is called whenever the run number changes
	
	vector<const DMCThrown*> locMCThrowns;
	locEventLoop->Get(locMCThrowns);

	if(!locMCThrowns.empty()){
	}	

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
jerror_t DEventProcessor_eta_c_KKpi0::evnt(jana::JEventLoop* locEventLoop, uint64_t locEventNumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// locEventLoop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	
	japp->RootWriteLock();
        {
                dHist_IsEvent->Fill(1);
        }
        japp->RootUnLock();
	
	vector<const DMCThrown*> locMCThrowns;
	locEventLoop->Get(locMCThrowns);
	
	if(!locMCThrowns.empty()){
	
		TLorentzVector loc2K_P4, locPi0Kp_P4, locPi0Km_P4, locPi0_P4, locKp_P4, locKm_P4, locEta_P4;
		for(unsigned int i=0; i<locMCThrowns.size(); i++){
			const DMCThrown* mcthrown = locMCThrowns[i];
			if(mcthrown->type == 11){ //K+
                        	loc2K_P4 += locMCThrowns[i]->lorentzMomentum();
                        	locPi0Kp_P4 += locMCThrowns[i]->lorentzMomentum();
 				locKp_P4 = locMCThrowns[i]->lorentzMomentum();
				//cout<<"K+ m = "<<locMCThrowns[i]->lorentzMomentum().M()<<endl;
	               	}
                	if(mcthrown->type == 12){ //K-
                        	loc2K_P4 += locMCThrowns[i]->lorentzMomentum();
                        	locPi0Km_P4 += locMCThrowns[i]->lorentzMomentum();
				locKm_P4 = locMCThrowns[i]->lorentzMomentum();
				//cout<<"K- m = "<<locMCThrowns[i]->lorentzMomentum().M()<<endl;
                	}
                	if(mcthrown->type == 7 && mcthrown->myid < 4){ //pi0 from eta_c
				locPi0_P4 = locMCThrowns[i]->lorentzMomentum();
                        	locPi0Kp_P4 += locMCThrowns[i]->lorentzMomentum();
                        	locPi0Km_P4 += locMCThrowns[i]->lorentzMomentum();
				//cout<<"Pi0 m = "<<locMCThrowns[i]->lorentzMomentum().M()<<endl;
                	}
		}
		locEta_P4 += loc2K_P4;
		locEta_P4 += locPi0_P4;

		japp->RootWriteLock();
		{
		if(locPi0_P4.M() != 0 && locKp_P4.M() != 0 && locKm_P4.M() != 0){
		  dDalitz1_thrown->Fill(locPi0Kp_P4.M2(),locPi0Km_P4.M2());
                  dDalitz2_thrown->Fill(loc2K_P4.M2(), locPi0Kp_P4.M2());
		  dpi0Mass_thrown->Fill(locPi0_P4.M());
		  dKplusMass_thrown->Fill(locKp_P4.M());
		  dKminusMass_thrown->Fill(locKm_P4.M());
		  dPhiMass_thrown->Fill(loc2K_P4.M());
		  dEtaMass_thrown->Fill(locEta_P4.M());
		}
		}
		japp->RootUnLock();
	
	} // if mcthrowns are not empty

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
	locEventWriterROOT->Fill_DataTrees(locEventLoop, "eta_c_KKpi0");
	*/

	
	//Optional: Get the analysis results for all DReactions. 
		//Getting these objects triggers the analysis, if it wasn't performed already. 
		//These objects contain the DParticleCombo objects that survived the DAnalysisAction cuts that were added to the DReactions
	vector<const DAnalysisResults*> locAnalysisResultsVector;
	locEventLoop->Get(locAnalysisResultsVector);
	

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
		if(locParticleCombo->Get_Reaction()->Get_ReactionName() != "eta_c_KKpi0")
			continue; // particle combination was for a different reaction
		//perform further analysis steps here...
	}
	*/

	/*
	//Optional: Perform further cuts on the particle combos in the analysis results. 
	for(size_t loc_i = 0; loc_i < locAnalysisResultsVector.size(); ++loc_i)
	{
		const DAnalysisResults* locAnalysisResults = locAnalysisResultsVector[loc_i];
		if(locAnalysisResults->Get_Reaction()->Get_ReactionName() != "eta_c_KKpi0")
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
	locEventLoop->Write_RESTEvent(locEventLoop, "eta_c_KKpi0"); //string is part of output file name
	*/

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t DEventProcessor_eta_c_KKpi0::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DEventProcessor_eta_c_KKpi0::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

