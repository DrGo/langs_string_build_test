local args = { ... } -- get arguments from command line
local src_dir = "src"
local out_dir = "out"
local exe_dir = "../exe" 

local function get_exe_name(lang)
	local exe_ext = ".exe"
	if os.getenv('OS') ~= "Windows" then
		exe_ext = ""
	end
	return string.format("%s/%s_strbld%s", exe_dir, lang, exe_ext)
end

local function echo(p)
	print(p)
end

local build_cmds = {
	c = [[gcc -O3 -march=native -funroll-loops -Ofast c_strbld.c -o ]]..get_exe_name("c"),

}

local function exec(cmd)
	echo("exec: " .. cmd)
	local p = assert(io.popen(cmd))
	local res = p:read("*all")
	p:close()
	return res
end

local function build(lang)
	local t
	local cmd = build_cmds[lang]
	if cmd then
		t = exec(cmd)
	else
		echo("echo: " .. lang .. " is not supported")
	end
	return t
end
local function build_all()
	for lang in pairs(build_cmds) do
		build(lang)
	end
end 

local function run(cmd)
end


for i in ipairs(args) do
	if args[i] == "run" then
		echo("echo: " .. args[i + 1])
	else
		if args[i] == "build" then
			build_all()
		end
	end
end
