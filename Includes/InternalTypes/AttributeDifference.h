#pragma once

#include <utility>
#include <vector>
#include "Attribute.h"

namespace InternalTypes
{
	class AttributeDifference : public std::pair<Attribute, Attribute>
	{
	public:
		AttributeDifference(std::pair<Attribute, Attribute> diff);
		AttributeDifference(Attribute a, Attribute b);

		static std::vector<AttributeDifference> getDifferentVectorsOfAttributes(std::vector<Attribute> a, std::vector<Attribute> b);


	protected:
		AttributeDifference();

		std::string key;
		std::string value;
	};
}
