

function GetSourcePath(isDev)
{
	var fso = new ActiveXObject("Scripting.FileSystemObject");
	var sdkFolder = fso.GetParentFolderName(WScript.ScriptFullName)
	if (isDev)
	{
		var devBinFolder = fso.BuildPath(sdkFolder, "dev\\dev\\bin")
		return [devBinFolder]
	}
	else
	{
		var prBinFolder = fso.BuildPath(sdkFolder, "dev\\pr\\bin")
		return [prBinFolder]
	}
}



function CopyDevTo(isDev)
{

	var objShell = new ActiveXObject("Shell.Application");
	var ssfBIF_EDITBOX = 16


	var objFolder = objShell.BrowseForFolder(0, "选择目标目录", 0);
	if (objFolder)
	{
		var fso = new ActiveXObject("Scripting.FileSystemObject");
		var destFolder
		
		try
		{
			destFolder = objFolder.Items().Item().Path
			
		}
		catch(e)
		{
			WScript.echo(e)
		}
		
		if (!destFolder)
		{
			WScript.echo("获取目标目录失败!")
			return
		}
		var sdkFolder = fso.GetParentFolderName(WScript.ScriptFullName)
			
		var dir = GetSourcePath(isDev)
		// WScript.echo(dir[0])
		// WScript.echo(dir[1])
		
		var fso = new ActiveXObject("Scripting.FileSystemObject");
		
		if(!fso.FolderExists(dir[0]))
		{
			WScript.echo(dir[0] + " not exist")
		}

		fso.CopyFolder(dir[0], destFolder, true)
	}
}


CopyDevTo(true)