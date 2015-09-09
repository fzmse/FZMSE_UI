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
                     ManagedObjectRelativeElement * oldItem,
                     std::string gmcMocOperation)
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

    this->gmcMocOperation = gmcMocOperation;
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

std::string GMCAction::getGmcMocOperation()
{
    return this->gmcMocOperation;
}


std::string GMCAction::getDistName()
{
    return this->distName;
}



void GMCAction::buildDistNameFromBase(std::string distNameBase)
{
    distName = "";
    if ( this->actionType == Add && this->actionScope == ManagedObject )
    {
        // calculate addon
        PDDBManagedObject * mocPDDB = (PDDBManagedObject*)this->compareResult.getSecondElement();
        string className = mocPDDB->getClassName();
        string resultString = "";
        PDDBManagedObjectParameter * par = mocPDDB->getMocIdParameter();
        if ( par != NULL )
        {
            string idString = ((PDDBSimpleTypeValue*) par->getPDDBValue() )->getEvaluatedValueForAdd();

            // add base
            if ( distNameBase == "")
            {
                resultString += className;
                resultString += "-";
                resultString += idString;
            }
            else
            {
                resultString += distNameBase;
                resultString += "/";
                resultString += className;
                resultString += "-";
                resultString += idString;
            }
            distName = resultString;
        }
    }
}
