// $Id$
//
//    File: DFactoryGenerator_eta_c_KKpi0.h
// Created: Wed Feb  3 11:04:34 EST 2016
// Creator: mpatsyuk (on Linux hidsk0001.cmsaf.mit.edu 2.6.32-358.el6.x86_64 x86_64)
//

#ifndef _DFactoryGenerator_eta_c_KKpi0_
#define _DFactoryGenerator_eta_c_KKpi0_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_eta_c_KKpi0.h"

class DFactoryGenerator_eta_c_KKpi0 : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_eta_c_KKpi0";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_eta_c_KKpi0());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_eta_c_KKpi0_

