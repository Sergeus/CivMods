-- ReqTradeRoutesForPeace
-- Author: S3rgeus
-- DateCreated: 8/11/2013 12:55:35 AM
--------------------------------------------------------------
function ReqTradeRoutesTraitCheck(iTeamOne, iTeamTwo)
	local pTeamOne = Teams[iTeamOne]
	local pTeamTwo = Teams[iTeamTwo]

	-- TODO implementation

	return true
end
GameEvents.CanDeclareWar.Add(ReqTradeRoutesTraitCheck)