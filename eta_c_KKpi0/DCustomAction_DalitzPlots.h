// $Id$
//
//    File: DCustomAction_DalitzPlots.h
// Created: Fri Feb  5 10:42:19 EST 2016
// Creator: mpatsyuk (on Linux hidsk0001.cmsaf.mit.edu 2.6.32-358.el6.x86_64 x86_64)
//

#ifndef _DCustomAction_DalitzPlots_
#define _DCustomAction_DalitzPlots_

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

class DCustomAction_DalitzPlots : public DAnalysisAction
{
	public:

		DCustomAction_DalitzPlots(const DReaction* locReaction, bool locUseKinFitResultsFlag, string locActionUniqueString = "") : 
		DAnalysisAction(locReaction, "Custom_DalitzPlots", locUseKinFitResultsFlag, locActionUniqueString) {}

		void Initialize(JEventLoop* locEventLoop);

	private:

		bool Perform_Action(JEventLoop* locEventLoop, const DParticleCombo* locParticleCombo);

		const DAnalysisUtilities* dAnalysisUtilities;

		//Store any histograms as member variables here
		TH2F *dDalitz1_reco1, *dDalitz2_reco1;
		TH1F* dpi0Mass;
		TH1F* dKplusMass;
		TH1F* dKminusMass;
		TH1F* dPhiMass;
		TH1F* dEtaMass;
		TH2I* dPIDrecoMatch;
		TH1I* dPIDetac;
};

#endif // _DCustomAction_DalitzPlots_

