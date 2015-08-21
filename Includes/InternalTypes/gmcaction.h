#pragma once

#include <vector>
#include <string>

#include "InternalTypes/ManagedObjectRelativeElement.h"
#include "InternalTypes/GMCManagedObject.h"

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
        };

        GMCAction(bool interaction, ManagedObjectRelativeElement * item,
                  ActionType actionType, ChangeScope actionScope, int pddbCompareResultId,
                  std::string helpNote = "",
                  std::vector<GMCManagedObject * > gmcMocsInvolved = std::vector<GMCManagedObject *>(),
                  std::vector<GMCAction> childActions = std::vector<GMCAction>());

        ~GMCAction();


        int getPDDBCompareResultId();

        ActionType getActionType();
        ChangeScope getChangeScope();
        std::vector<GMCAction> getChildActions();
        ManagedObjectRelativeElement * getItem();

        std::string getHelpNote();
        bool isReaderInteractionRequired();

    protected:
        GMCAction();
        std::vector<GMCManagedObject * > gmcMocsInvolved;
        std::vector<GMCAction> childActions;
        ManagedObjectRelativeElement * item;
        ActionType actionType; ChangeScope actionScope;
        int pddbCompareResultId;

        std::string helpNote;

        bool requiresReaderInteraction;


    };
}
