/*
 * Project: VizKit
 * Version: 2.3
 
 * Date: 20090823
 * File: VisualAppleScript.cpp
 *
 */

/***************************************************************************

Copyright (c) 2004-2009 Heiko Wichmann (http://www.imagomat.de/vizkit)


This software is provided 'as-is', without any expressed or implied warranty. 
In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; 
   you must not claim that you wrote the original software. 
   If you use this software in a product, an acknowledgment 
   in the product documentation would be appreciated 
   but is not required.

2. Altered source versions must be plainly marked as such, 
   and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.

 ***************************************************************************/

#include "VisualAppleScript.h"
#include "VisualFile.h"
#include "VisualString.h"


using namespace VizKit;


bool VisualAppleScript::executeScriptFile(VisualFile& scriptFile) {

	bool success = false;
	
	VisualString scriptFilePath;
	scriptFile.getFilePath(scriptFilePath);
	const char* const scriptPathCStr = scriptFilePath.getUtf8Representation();
	
	size_t scriptPathCStrLen = strlen(scriptPathCStr);
	if (scriptPathCStrLen > 0) {
		char* script = (char*)malloc(scriptPathCStrLen + strlen("osascript ") + 3);
		sprintf(script, "osascript \"%s\"", scriptPathCStr);
		system(script);
		free(script);
		success = true;
	}
	
	return success;
}

