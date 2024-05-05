local overseer = require("overseer")
overseer.run_template({ name = "make" }, function(task)
	if task then
		overseer.run_action(task, "open float")
	end
end)

-- abro com o nvim e uso <space><Enter>, para executar o make
