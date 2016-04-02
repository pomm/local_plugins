// $Id$
//
//    File: DCustomAction_DalitzPlots.cc
// Created: Fri Feb  5 10:42:19 EST 2016
// Creator: mpatsyuk (on Linux hidsk0001.cmsaf.mit.edu 2.6.32-358.el6.x86_64 x86_64)
//

#include "DCustomAction_DalitzPlots.h"

void DCustomAction_DalitzPlots::Initialize(JEventLoop* locEventLoop)
{
	
	//Optional: Create histograms and/or modify member variables.
	//Create any histograms/trees/etc. within a ROOT lock. 
		//This is so that when running multithreaded, only one thread is writing to the ROOT file at a time. 
	//NEVER: Get anything from the JEventLoop while in a lock: May deadlock

	japp->RootWriteLock(); //ACQUIRE ROOT LOCK!!
	{
		//Required: Create a folder in the ROOT output file that will contain all of the output ROOT objects (if any) for this action.
			//If another thread has already created the folder, it just changes to it. 
		CreateAndChangeTo_ActionDirectory();

		CreateAndChangeTo_Directory("Reconstructed_1", "Dalitz plots for reconstructed particles");
		dDalitz1_reco1 = GetOrCreate_Histogram<TH2F>("Dalitz1_reco1", "Dalitz; M^{2}_{K^{+}#pi0}; M^{2}_{K^{-}#pi0}", 200, 0.0, 8.0, 200, 0., 8.);
		dDalitz2_reco1 = GetOrCreate_Histogram<TH2F>("Dalitz2_reco1", "Dalitz; M^{2}_{K^{+}K^{-}}; M^{2}_{K^{+}#pi^{0}}", 200, 0.0, 8.0, 200, 0., 8.);

		gDirectory->cd(".."); //return to the action directory

		CreateAndChangeTo_Directory("Masses reco", "Masses for reconstructed particles");
                dpi0Mass = GetOrCreate_Histogram<TH1F>("dpi0Mass", "mass of #pi^{0}", 300, 0.0, 0.3);
                dKplusMass = GetOrCreate_Histogram<TH1F>("dKplusMass", "mass of K^{+}", 300, 0.3, 0.6);
		dKminusMass = GetOrCreate_Histogram<TH1F>("dKminusMass", "mass of K^{-}", 300, 0.3, 0.6);
		dPhiMass = GetOrCreate_Histogram<TH1F>("dPhiMass", "mass of fake #phi", 250, 0.5, 3.);
		dEtaMass = GetOrCreate_Histogram<TH1F>("dEtaMass", "mass of #eta_{c}", 400, 0., 4.);		
		
		gDirectory->cd("..");
		
		CreateAndChangeTo_Directory("PID match", "matching MC to reco particles");
		dPIDrecoMatch = GetOrCreate_Histogram<TH2I>("dPIDrecoMatch","Matching reco to MC particles; PID(MC); PID(reco)", 20,-0.5,19.5,20,-0.5,19.5);
		dPIDetac = GetOrCreate_Histogram<TH1I>("dPIDetac","PID of particles from the bump; PID",100,-0.5,99.5);		

		gDirectory->cd("..");
	}
	japp->RootUnLock(); //RELEASE ROOT LOCK!!
	
}

bool DCustomAction_DalitzPlots::Perform_Action(JEventLoop* locEventLoop, const DParticleCombo* locParticleCombo)
{
	vector<const DMCThrownMatching*> locMCThrownMatchingVector;
        locEventLoop->Get(locMCThrownMatchingVector);

        const DMCThrown* locMCThrownGamma1 = NULL;
        const DMCThrown* locMCThrownGamma2 = NULL;
        const DMCThrown* locMCThrownKp = NULL;
        const DMCThrown* locMCThrownKm = NULL;
        double locMatchFOMg1, locMatchFOMg2, locMatchFOMkp, locMatchFOMkm;

	const DParticleComboStep* locParticleComboStep1 = locParticleCombo->Get_ParticleComboStep(1);

	//0. reconstruct pi0:
	deque<const DKinematicData*> locGammas;
	locParticleComboStep1->Get_FinalParticles(locGammas);
	DLorentzVector locPi0_P4;
	locPi0_P4 += locGammas[0]->lorentzMomentum();
	locPi0_P4 += locGammas[1]->lorentzMomentum();
	//cout<<"gammas pid: "<<locGammas[0]->PID()<<" and "<<locGammas[1]->PID()<<endl;
	const DNeutralShower* locNeutralShower1 = static_cast<const DNeutralShower*>(locParticleComboStep1->Get_FinalParticle_SourceObject(0));
	const DNeutralShower* locNeutralShower2 = static_cast<const DNeutralShower*>(locParticleComboStep1->Get_FinalParticle_SourceObject(1));
	locMCThrownGamma1 = locMCThrownMatchingVector[0]->Get_MatchingMCThrown(locNeutralShower1, locMatchFOMg1);
	locMCThrownGamma2 = locMCThrownMatchingVector[0]->Get_MatchingMCThrown(locNeutralShower2, locMatchFOMg2);

	const DParticleComboStep* locParticleComboStep = locParticleCombo->Get_ParticleComboStep(0);
	
	// 1. get final state particles:
	deque<const DKinematicData*> locParticles;
	locParticleComboStep->Get_FinalParticles(locParticles);
	
	//std::cout<<locParticles[0]->PID()<<", "<<locParticles[1]->PID()<<std::endl;
	//std::cout<<locParticles[2]->PID()<<"++++"<<std::endl;
	
	if(!locParticles.empty() && !locGammas.empty()){

	DLorentzVector loc2K_P4_a, locPi0Kp_P4_a, locPi0Km_P4_a;
	DLorentzVector locKp_P4, locKm_P4;
	locPi0Kp_P4_a = locPi0_P4;
	locPi0Km_P4_a = locPi0_P4;
	const DChargedTrack* locChargedTrackKp;
	const DChargedTrack* locChargedTrackKm;
	// calculate masses for Dalitz plot:
	for(size_t loc_i=0; loc_i < 2; loc_i++){
		if(locParticles[loc_i]->PID() == 11){ // Kplus
			loc2K_P4_a += locParticles[loc_i]->lorentzMomentum();
			locPi0Kp_P4_a += locParticles[loc_i]->lorentzMomentum();
			locKp_P4 = locParticles[loc_i]->lorentzMomentum();
			locChargedTrackKp = static_cast<const DChargedTrack*>(locParticleComboStep->Get_FinalParticle_SourceObject(loc_i));
			//cout<<"PID Kp = "<<locChargedTrackKp->pid<<endl;
		}
		if(locParticles[loc_i]->PID() == 12){ // Kminus
			loc2K_P4_a += locParticles[loc_i]->lorentzMomentum();
			locPi0Km_P4_a += locParticles[loc_i]->lorentzMomentum();
			locKm_P4 = locParticles[loc_i]->lorentzMomentum();
			locChargedTrackKm = static_cast<const DChargedTrack*>(locParticleComboStep->Get_FinalParticle_SourceObject(loc_i));
		}
	}

	locMCThrownKp = locMCThrownMatchingVector[0]->Get_MatchingMCThrown(locChargedTrackKp, locMatchFOMkp);
	locMCThrownKm = locMCThrownMatchingVector[0]->Get_MatchingMCThrown(locChargedTrackKm, locMatchFOMkm); 

	DLorentzVector locEta_c_P4;
	locEta_c_P4 += loc2K_P4_a;
	locEta_c_P4 += locPi0_P4;
/*
	// 2. reconstructed Kaons
	DLorentzVector locKp_P4;
	DLorentzVector locKm_P4;
	const DChargedTrack* locChargedTrack_Kp = static_cast<const DChargedTrack*>(locParticleComboStep->Get_FinalParticle_SourceObject(1));
	const DChargedTrackHypothesis* locChargedTrackHypothesis_Kp = locChargedTrack_Kp->Get_Hypothesis(KPlus);
	const DChargedTrack* locChargedTrack_Km = static_cast<const DChargedTrack*>(locParticleComboStep->Get_FinalParticle_SourceObject(2));
        const DChargedTrackHypothesis* locChargedTrackHypothesis_Km = locChargedTrack_Km->Get_Hypothesis(KMinus);
	locKp_P4 = locChargedTrackHypothesis_Kp->lorentzMomentum();
	locKm_P4 = locChargedTrackHypothesis_Km->lorentzMomentum();
	DLorentzVector loc2K_P4 = locKp_P4; loc2K_P4 += locKm_P4;

	// calculate pi0 mass
	DLorentzVector locPi0_P4 = dAnalysisUtilities->Calc_FinalStateP4(locParticleCombo, 3, Get_UseKinFitResultsFlag());	

	DLorentzVector locPi0Kp_P4 = locKp_P4; locPi0Kp_P4 += locPi0_P4;
	DLorentzVector locPi0Km_P4 = locKm_P4; locPi0Km_P4 += locPi0_P4;
*/	

	japp->RootWriteLock(); //ACQUIRE ROOT LOCK!!
	{
		//if(locPi0_P4.M() > 0.10 && locPi0_P4.M() < 0.16){
	//		dDalitz1_reco1->Fill(locPi0Kp_P4.M2(),locPi0Km_P4.M2());
	//		dDalitz2_reco1->Fill(loc2K_P4.M2(), locPi0Kp_P4.M2());
		//}
	if(locEta_c_P4.M() >= 2.88 && locEta_c_P4.M() <=3.08){        

        	dDalitz1_reco1->Fill(locPi0Kp_P4_a.M2(),locPi0Km_P4_a.M2());
                dDalitz2_reco1->Fill(loc2K_P4_a.M2(), locPi0Kp_P4_a.M2());
		//fill mass histos
		dKplusMass->Fill(locKp_P4.M());
		dKminusMass->Fill(locKm_P4.M());
		dPhiMass->Fill(loc2K_P4_a.M());
		
		// fill in PID of particles ending up in the Dalitz plot:
		//cout<<"KAON PID = "<<locMCThrownKp->PID()<<endl;
        	//dPIDrecoMatch->Fill(locMCThrownKp->PID(), 11);
        	//dPIDrecoMatch->Fill(locMCThrownKm->PID(), 12);
        	//dPIDrecoMatch->Fill(locMCThrownGamma1->PID(), 1);
        	//dPIDrecoMatch->Fill(locMCThrownGamma2->PID(), 1);
	}//if eta_c mass
	dEtaMass->Fill(locEta_c_P4.M()); // plot eta mass without cuts
	if(locEta_c_P4.M() < 2.8){
		//dPIDetac->Fill(locMCThrownKp->PID());
		//dPIDetac->Fill(locMCThrownKm->PID());
	}
	dpi0Mass->Fill(locPi0_P4.M());
	}
	japp->RootUnLock(); //RELEASE ROOT LOCK!!
	}// if particles not empty

	return true; //return false if you want to use this action to apply a cut (and it fails the cut!)
}
