/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <emscripten.h>
#include <bind.h>
#include "GUIDOEngine.h"
#include "GUIDOEngineAdapter.h"
#include "GUIDOInternal.h"
#include "GuidoParser.h"

using namespace emscripten;

/* Structure and enum binding.
 * This structures can be created and manipulated in javascript side like json object and passed to C++ method.
 */
EMSCRIPTEN_BINDINGS(CStruct) {

	emscripten::value_object<GuidoLayoutSettings>("GuidoLayoutSettings")
			.field("systemsDistance", &GuidoLayoutSettings::systemsDistance)
			.field("systemsDistribution", &GuidoLayoutSettings::systemsDistribution)
			.field("systemsDistribLimit", &GuidoLayoutSettings::systemsDistribLimit)
			.field("force", &GuidoLayoutSettings::force)
			.field("spring", &GuidoLayoutSettings::spring)
			.field("neighborhoodSpacing", &GuidoLayoutSettings::neighborhoodSpacing)
			.field("optimalPageFill", &GuidoLayoutSettings::optimalPageFill)
			.field("resizePage2Music", &GuidoLayoutSettings::resizePage2Music)
			.field("proportionalRenderingForceMultiplicator", &GuidoLayoutSettings::proportionalRenderingForceMultiplicator);

	emscripten::value_object<GuidoDate>("GuidoDate")
			.field("num", &GuidoDate::num)
			.field("denom", &GuidoDate::denom);

	emscripten::value_object<GuidoPageFormat>("GuidoPageFormat")
			.field("width", &GuidoPageFormat::width)
			.field("height", &GuidoPageFormat::height)
			.field("marginleft", &GuidoPageFormat::marginleft)
			.field("margintop", &GuidoPageFormat::margintop)
			.field("marginright", &GuidoPageFormat::marginright)
			.field("marginbottom", &GuidoPageFormat::marginbottom);

	emscripten::value_object<ParserError>("ParserError")
			.field("line", &ParserError::line)
			.field("col", &ParserError::col)
			.field("msg", &ParserError::msg);

	emscripten::value_object<GuidoVersion>("GuidoVersion")
			.field("major", &GuidoVersion::major)
			.field("minor", &GuidoVersion::minor)
			.field("sub", &GuidoVersion::sub)
			.field("str", &GuidoVersion::str);

	emscripten::enum_<GuidoErrCode>("GuidoErrCode")
			.value("guidoNoErr", GuidoErrCode::guidoNoErr)
			.value("guidoErrParse", guidoErrParse)
			.value("guidoErrMemory", GuidoErrCode::guidoErrMemory)
			.value("guidoErrFileAccess", GuidoErrCode::guidoErrFileAccess)
			.value("guidoErrUserCancel", GuidoErrCode::guidoErrUserCancel)
			.value("guidoErrNoMusicFont", GuidoErrCode::guidoErrNoMusicFont)
			.value("guidoErrNoTextFont", GuidoErrCode::guidoErrNoTextFont)
			.value("guidoErrBadParameter", GuidoErrCode::guidoErrBadParameter)
			.value("guidoErrInvalidHandle", GuidoErrCode::guidoErrInvalidHandle)
			.value("guidoErrNotInitialized", GuidoErrCode::guidoErrNotInitialized)
			.value("guidoErrActionFailed", GuidoErrCode::guidoErrActionFailed);
}

EMSCRIPTEN_BINDINGS(Adapter) {
	// Binding C++ class adapter for guidoEngine
	emscripten::class_<GuidoEngineAdapter>("GuidoEngineAdapter")
			//.smart_ptr_constructor("GuidoEngineAdapter",&std::make_shared<GuidoEngineAdapter>)
			.constructor<>()
			.function("init", select_overload<GuidoErrCode(void)>(&GuidoEngineAdapter::init))
			.function("shutdown", &GuidoEngineAdapter::shutdown)
			.function("ar2gr", select_overload<NodeGR*(NodeAR*)>(&GuidoEngineAdapter::ar2gr), allow_raw_pointers())
			.function("ar2grSettings", select_overload<NodeGR*(NodeAR*, const GuidoLayoutSettings &)>(&GuidoEngineAdapter::ar2gr), allow_raw_pointers())
			.function("updateGR", select_overload<GuidoErrCode(NodeGR*)>(&GuidoEngineAdapter::updateGR), allow_raw_pointers())
			.function("updateGRSettings", select_overload<GuidoErrCode(NodeGR*, const GuidoLayoutSettings &)>(&GuidoEngineAdapter::updateGR), allow_raw_pointers())
			.function("freeAR", &GuidoEngineAdapter::freeAR, allow_raw_pointers())
			.function("freeGR", &GuidoEngineAdapter::freeGR, allow_raw_pointers())
			.function("getErrorString", &GuidoEngineAdapter::getErrorString)
			.function("getDefaultLayoutSettings", &GuidoEngineAdapter::getDefaultLayoutSettings)
			.function("countVoices", &GuidoEngineAdapter::countVoices, allow_raw_pointers())
			.function("getPageCount", &GuidoEngineAdapter::getPageCount, allow_raw_pointers())
			.function("getSystemCount", &GuidoEngineAdapter::getSystemCount, allow_raw_pointers())
			.function("duration", &GuidoEngineAdapter::duration, allow_raw_pointers())
			.function("findEventPage", &GuidoEngineAdapter::findEventPage, allow_raw_pointers())
			.function("findPageAt", &GuidoEngineAdapter::findPageAt, allow_raw_pointers())
			.function("getPageDate", &GuidoEngineAdapter::getPageDate, allow_raw_pointers())
			.function("svgExport", select_overload<string (NodeGR*, int)>(&GuidoEngineAdapter::svgExport), allow_raw_pointers())
			.function("abstractExport", select_overload<string (NodeGR*, int)>(&GuidoEngineAdapter::abstractExport), allow_raw_pointers())
			.function("binaryExport", select_overload<string (NodeGR*, int)>(&GuidoEngineAdapter::binaryExport), allow_raw_pointers())
			.function("javascriptExport", &GuidoEngineAdapter::javascriptExport, allow_raw_pointers())
			.function("setDrawBoundingBoxes", &GuidoEngineAdapter::setDrawBoundingBoxes)
			.function("getDrawBoundingBoxes", &GuidoEngineAdapter::getDrawBoundingBoxes)
			.function("getPageFormat", &GuidoEngineAdapter::getPageFormat, allow_raw_pointers())
			.function("setDefaultPageFormat", &GuidoEngineAdapter::setDefaultPageFormat)
			.function("getDefaultPageFormat", &GuidoEngineAdapter::getDefaultPageFormat)
			.function("unit2CM", &GuidoEngineAdapter::unit2CM)
			.function("cm2Unit", &GuidoEngineAdapter::cm2Unit)
			.function("unit2Inches", &GuidoEngineAdapter::unit2Inches)
			.function("inches2Unit", &GuidoEngineAdapter::inches2Unit)
			.function("resizePageToMusic", &GuidoEngineAdapter::resizePageToMusic, allow_raw_pointers())
			.function("getVersion", &GuidoEngineAdapter::getVersion)
			.function("checkVersionNums", &GuidoEngineAdapter::checkVersionNums)
			.function("getLineSpace", &GuidoEngineAdapter::getLineSpace)
			.function("markVoice", &GuidoEngineAdapter::markVoice, allow_raw_pointers())
			.function("openParser", &GuidoEngineAdapter::openParser, allow_raw_pointers())
			.function("closeParser", &GuidoEngineAdapter::closeParser, allow_raw_pointers())
			.function("file2AR", &GuidoEngineAdapter::file2AR, allow_raw_pointers())
			.function("string2AR", &GuidoEngineAdapter::string2AR, allow_raw_pointers())
			.function("getStream", &GuidoEngineAdapter::getStream, allow_raw_pointers())
			.function("stream2AR", &GuidoEngineAdapter::stream2AR, allow_raw_pointers())
			.function("parserGetErrorCode", &GuidoEngineAdapter::parserGetErrorCode, allow_raw_pointers())
			.function("openStream", &GuidoEngineAdapter::openStream, allow_raw_pointers())
			.function("closeStream", &GuidoEngineAdapter::closeStream, allow_raw_pointers())
			.function("writeStream", &GuidoEngineAdapter::writeStream, allow_raw_pointers())
			.function("resetStream", &GuidoEngineAdapter::resetStream, allow_raw_pointers());

	// Black box object, just for passing argument pointer in method
	emscripten::class_<GuidoParser>("GuidoParser");
	emscripten::class_<NodeAR>("NodeAR");
	emscripten::class_<NodeGR>("NodeGR");
	emscripten::class_<GuidoStream>("GuidoStream");
}
