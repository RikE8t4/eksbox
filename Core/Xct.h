#ifndef XCT_H
#define XCT_H

//==================================
//  Xct.h - Xct Database Handler
//==================================

#define XCT "\\Names\\Names.xct"
#define BUFSIZE 256

class Xct{

public:

	void FindMatch(string& pDirName, bool& pIniMatch, int& pEntries);
	void Dump();

};

#endif
