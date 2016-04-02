// $Id$
//
//    File: DReaction_factory_eta_c_KKpi0.h
// Created: Wed Feb  3 11:04:34 EST 2016
// Creator: mpatsyuk (on Linux hidsk0001.cmsaf.mit.edu 2.6.32-358.el6.x86_64 x86_64)
//

#ifndef _DReaction_factory_eta_c_KKpi0_
#define _DReaction_factory_eta_c_KKpi0_

#include <iostream>
#include <iomanip>

#include <JANA/JFactory.h>
#include <ANALYSIS/DReaction.h>
#include <ANALYSIS/DHistogramActions.h>
#include <ANALYSIS/DCutActions.h>

#include "DCustomAction_DalitzPlots.h"

using namespace std;
using namespace jana;

class DReaction_factory_eta_c_KKpi0 : public jana::JFactory<DReaction>
{
	public:
		DReaction_factory_eta_c_KKpi0()
		{
			// This is so that the created DReaction objects persist throughout the life of the program instead of being cleared each event. 
			SetFactoryFlag(PERSISTANT);
		}
		const char* Tag(void){return "eta_c_KKpi0";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		deque<DReactionStep*> dReactionStepPool; //to prevent memory leaks
};

#endif // _DReaction_factory_eta_c_KKpi0_

