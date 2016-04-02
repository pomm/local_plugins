// $Id$
//
//    File: DEventProcessor_eta_c_KKpi0.h
// Created: Wed Feb  3 11:04:34 EST 2016
// Creator: mpatsyuk (on Linux hidsk0001.cmsaf.mit.edu 2.6.32-358.el6.x86_64 x86_64)
//

#ifndef _DEventProcessor_eta_c_KKpi0_
#define _DEventProcessor_eta_c_KKpi0_

#include <TROOT.h>

#include <JANA/JEventProcessor.h>
#include <JANA/JApplication.h>

#include <ANALYSIS/DEventWriterROOT.h>
#include <HDDM/DEventWriterREST.h>
#include <ANALYSIS/DHistogramActions.h>

#include "DFactoryGenerator_eta_c_KKpi0.h"

using namespace jana;
using namespace std;

class DEventProcessor_eta_c_KKpi0 : public jana::JEventProcessor
{
	public:
		const char* className(void){return "DEventProcessor_eta_c_KKpi0";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop* locEventLoop, int32_t locRunNumber);	///< Called every time a new run number is detected.
		jerror_t evnt(jana::JEventLoop* locEventLoop, uint64_t locEventNumber);	///< Called every event.
		jerror_t erun(void);						///< Called every time run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		//For non-custom reaction-independent histograms, it is recommended that you simultaneously run the monitoring_hists plugin instead of defining them here
		TH1D* dHist_IsEvent;
		TH2F *dDalitz1_thrown, *dDalitz2_thrown;
		TH1F *dpi0Mass_thrown, *dKplusMass_thrown, *dKminusMass_thrown, *dPhiMass_thrown, *dEtaMass_thrown; 

};

#endif // _DEventProcessor_eta_c_KKpi0_

