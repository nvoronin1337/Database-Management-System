#include "pch.h"
#include "Table.h"

namespace dbms {

	Table::Table(std::string tableName)
	{
		this->tableName = tableName;
	}
	std::ostream & operator<<(std::ostream & output, const Table * table)
	{
		try {
			std::string tableInfo = "";
			tableInfo.append("\t\t<Table>: [" + table->tableName + "]");
			tableInfo.append(" {");
			for (size_t col = 0; col < table->columnNames.size(); col++) {
				tableInfo.append(table->columnNames[col]);
				if (col != table->columnNames.size() - 1) {
					tableInfo.append(", ");
				}
			}
			tableInfo.append("}\n");
			for (size_t j = 0; j < table->records.size(); j++) {	// loop through records in current table
				tableInfo.append("\t\t\t<RecordID>: [" + std::to_string(table->records[j]->recordID) + "]\n");
				// loop through the table columns and corresponding values
				for (size_t h = 0; h < table->columnNames.size(); h++) {
					tableInfo.append("\t\t\t\t<data>(" + table->columnNames[h] + "): [" + table->records[j]->dataFields[h] + "]\n");
				}
			}

			output << tableInfo;
			return output;
		}
		catch (const std::exception e) {

		}
		return output << "";
	}
}

