-- WhiteTowerStatus
-- Author: S3rgeus
-- DateCreated: 3/2/2014 7:34:30 PM
--------------------------------------------------------------

ContextPtr:SetHide(true)

function OnOkClicked()
	UIManager:DequeuePopup(ContextPtr)
end
Controls.OkButton:RegisterCallback(Mouse.eLClick, OnOkClicked)

function OnShowHide()
end
ContextPtr:SetShowHideHandler(OnShowHide)