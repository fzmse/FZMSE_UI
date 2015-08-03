
#pragma once

#include <string>
#include <utility>

namespace InternalTypes
{
	class Attribute : std::pair<std::string, std::string>
	{
	public:
		Attribute();
		Attribute ( std::string key, std::string value );
		Attribute ( std::pair<std::string, std::string> );

		std::string getKey();
		std::string getValue();

		void setKey( std::string k );
		void setValue( std::string v );

		bool operator == (Attribute & b);

	protected:

		std::string key;
		std::string value;
	};
}
