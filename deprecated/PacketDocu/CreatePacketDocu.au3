Local $ServerPath = GetServerPath()
Local $GameServer = $ServerPath & "\GameServer\Packets"
Local $LoginServer = $ServerPath & "\LoginServer\Packets"
Local Const $NewLine = @CRLF

DirRemove("wiki", 1)
DirCreate("wiki")

FileWrite("wiki\PacketDoc.wiki", "#summary Documentation of GuildWars Packets")
CreateWikiHeader("= Game Server =")
CreateWikiFiles($GameServer, "From Client")
CreateWikiFiles($GameServer, "To Client")
CreateWikiFiles($GameServer, "From Login Server")
CreateWikiFiles($GameServer, "To Login Server")

CreateWikiHeader("= Login Server =")
CreateWikiFiles($LoginServer, "From Client")
CreateWikiFiles($LoginServer, "To Client")
CreateWikiFiles($LoginServer, "From Game Server")
CreateWikiFiles($LoginServer, "To Game Server")


Func CreateWikiHeader($Header)
	WikiAddLine("PacketDoc", $NewLine & $Header)
EndFunc   ;==>CreateWikiHeader

Func CreateWikiFiles($Path, $Type)
	$Path &= "\" & StringReplace($Type, " ", "") & "\"
	Local $Search = FileFindFirstFile($Path & "*.*")
	If $Search = -1 Then Return

	Local $File

	WikiAddLine("PacketDoc", $NewLine & "== " & $Type & " ==")

	While 1
		$File = FileFindNextFile($Search)
		If @error Then ExitLoop
		CreateWikiPacket($Path, $File)
	WEnd

	FileClose($Search)
EndFunc   ;==>CreateWikiFiles

Func CreateWikiPacket($Path, $Packet)
	If StringInStr($Packet, "_") = 0 Then Return
	$Path &= $Packet
	$Packet = StringTrimRight($Packet, 3)
	CreateWikiStruct($Path, $Packet)

	WikiAddLine("PacketDoc", " * [" & $Packet & "]")
EndFunc   ;==>CreateWikiPacket

Func CreateWikiStruct($Path, $Packet)
	Local $File = FileRead($Path)

	Local $Split = StringSplit($File, "UInt16 Header", 1)
	$Split = StringSplit($Split[2], "public void", 1)
	$Split = $Split[1]

	$Start = StringInStr($Split, "public")
	Local $Block = StringMid($Split, $Start, StringInStr($Split, ";", 0, -1) - $Start)

	$Block = StringStripWS($Block, 4)
	$Block = StringReplace($Block, "public ", "")
	$Block = StringReplace($Block, ";", "")

	$Block = TidyPacket($Block)
	If Not @error Then
		If StringLen(FileRead("wiki\" & $Packet & ".wiki")) < 3 Then ;check for double packets
			FileWrite("wiki\" & $Packet & ".wiki", "== " & $Packet & " ==" & $NewLine & $Block)
		EndIf
	EndIf
EndFunc   ;==>CreateWikiStruct

Func TidyPacket($Block)
	Local $Line = StringSplit($Block, @CRLF)

	If $Line[0] = 0 Then
		SetError(1)
		Return
	EndIf

	Local $Output = "|| *Datatype* || *Field* || *Comment* ||" & $NewLine & "|| *UInt16* || Header || ||"
	Local $NextComment = ""

	For $i = 1 To $Line[0]
		$ThisLine = $Line[$i]

		If StringStripWS($ThisLine, 8) = "" Then
			SetError(1)
			Return
		EndIf

		If StringMid($ThisLine, 1, 1) = "[" Then
			$NextComment = "_" & StringMid($ThisLine, 17, StringLen($ThisLine) - 17) & "_"
		ElseIf StringMid($ThisLine, 1, 1) <> "/" Then
			$ThisLine = FormatLine($ThisLine)
			$Output &= $NewLine & "|| " & $ThisLine & " " & $NextComment & " ||"
			$NextComment = ""
		Else
			$NextComment = ""
		EndIf
	Next

	If StringStripWS($Output, 8) = "" Then
		SetError(1)
	Else
		Return $Output
	EndIf
EndFunc   ;==>TidyPacket

Func FormatLine($Line)
	Local $Space = StringInStr($Line, " ")
	Local $NewLine = "*" & StringLeft($Line, $Space - 1) & "* || " & StringRight($Line, StringLen($Line) - $Space)

	If StringInStr($NewLine, "//") > 0 Then
		$NewLine = StringReplace($NewLine, "//", "|| _")
		$NewLine &= "_"
	Else
		$NewLine &= " ||"
	EndIf

	Return $NewLine
EndFunc   ;==>FormatLine

Func WikiAddLine($File, $Line)
	FileWrite("wiki\" & $File & ".wiki", $NewLine & $Line)
EndFunc   ;==>WikiAddLine

Func GetServerPath()
	Return StringReplace(@ScriptDir, "Utils\PacketDocu", "Server2011\GWLP-R")
EndFunc   ;==>GetServerPath

Func Debug($a)
	ConsoleWrite($a & @CRLF)
EndFunc   ;==>Debug