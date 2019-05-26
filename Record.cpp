#include "pch.h"
#include "Record.h"

std::ostream & dbms::operator<<(std::ostream & output, const Record * record)
{
	try {
		std::string recordInfo = "";

		recordInfo.append("\t\t\t<RecordID>: [" + std::to_string(record->recordID) + "]\n");
		// loop through the table columns and corresponding values
		for (size_t h = 0; h < record->dataFields.size(); h++) {
			recordInfo.append("\t\t\t\t<data>(" + record->dataFields[h] + "): [" + record->dataFields[h] + "]\n");
		}

		output << recordInfo;
		return output;
	}
	catch (const std::exception e) {

	}
}
