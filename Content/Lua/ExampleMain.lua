
ExampleMain = ClassPanel("ExampleMain", '/Game/Example/ExampleMain/ExampleMainUI.ExampleMainUI')

function ExampleMain:construct()

    self.Button_GC = self.widget:FindWidget("Button_GC")
    self.Button_GC.OnClicked:Add(function() self:OnGC() end)

    self.Button_ReuseList = self.widget:FindWidget("Button_ReuseList")
    self.Button_ReuseList.OnClicked:Add(function() self:OnClick_Button_ReuseList() end)

    self.Button_ReusePage = self.widget:FindWidget("Button_ReusePage")
    self.Button_ReusePage.OnClicked:Add(function() self:OnClick_Button_ReusePage() end)

end

function ExampleMain:destruct()
    self.Button_GC.OnClicked:Clear()
    self.Button_ReuseList.OnClicked:Clear()
    self.Button_ReusePage.OnClicked:Clear()
end

function ExampleMain:Trace()
    log("ExampleMain:Trace "..tostring(self.widget))
end

function ExampleMain:OnGC()
    collectgarbage("collect")
    SHC.GC()
    SHC.TraceAllObject()
    --DumpSingleObjRef(self.widget)
    --MRI.m_cMethods.DumpMemorySnapshot("", "All", -1)
    --log_tree("debug.getregistry().SLUA_PTR_USERTABLE_MAPPING", debug.getregistry().SLUA_PTR_USERTABLE_MAPPING)
end

function ExampleMain:OnClick_Button_ReuseList()
    local w = ExampleReuseList.new()
    w:Open()
end

function ExampleMain:OnClick_Button_ReusePage()
    local w = ExampleReusePage.new()
    w:Open()
end