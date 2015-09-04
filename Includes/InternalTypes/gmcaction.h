#pragma once

#include <vector>
#include <string>

#include "InternalTypes/ManagedObjectRelativeElement.h"
#include "InternalTypes/GMCManagedObject.h"
#include "PDDBManagedObjectCompareResult.h"

namespace InternalTypes
{
    class GMCAction
    {
    public:
        enum ActionType
        {
            Add,
            Modify,
            Remove
        };

        enum ChangeScope
        {
            ManagedObject,
            ManagedObjectParameter,
            ComplexParameter
        };
        GMCAction();
        GMCAction(PDDBManagedObjectCompareResult cmpRes, bool interaction, ManagedObjectRelativeElement * item,
                  ActionType actionType, ChangeScope actionScope, int pddbCompareResultId,
                  std::string helpNote = "",
                  std::vector<GMCManagedObject * > gmcMocsInvolved = std::vector<GMCManagedObject *>(),
                  std::vector<GMCAction> childActions = std::vector<GMCAction>(),
                  ManagedObjectRelativeElement * oldItem = NULL,
                  std::string gmcMocOperation = "create"
                  );

        ~GMCAction();


        void setIncludedInGMC(bool val);


        bool isIncludedInGMC();


        int getPDDBCompareResultId();

        ActionType getActionType();
        ChangeScope getChangeScope();
        std::vector<GMCAction> getChildActions();
        ManagedObjectRelativeElement * getOldItem();
        ManagedObjectRelativeElement * getItem();
        std::vector<GMCManagedObject*> getGmcMocsInvolved();
        PDDBManagedObjectCompareResult getCompareResult();
        PDDBManagedObjectCompareResult compareResult;
        std::string getGmcMocOperation();
        std::string getHelpNote();
        bool isReaderInteractionRequired();

    protected:

        std::vector<GMCManagedObject * > gmcMocsInvolved;
        std::vector<GMCAction> childActions;
        ManagedObjectRelativeElement * oldItem;
        ManagedObjectRelativeElement * item;
        ActionType actionType; ChangeScope actionScope;
        int pddbCompareResultId;

        std::string helpNote;

        bool requiresReaderInteraction;

        std::string gmcMocOperation;

        bool includedInGMC;


    };
}
