Local Const $Serverpath = "yourdirblabla\GWLP\Server2011\GWLP-R\" ;change this line first!

DumpPacketsStructure("GStoC_Structure.ini", "GameServer\Packets\ToClient\")
DumpPacketsStructure("LStoC_Structure.ini", "LoginServer\Packets\ToClient\")
DumpGenericValues("GenericValues.ini")

Func DumpGenericValues($IniFile)
	Local $Path = $Serverpath & "GameServer\Enums\GenericValues.cs"
	Local $File = FileOpen($Path, 0)
	Local $Line, $Split, $Number

	If $File = -1 Then Exit

	FileDelete($IniFile)

	While 1
		$Line = FileReadLine($File)
		If @error = -1 Then ExitLoop
		If StringInStr($Line, "//") = 0 And StringInStr($Line, "=") > 0 Then
			$Split = StringSplit(StringStripWS($Line, 8), "=")
			$Number = StringReplace($Split[2], ",", "")
			IniWrite($IniFile, "GenericValues", $Number, $Split[1] & " (" & $Number & ")")
		EndIf
	WEnd

	FileClose($File)
EndFunc   ;==>DumpGenericValues

Func DumpPacketsStructure($IniFile, $Dir)
	Local $Path = $Serverpath & $Dir
	Local $Search = FileFindFirstFile($Path & "*.cs")

	FileDelete($IniFile)

	If $Search <> -1 Then
		Local $File

		While 1
			$File = FileFindNextFile($Search)
			If @error Then ExitLoop
			DumpPacketStructure($IniFile, $Path & $File)
		WEnd
	Else
		MsgBox(0, "Error", "No packet sources found! please check your Filepaths.")
	EndIf

	FileClose($Search)
EndFunc   ;==>DumpPacketsStructure

Func DumpPacketStructure($IniFile, $Path)
	Local $File = FileRead($Path)
	Local $Split = StringSplit($File, "IPacketTemplate", 1)

	$Name = ExtractName($Split[1])
	$Struct = ExtractStruct($Split[2])
	$Header = @extended

	IniWrite($IniFile, "Templates", $Header, $Name & ";" & $Struct)
EndFunc   ;==>DumpPacketStructure

Func ExtractName($Block)
	Local $Split = StringSplit($Block, "_")
	If $Split[0] < 2 Then Return "Undocumented"
	$Split = StringSplit($Split[2], " ")
	Return $Split[1]
EndFunc   ;==>ExtractName

Func ExtractStruct($Block)
	Local $Split = StringSplit($Block, "public void", 1)
	$Split = StringSplit($Split[1], ";")
	Local $Struct = ""

	For $i = 2 To $Split[0]
		$Line = ExtractStructLine($Split[$i])
		If Not @error Then $Struct &= $Line & ";"
	Next

	SetExtended(ExtractHeader($Split[1]))
	Return $Struct
EndFunc   ;==>ExtractStruct

Func ExtractHeader($Block)
	Local $Split = StringSplit($Block, " ")
	Return $Split[$Split[0]]
EndFunc   ;==>ExtractHeader

Func ExtractStructLine($Block)
	Local $Split = StringSplit($Block, "public ", 1)

	If $Split[0] > 1 Then
		If StringInStr($Split[1], "ConstSize = true") Then
			Local $Split2 = StringSplit($Split[1], ")]", 1)
			$Split2 = StringSplit($Split2[1], " ")
			Return StringReplace($Split[2], "[]", "[" & $Split2[$Split2[0]] & "]")
		Else
			Return $Split[2]
		EndIf
	Else
		SetError(1)
	EndIf
EndFunc   ;==>ExtractStructLine