main();

// 添加系统环境变量
function BuildSystemEnv(strSDKFolder)
{
	// WScript.echo(strSDKFolder)
	var WSShell = WScript.CreateObject("WScript.Shell");
	
	// 此处设置变量
	try
	{
		WSShell.Environment("system").Item("BOLTSDK")=strSDKFolder
	}
	catch(e)
	{
		WScript.Echo("ERROR: Set BOLTSDK Environment variable failed!");
		return false;
	}
	
	
	// WScript.echo("env set ok")
	return true;
}

function main()
{
	// 简单解析命令行
	var bElevated = false;
	var Args = WScript.Arguments;
	for(var i = 0; i < Args.length; i++)
	{
		if(Args(i) == "/elevated")
		{
			bElevated = true;
		}
	}
	

	// 检查是否需要提升权限
	var Shell = WScript.CreateObject("Shell.Application");
	if(!bElevated && Shell.IsRestricted("System", "EnableLUA"))
	{
		// Check that the script is being run interactively.
		if(!WScript.Interactive)
		{
			WScript.Echo("ERROR: Elevation required.");
			return;
		}
		
		// 尝试重新以管理员权限运行
		var strParams = "\"" + WScript.ScriptFullName + "\"";
		strParams += " /elevated";
		Shell.ShellExecute(WScript.FullName, strParams, null, "RunAs");
		return;
	}
	
	var FileSys = WScript.CreateObject("Scripting.FileSystemObject");
	
	// 获取脚本所在文件夹
	var strSDKFolder = FileSys.GetParentFolderName(WScript.ScriptFullName);
	if(strSDKFolder == null || strSDKFolder == "")
	{
		strSDKFolder = ".";
	}
	
	// 设置sdk的系统环境变量
	BuildSystemEnv(strSDKFolder);
	 
	// 设置vs的环境变量
	AddVcDir()
	
	// 在开始菜单添加
	CreateStartMenu()
	
	//CopyXLUEToTools()
	
	//编译samples xar
	CompileSamples()
	
	//设置默认pr
	SetProductRelease()
	
	WScript.echo("安装成功!!!!")
	return;
}

// 返回文件内所有行数组
function GetAllLineFromeFile(path)
{
	var fso = new ActiveXObject("Scripting.FileSystemObject");
	if(fso.FileExists(path))
	{
		var f = fso.opentextfile(path, 1, false, -2)
		var lineArray = new Array()
		while(!f.AtEndOfStream)
		{
			var line = f.readline()
			lineArray.push(line)
		}
		f.close()
		return lineArray
	}
}

//返回逻辑结构
function Analyse(lineArray)
{	
	var data = {}
	var lastkey
	
	for (var i = 0; i < lineArray.length; i++)
	{
		if (lineArray[i].charAt(0) == '[')
		{
			lastkey = lineArray[i]
			data[lastkey] = {}
		}
		else
		{
			if(data[lastkey])
			{
				var matches = lineArray[i].match("(.*)=(.*)")
				data[lastkey][matches[1]] = matches[2].split(";")
			}
		}
	}
	
	return data
}

function ElementInArray(array, element)
{
	for(var i = 0; i < array.length; i++)
	{
		if (element == array[i])
			return true
	}
	return false
}

//添加 include和lib
function OperatorData(data)
{
	// WScript.echo("OperatorData")

	var win32 = "[VC\\VC_OBJECTS_PLATFORM_INFO\\Win32\\Directories]"
	var include = "Include Dirs"
	var lib = "Library Dirs"
	var appendInclude = "$(BOLTSDK)\\include";
	var appendLib = "$(BOLTSDK)\\lib"
	
	if(data[win32] && data[win32][include] && !ElementInArray(data[win32][include], appendInclude))
	{
		// WScript.echo("find and append include")
		data[win32][include].push(appendInclude)
	}
	
	if(data[win32] && data[win32][lib] && !ElementInArray(data[win32][lib], appendLib))
	{
		// WScript.echo("find and append lib")
		data[win32][lib].push(appendLib)
	}
}

function TraverseData(data)
{
	WScript.Echo("TraverseData")
	for (k in data)
	{
		WScript.Echo(k)
		for(var v in data[k])
		{
			WScript.echo(v + "---------" + data[k][v].join(";"))
		}
	}
}

function GetLinesFromData(data)
{
	var lines = new Array()
	for (k in data)
	{
		lines.push(k)
		for(var v in data[k])
		{
			lines.push(v + "=" + data[k][v].join(";"))
		}
	}
	return lines
}

function SaveLineToFile(path, lines)
{
	var fso = new ActiveXObject("Scripting.FileSystemObject");
	var f = fso.opentextfile(path, 2, false, -2)
	
	for(var i = 0; i < lines.length; i++)
	{
		f.WriteLine(lines[i])
	}
	
	f.close()
}

function ProcessConfigFile(path)
{
	var lineArray = GetAllLineFromeFile(path)
	if(!lineArray)
	{
		// WScript.echo("GetAllLineFromeFile fail " + path)
		return
	}
	
	var data = Analyse(lineArray)
	if(!data)
	{
		WScript.echo("Analyse fail")
		return
	}
		
	// TraverseData(data)
	OperatorData(data)
	// TraverseData(data)
	
	var lines = GetLinesFromData(data)
	if(!lines)
	{
		// WScript.echo("GetLinesFromData fail")
		return
	}
		
	SaveLineToFile(path, lines)
}

function AddVcDir()
{
	var vc71path
	var vc80path
	var vc90path
	var WshShell = WScript.CreateObject("WScript.Shell");
	var localappdata = WshShell.ExpandEnvironmentStrings("%localappdata%");

	if (localappdata == "%localappdata%")
	{
		var user = WshShell.ExpandEnvironmentStrings("%userprofile%");
		vc71path = user + "\\Local Settings\\Application Data\\Microsoft\\VisualStudio\\7.1\\VCComponents.dat"
		vc80path = user + "\\Local Settings\\Application Data\\Microsoft\\VisualStudio\\8.0\\VCComponents.dat"
		vc90path = user + "\\Local Settings\\Application Data\\Microsoft\\VisualStudio\\9.0\\VCComponents.dat"
	}
	else
	{
		vc71path = localappdata + "\\Microsoft\\VisualStudio\\7.1\\VCComponents.dat"
		vc80path = localappdata + "\\Microsoft\\VisualStudio\\8.0\\VCComponents.dat"
		vc90path = localappdata + "\\Microsoft\\VisualStudio\\9.0\\VCComponents.dat"
	}

	var fso = new ActiveXObject("Scripting.FileSystemObject");
	if(fso.FileExists(vc71path))
	{
		ProcessConfigFile(vc71path)
	}
	if(fso.FileExists(vc80path))
	{
		ProcessConfigFile(vc80path)
	}
	if(fso.FileExists(vc90path))
	{
		ProcessConfigFile(vc90path)
	}
}

// 第三个参数表示描述,第四个参数表示icon的位置 "notepad.exe, 0"
function CreateStartMenuItem(linkName, target)
{
	var WshShell = WScript.CreateObject("WScript.Shell"); 
	var startMenu = WshShell.SpecialFolders("StartMenu"); 

	var fso = new ActiveXObject("Scripting.FileSystemObject");
	var linkPath = fso.BuildPath(startMenu, linkName)
	var parentFolder = fso.GetParentFolderName(linkPath)
	if (!fso.FolderExists(parentFolder))
	{
		fso.CreateFolder(parentFolder)
	}

	var itemLink = WshShell.CreateShortcut(linkPath);
	itemLink.TargetPath = target;
	if (arguments[2])
		itemLink.Description = arguments[2]
	if (arguments[3])
		itemLink.IconLocation = arguments[3]
		
	itemLink.WorkingDirectory = fso.GetParentFolderName(target)
	itemLink.Save();
}

function CreateStartMenu()
{
	var fso = new ActiveXObject("Scripting.FileSystemObject");
	var sdkFolder = fso.GetParentFolderName(WScript.ScriptFullName)

	var devFolder = fso.BuildPath(sdkFolder, "convert_to_dev.bat")
	var prFolder = fso.BuildPath(sdkFolder, "convert_to_pr.bat")
	var copyDev = fso.BuildPath(sdkFolder, "copydevto.js")
	var copyPr = fso.BuildPath(sdkFolder, "copyprto.js")

	CreateStartMenuItem("BoltSDK\\打开sdk目录.lnk", sdkFolder)
	CreateStartMenuItem("BoltSDK\\切换到开发版.lnk", devFolder)
	CreateStartMenuItem("BoltSDK\\切换到正式版.lnk", prFolder)
	CreateStartMenuItem("BoltSDK\\拷贝开发版到.lnk", copyDev)
	CreateStartMenuItem("BoltSDK\\拷贝正式版到.lnk", copyPr)
}

function CompileSamples()
{
	var WshShell = WScript.CreateObject("WScript.Shell"); 
	var fso = WScript.CreateObject("Scripting.FileSystemObject");
	
	// 获取脚本所在文件夹
	var strSDKFolder = fso.GetParentFolderName(WScript.ScriptFullName);
	var xluecl = fso.BuildPath(strSDKFolder, "tools\\xluecl.exe")
	xluecl = " \"" + xluecl + "\""
	
	//BoltFox
	WshShell.run(xluecl + " -xar " + fso.BuildPath(strSDKFolder, "samples\\BoltFox\\xar\\BoltFox"), 0)
	
	//MineSweeper
	WshShell.run(xluecl + " -xar " + fso.BuildPath(strSDKFolder, "samples\\MineSweeper\\xar\\MineSweeper"), 0)
	
	//HelloBolt
	WshShell.run(xluecl + " -xar " + fso.BuildPath(strSDKFolder, "samples\\HelloBolt\\xar\\HelloBolt1"), 0)
	WshShell.run(xluecl + " -xar " + fso.BuildPath(strSDKFolder, "samples\\HelloBolt\\xar\\HelloBolt2"), 0)
	WshShell.run(xluecl + " -xar " + fso.BuildPath(strSDKFolder, "samples\\HelloBolt\\xar\\HelloBolt3"), 0)
	WshShell.run(xluecl + " -xar " + fso.BuildPath(strSDKFolder, "samples\\HelloBolt\\xar\\HelloBolt4"), 0)
	WshShell.run(xluecl + " -xar " + fso.BuildPath(strSDKFolder, "samples\\HelloBolt\\xar\\HelloBolt5"), 0)
	WshShell.run(xluecl + " -xar " + fso.BuildPath(strSDKFolder, "samples\\HelloBolt\\xar\\HelloBolt6"), 0)
	WshShell.run(xluecl + " -xar " + fso.BuildPath(strSDKFolder, "samples\\HelloBolt\\xar\\HelloBolt7"), 0)
	
	//Wizard
	WshShell.run(xluecl + " -xar " + fso.BuildPath(strSDKFolder, "samples\\Wizard\\xar\\BaseUI"), 0)
	WshShell.run(xluecl + " -xar " + fso.BuildPath(strSDKFolder, "samples\\Wizard\\xar\\wizard"), 0)
}

function SetProductRelease()
{
	var fso = new ActiveXObject("Scripting.FileSystemObject");
	var sdkFolder = fso.GetParentFolderName(WScript.ScriptFullName)
	var src = fso.BuildPath(sdkFolder, "dev\\pr\\bin\\*.dll")
	var dest = fso.BuildPath(sdkFolder, "bin")
	fso.CopyFile(src, dest)
	
	var src = fso.BuildPath(sdkFolder, "dev\\pr\\bin\\*.exe")
	fso.CopyFile(src, dest)
	
	var src = fso.BuildPath(sdkFolder, "dev\\pr\\lib\\*.lib")
	var dest = fso.BuildPath(sdkFolder, "lib")
	
	fso.CreateFolder(dest)
	fso.CopyFile(src, dest)
}

function CopyXLUEToTools()
{
	var fso = new ActiveXObject("Scripting.FileSystemObject");
	var sdkFolder = fso.GetParentFolderName(WScript.ScriptFullName)
	var src = fso.BuildPath(sdkFolder, "bin\\*.dll")
	var dest = fso.BuildPath(sdkFolder, "tools")
	fso.CopyFile(src, dest)
	
	var src = fso.BuildPath(sdkFolder, "dev\\pr\\bin\\*.dll")
	fso.CopyFile(src, dest)
}