#include "InternalTypes/GMCAction.h"
\
using namespace std;
using namespace tinyxml2;
using namespace InternalTypes;

GMCAction::GMCAction(PDDBManagedObjectCompareResult r, bool interaction, ManagedObjectRelativeElement * item,
                     ActionType actionType, ChangeScope actionScope, int pddbCompareResultId,
                     std::string helpNote,
                     std::vector<GMCManagedObject * > gmcMocsInvolved,
                     std::vector<GMCAction> childActions,
                     ManagedObjectRelativeElement * oldItem)
{
    this->compareResult = r;

    this->requiresReaderInteraction = interaction;
    this->item = item;
    this->actionType = actionType;
    this->actionScope = actionScope;
    this->pddbCompareResultId = pddbCompareResultId;
    this->childActions = childActions;
    this->gmcMocsInvolved = gmcMocsInvolved;
    this->helpNote = helpNote;

    this->oldItem = oldItem;

    this->includedInGMC = !this->requiresReaderInteraction;
}


GMCAction::GMCAction()
{

}

GMCAction::~GMCAction()
{

}

std::vector<GMCManagedObject*> GMCAction::getGmcMocsInvolved()
{
    return this->gmcMocsInvolved;
}

int GMCAction::getPDDBCompareResultId()
{
    return this->pddbCompareResultId;
}

GMCAction::ActionType GMCAction::getActionType()
{
    return this->actionType;
}

GMCAction::ChangeScope GMCAction::getChangeScope()
{
    return this->actionScope;
}

std::vector<GMCAction> GMCAction::getChildActions()
{
    return this->childActions;
}

ManagedObjectRelativeElement * GMCAction::getItem()
{
    return this->item;
}

ManagedObjectRelativeElement * GMCAction::getOldItem()
{
    return this->oldItem;
}

std::string GMCAction::getHelpNote()
{
    return this->helpNote;
}


bool GMCAction::isReaderInteractionRequired()
{
    return this->requiresReaderInteraction;
}

bool GMCAction::isIncludedInGMC()
{
    return this->includedInGMC;
}

void GMCAction::setIncludedInGMC(bool val)
{
    this->includedInGMC = val;
}

PDDBManagedObjectCompareResult GMCAction::getCompareResult()
{
    return this->compareResult;
}

