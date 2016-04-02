// $Id$
//
//    File: JEventProcessor_detstudies.h
// Created: Tue Jan 19 09:55:05 EST 2016
// Creator: mpatsyuk (on Darwin PATSYUKMAC.MIT.EDU 15.0.0 i386)
//

#ifndef _JEventProcessor_detstudies_
#define _JEventProcessor_detstudies_

#include <TFile.h>
#include <TROOT.h>

#include <JANA/JEventProcessor.h>

#include <DANA/DApplication.h>
#include <ANALYSIS/DHistogramActions.h>

#include "DCustomAction_ReconnedThrownKin_CUT.h"
#include "DCustomAction_ReconnedThrKin_cut2.h"
//#include <JANA/JEventProcessor.h>

class JEventProcessor_detstudies:public jana::JEventProcessor{
	public:
		JEventProcessor_detstudies();
		~JEventProcessor_detstudies();
		const char* className(void){return "JEventProcessor_detstudies";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
		DHistogramAction_TrackMultiplicity dHistogramAction_TrackMultiplicity;
		DHistogramAction_ThrownParticleKinematics dHistogramAction_ThrownParticleKinematics;
		DHistogramAction_DetectedParticleKinematics dHistogramAction_DetectedParticleKinematics;
		DHistogramAction_GenReconTrackComparison dHistogramAction_GenReconTrackComparison;
		DHistogramAction_NumReconstructedObjects dHistogramAction_NumReconstructedObjects;
		DHistogramAction_ReconnedThrownKinematics dHistogramAction_ReconnedThrownKinematics;
		DHistogramAction_EventVertex dHistogramAction_EventVertex;

		DHistogramAction_DetectorMatchParams dHistogramAction_DetectorMatchParams;
		DHistogramAction_Neutrals dHistogramAction_Neutrals;
		DHistogramAction_DetectorPID dHistogramAction_DetectorPID;
		DHistogramAction_DetectorMatching dHistogramAction_DetectorMatching;
		DHistogramAction_Reconstruction dHistogramAction_Reconstruction;
//		DHistogramAction_ObjectMemory dHistogramAction_ObjectMemory;

		DCustomAction_ReconnedThrownKin_CUT dCustomAction_ReconnedThrownKin_CUT;
		DCustomAction_ReconnedThrKin_cut2 dCustomAction_ReconnedThrKin_cut2;
		TH1D* dHist_IsEvent; //counts events
};

#endif // _JEventProcessor_detstudies_

