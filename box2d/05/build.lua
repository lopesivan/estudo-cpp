local overseer = require("overseer")
overseer.run_template({ name = "make" }, function(task)
    if task then
        overseer.run_action(task, "open float")
    end
end)
