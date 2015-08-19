#pragma once

#include <memory>

#include "InternalTypes/GMCDocument.h"

static std::string dir = "D:/Projects/FZMSE/FZMSE/";

std::shared_ptr<InternalTypes::GMCDocument> gmc_1506_02 = std::make_shared<InternalTypes::GMCDocument>(dir + "UT/TestFiles/GMC/gmc150602.xml");
