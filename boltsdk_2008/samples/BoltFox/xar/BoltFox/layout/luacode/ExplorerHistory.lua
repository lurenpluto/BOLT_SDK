function Initialize(self)
	self.RecentUrls = {}
	self.MaxRecentUrlCount = 10
	self.RecentSearchKeywords = {}
	self.MaxRecentSearchKeywordCount = 10
end

local function pushFrontString(strTable, maxStrCnt, str)
	local cnt = table.maxn(strTable)
	if cnt == 0 then
		strTable[1] = str
		return 
	end
	for i=1,cnt do
		if strTable[i] == str then
			if i >= 2 then
				for j=i,2,-1 do
					strTable[i] = strTable[i - 1]
				end
			end
			strTable[1] = str
			return 
		end
	end
	cnt = cnt + 1
	if cnt <= maxStrCnt then
		strTable[cnt] = nil
	else
		cnt = maxStrCnt
	end
	for i=cnt,2,-1 do
		strTable[i] = strTable[i - 1]
	end
	strTable[1] = str
end

function PushFrontRecentUrl(self,url)
	pushFrontString(self.RecentUrls, self.MaxRecentUrlCount, url)
end

function GetRecentUrls(self)
	return self.RecentUrls
end

function PushFrontRecentSearchKeyword(self, keyword)
	pushFrontString(self.RecentSearchKeywords, self.MaxRecentSearchKeywordCount, keyword)
end

function GetRecentSearchKeywords(self)
	return self.RecentSearchKeywords
end

function Save(self)
	local historyFile = XLGetObject("BoltFox.HistoryFile")
	historyFile:Save(self.RecentUrls,self.RecentSearchKeywords)
end

function Load(self)
	local historyFile = XLGetObject("BoltFox.HistoryFile")
	self.RecentUrls, self.RecentSearchKeywords = historyFile:Load()
end

function RegisterObject(self)
	local explorerHistory = {}
	explorerHistory.Initialize = Initialize
	explorerHistory.PushFrontRecentUrl = PushFrontRecentUrl
	explorerHistory.GetRecentUrls = GetRecentUrls
	explorerHistory.Save = Save
	explorerHistory.Load = Load
	XLSetGlobal("BoltFox.History",explorerHistory)
	
	explorerHistory:Initialize()
	explorerHistory:Load()
end