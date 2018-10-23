
--Create an class.
function class(classname, url)
    local cls = {}
    cls.__classname = classname
    cls.ctor        = function() end
    cls.binding     = function() end
    function cls.new(...)
        local obj = {}
        setmetatable(obj, cls)
        cls.__index = cls
        --obj.__class = cls
        obj.widget = slua.loadUI(url)
        obj.widget:SetLuaTable(obj)
        obj:ctor(...)
        return obj
    end
    function cls:del()
        if self.widget then
            self.widget:SetLuaTable(nil)
            self.widget:RemoveFromViewport()
            self.widget = nil
            for k,v in pairs(self) do
                --log(""..type(v).." "..tostring(v))
                if type(v) == "userdata" then
                    self[k] = nil
                end
            end
        end
    end
    function cls.bind(w,...)
        local obj = {}
        setmetatable(obj, cls)
        cls.__index = cls
        obj.widget = w
        obj.widget:SetLuaTable(obj)
        obj:binding(...)
        return obj
    end
    function cls:unbind()
        if self.widget then
            self.widget:SetLuaTable(nil)
            self.widget = nil
            for k,v in pairs(self) do
                --log(""..type(v).." "..tostring(v))
                if type(v) == "userdata" then
                    self[k] = nil
                end
            end
        end
    end
    return cls
end