#pragma once
#ifndef RECORD
#define RECORD

//#include <xstring>
#include <string>
#include <vector>
#include <fstream>
//#include <atlstr.h>

namespace dbms {
	
	class Record {
	private:		             
	public:
		int recordID;
		
		std::vector<std::string> dataFields;

		//virtual std::string getType() = 0;	// making class abstract

		Record() = default;
		~Record() = default;

		friend std::ostream &operator<<(std::ostream &output, const Record *record);
	};
}
#endif // RECORD



