// $Id$
//
//    File: DCustomAction_ReconnedThrownKin_CUT.h
// Created: Mon Jan 25 10:21:57 EST 2016
// Creator: mpatsyuk (on Darwin PATSYUKMAC.MIT.EDU 15.0.0 i386)
//

#ifndef _DCustomAction_ReconnedThrownKin_CUT_
#define _DCustomAction_ReconnedThrownKin_CUT_

#include <string>
#include <iostream>

#include "JANA/JEventLoop.h"
#include "JANA/JApplication.h"

#include "ANALYSIS/DAnalysisAction.h"
#include "ANALYSIS/DReaction.h"
#include "ANALYSIS/DParticleCombo.h"
#include "ANALYSIS/DAnalysisUtilities.h"

using namespace std;
using namespace jana;

class DCustomAction_ReconnedThrownKin_CUT : public DAnalysisAction
{
	public:

		//user can call any of these three constructors
		DCustomAction_ReconnedThrownKin_CUT(const DReaction* locReaction, string locActionUniqueString = "") : 
                DAnalysisAction(locReaction, "Custom_ReconnedThrownKin_CUT", false, locActionUniqueString),
		dMinThrownMatchFOM(5.73303E-7), dNumPBins(500), dNumThetaBins(560), dNumPhiBins(360), dNumVertexZBins(600), dNumTBins(400), dNumVertexXYBins(400),
		dNum2DPBins(250), dNum2DThetaBins(140), dNum2DPhiBins(180), dNumBetaBins(400),
		dMinT(-20.0), dMaxT(20.0), dMinP(0.0), dMaxP(10.0), dMinTheta(0.0), dMaxTheta(140.0), dMinPhi(-180.0), dMaxPhi(180.0), dMinVertexZ(0.0),
		dMaxVertexZ(200.0), dMinVertexXY(-10.0), dMaxVertexXY(10.0), dMinBeta(-0.2), dMaxBeta(1.2)
                {
		        dFinalStatePIDs.clear();
			dFinalStatePIDs.push_back(Gamma);  dFinalStatePIDs.push_back(Neutron);
			dFinalStatePIDs.push_back(PiPlus);  dFinalStatePIDs.push_back(KPlus);  dFinalStatePIDs.push_back(Proton);
			dFinalStatePIDs.push_back(PiMinus);  dFinalStatePIDs.push_back(KMinus);

			dAnalysisUtilities = NULL;
		}

		DCustomAction_ReconnedThrownKin_CUT(string locActionUniqueString) : 
		DAnalysisAction(NULL, "Custom_ReconnedThrownKin_CUT", false, locActionUniqueString), 
		dMinThrownMatchFOM(5.73303E-7), dNumPBins(500), dNumThetaBins(560), dNumPhiBins(360), dNumVertexZBins(600), dNumTBins(400), dNumVertexXYBins(400),
		dNum2DPBins(250), dNum2DThetaBins(140), dNum2DPhiBins(180), dNumBetaBins(400),
		dMinT(-20.0), dMaxT(20.0), dMinP(0.0), dMaxP(10.0), dMinTheta(0.0), dMaxTheta(140.0), dMinPhi(-180.0), dMaxPhi(180.0), dMinVertexZ(0.0),
		dMaxVertexZ(200.0), dMinVertexXY(-10.0), dMaxVertexXY(10.0), dMinBeta(-0.2), dMaxBeta(1.2)
		{
		        dFinalStatePIDs.clear();
			dFinalStatePIDs.push_back(Gamma);  dFinalStatePIDs.push_back(Neutron);
			dFinalStatePIDs.push_back(PiPlus);  dFinalStatePIDs.push_back(KPlus);  dFinalStatePIDs.push_back(Proton);
			dFinalStatePIDs.push_back(PiMinus);  dFinalStatePIDs.push_back(KMinus);

			dAnalysisUtilities = NULL;
		}

		DCustomAction_ReconnedThrownKin_CUT(void) : 
		DAnalysisAction(NULL, "Custom_ReconnedThrownKin_CUT", false, ""), 
		dMinThrownMatchFOM(5.73303E-7), dNumPBins(500), dNumThetaBins(560), dNumPhiBins(360), dNumVertexZBins(600), dNumTBins(400), dNumVertexXYBins(400),
		dNum2DPBins(250), dNum2DThetaBins(140), dNum2DPhiBins(180), dNumBetaBins(400),
		dMinT(-20.0), dMaxT(20.0), dMinP(0.0), dMaxP(10.0), dMinTheta(0.0), dMaxTheta(140.0), dMinPhi(-180.0), dMaxPhi(180.0), dMinVertexZ(0.0),
		dMaxVertexZ(200.0), dMinVertexXY(-10.0), dMaxVertexXY(10.0), dMinBeta(-0.2), dMaxBeta(1.2)
		{
		        dFinalStatePIDs.clear();
			dFinalStatePIDs.push_back(Gamma);  dFinalStatePIDs.push_back(Neutron);
			dFinalStatePIDs.push_back(PiPlus);  dFinalStatePIDs.push_back(KPlus);  dFinalStatePIDs.push_back(Proton);
			dFinalStatePIDs.push_back(PiMinus);  dFinalStatePIDs.push_back(KMinus);

			dAnalysisUtilities = NULL;
		}

		double dMinThrownMatchFOM;

		unsigned int dNumPBins, dNumThetaBins, dNumPhiBins, dNumVertexZBins, dNumTBins, dNumVertexXYBins, dNum2DPBins, dNum2DThetaBins, dNum2DPhiBins, dNumBetaBins;
		double dMinT, dMaxT, dMinP, dMaxP, dMinTheta, dMaxTheta, dMinPhi, dMaxPhi, dMinVertexZ, dMaxVertexZ, dMinVertexXY, dMaxVertexXY, dMinBeta, dMaxBeta;

		deque<Particle_t> dFinalStatePIDs;
		
		void Initialize(JEventLoop* locEventLoop);

	private:

		bool Perform_Action(JEventLoop* locEventLoop, const DParticleCombo* locParticleCombo = NULL);

		//Store any histograms as member variables here
		const DAnalysisUtilities* dAnalysisUtilities;

		TH1I* 	dBeamParticle_P;

		//PID
		map<int, TH2I*> dHistMap_QBetaVsP; //int is charge: -1, 1

		map<Particle_t, TH2I*> dHistMap_PVsTheta;
		map<Particle_t, TH2I*> dHistMap_PhiVsTheta;
		map<Particle_t, TH1I*> dHistMap_P;
		map<Particle_t, TH1I*> dHistMap_Theta;
		map<Particle_t, TH1I*> dHistMap_Phi;
		map<Particle_t, TH1I*> dHistMap_VertexZ;
		map<Particle_t, TH2I*> dHistMap_VertexYVsX;
		map<Particle_t, TH1I*> dHistMap_VertexT;
};

#endif // _DCustomAction_ReconnedThrownKin_CUT_

