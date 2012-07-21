#include <GuiEdit.au3>
#include <GuiConstantsEx.au3>

;Things you may want to change
Local $DumpAnonymous = False
Local $DumpTime = False
Local $DumpArrays = True
Local $DumpArraysInHex = True
Local $DumpFieldsInHex = True
Local $DumpStringsInHex = False
Local $UseConsole = True

;Things you may not have to change
Local $PacketTemplates = "GStoC_Structure.ini"

;Things you actually shouldnt change
Local $CurrentArraySize = 0
Local $AlreadyRead = 0
Local $Time = ""
Local Const $Spaces = "                                                                         "
Local Const $Dashes = "-------------------------------------------------------------------------"

If $UseConsole Then
	FileDelete("out.txt")
	HotKeySet("{NUMPADDIV}", "ClearDump")
	Local $Gui = GUICreate("Filter", 800, 400)
	Local $Edit = GUICtrlCreateEdit("", 0, 0, 800, 400)
	GUICtrlSetFont($Edit, 9, 400, 0, "Courier New")
	GUISetState()

	While GUIGetMsg() <> $GUI_EVENT_CLOSE
		Sleep(10)
		DumpNewStuff()
	WEnd
	GUIDelete()
Else
	DumpNewStuff()
EndIf

Func ClearDump()
	FileDelete("ByteArrayDump.txt")
EndFunc   ;==>ClearDump

Func DumpNewStuff()
	Local $Dump = GetNewDump()
	If @error Then Return

	Local $Split = StringSplit($Dump, "{")
	Local $Output = ""

	For $i = 2 To $Split[0]
		$Output &= ReadDump($Split[$i])
	Next

	If $UseConsole Then
		_GUICtrlEdit_AppendText($Edit, $Output)
		FileWrite("out.txt", $Output)
	Else
		FileDelete("out.txt")
		FileWrite("out.txt", $Output)
	EndIf
EndFunc   ;==>DumpNewStuff

Func GetNewDump()
	Local $Dump = FileRead("ByteArrayDump.txt")
	Local $NewDump = StringTrimLeft($Dump, $AlreadyRead)
	Local $NewRead = StringLen($Dump)

	If $NewRead = $AlreadyRead Then
		SetError(1)
		Return
	ElseIf $NewRead < $AlreadyRead Then
		If $UseConsole Then GUICtrlSetData($Edit, "")
		$AlreadyRead = 0
		Return $Dump
	Else
		$AlreadyRead = $NewRead
		Return $NewDump
	EndIf
EndFunc   ;==>GetNewDump

Func ReadDump($Block)
	Local $RawData = GetRawData($Block)

	Local $Header = GetShort(StringLeft($RawData, 4))

	Local $Template = IniRead($PacketTemplates, "Templates", $Header, "")
	$RawData = StringTrimLeft($RawData, 4)

	Local $StructMember = StringSplit($Template, ";")

	Local $Name = $StructMember[1]

	;filter most spam
	If StringInStr($Name, "Heartbeat") Or StringInStr($Name, "Ping") Or StringInStr($Name, "Movement") Or StringInStr($Name, "RotateAgent") Then Return ""

	Local $Output = ""
	Local $Member[3]
	Local $RawDataRemainder = $RawData
	Local $ArraySize
	Local $Declaration

	For $i = 2 To $StructMember[0] - 1
		$Member = GetStructMember($StructMember[$i])
		$ArraySize = $Member[1]
		$Declaration = SpaceUp($StructMember[$i]) ;formatting

		If $ArraySize = 0 Then
			If $Member[0] < 5 Then ;SINGLE VALUE FIELDS
				$Out = GetMemberValues($Member, $RawDataRemainder)
				$Value = $Out[0]
				$RawDataRemainder = $Out[1]

				If StringInStr($Member[2], "ArraySize") Or StringInStr($Member[2], "NumStats") Then
					$CurrentArraySize = $Value
				ElseIf StringInStr($Member[2], "ValueID") Then
					$Value = IniRead("GenericValues.ini", "GenericValues", Dec($Value), $Value)
				EndIf

				$Output &= "  " & $Declaration & " = " & $Value & @CRLF
			Else ;STRING VALUES
				$Out = GetString($RawDataRemainder)
				If $DumpStringsInHex Then
					$String = StringTrimRight($RawDataRemainder, StringLen($Out[1]))
				Else
					$String = $Out[0]
				EndIf
				$RawDataRemainder = $Out[1]

				If $DumpAnonymous Then $String = StringLeft($Dashes, StringLen($String))
				$Output &= "  " & $Declaration & " = """ & $String & """" & @CRLF
			EndIf
		Else ;ARRAYS
			$Declaration = StringReplace($Declaration, "[]", "[" & $Member[1] & "]")
			If @extended Then $Declaration = StringTrimRight($Declaration, StringLen($Member[1])) ;formatting

			$Out = GetMemberValues($Member, $RawDataRemainder)
			Local $Values[$ArraySize]
			$Values = $Out[0]
			$RawDataRemainder = $Out[1]
			$ArrayOutput = @CRLF

			If $DumpArrays Then
				For $j = 0 To $ArraySize - 1
					If $DumpArraysInHex Then
						$Value = Hex($Values[$j], 2 * $Member[0])
					Else
						$Value = $Values[$j]
					EndIf

					$ArrayOutput &= "    " & $Member[2] & "[" & $j & "] = " & $Value & @CRLF
				Next
			EndIf

			$Output &= "  " & $Declaration & " = []" & $ArrayOutput
		EndIf
	Next

	If $DumpTime Then $Time = "[" & @HOUR & ":" & @MIN & ":" & @SEC & "] "

	Return $Time & $Name & "(" & $Header & ")" & @CRLF & $Output & @CRLF
EndFunc   ;==>ReadDump

Func SpaceUp($String)
	Return $String & StringLeft($Spaces, 30 - StringLen($String))
EndFunc   ;==>SpaceUp

Func GetString($RawData)
	Local $StringSize = GetShort(StringLeft($RawData, 4))
	Local $RawDataRemainder = StringTrimLeft($RawData, 4)
	Local $wCharStruct = DllStructCreate("wchar")
	Local $String = ""

	For $i = 0 To $StringSize - 1
		$Char = GetShort(StringLeft($RawDataRemainder, 4))
		DllStructSetData($wCharStruct, 1, $Char)
		$String &= DllStructGetData($wCharStruct, 1)
		$RawDataRemainder = StringTrimLeft($RawDataRemainder, 4)
	Next

	Local $ret[2] = [$String, $RawDataRemainder]
	Return $ret
EndFunc   ;==>GetString

Func GetByte($RawData)
	Return Dec($RawData)
EndFunc   ;==>GetByte

Func GetShort($RawData)
	Local $BigEndian = StringMid($RawData, 3, 2) & StringMid($RawData, 1, 2)
	Return Dec($BigEndian)
EndFunc   ;==>GetShort

Func GetLong($RawData)
	Local $BigEndian = StringMid($RawData, 7, 2) & StringMid($RawData, 5, 2) & StringMid($RawData, 3, 2) & StringMid($RawData, 1, 2)
	Return Dec($BigEndian)
EndFunc   ;==>GetLong

Func GetUnsignedVariant($Size, $RawData)
	Switch $Size
		Case 1
			Local $TempStruct = DllStructCreate("byte")
			DllStructSetData($TempStruct, 1, GetByte($RawData))
			Return DllStructGetData($TempStruct, 1)
		Case 2
			Local $TempStruct = DllStructCreate("word")
			DllStructSetData($TempStruct, 1, GetShort($RawData))
			Return DllStructGetData($TempStruct, 1)
		Case 4
			Local $TempStruct = DllStructCreate("dword")
			DllStructSetData($TempStruct, 1, GetLong($RawData))
			Return DllStructGetData($TempStruct, 1)
		Case Else
			MsgBox(0, "Error", "Unexpected Size in GetValue: " & $Size)
			Return 0
	EndSwitch
EndFunc   ;==>GetUnsignedVariant

Func GetMemberValues($Member, $RawData)
	Local $ret[2]
	Local $Size = $Member[0]
	Local $ArraySize = $Member[1]

	If $ArraySize = 0 Then
		$ret = GetMemberValue($Size, $RawData)
	Else
		Local $Values[$ArraySize]
		Local $RawDataRemainder = $RawData
		Local $Out[2]

		For $i = 0 To $ArraySize - 1
			$Out = GetMemberValue($Size, $RawDataRemainder)
			$Values[$i] = $Out[0]
			$RawDataRemainder = $Out[1]
		Next

		$ret[0] = $Values
		$ret[1] = $RawDataRemainder
	EndIf

	Return $ret
EndFunc   ;==>GetMemberValues

Func GetMemberValue($Size, $RawData)
	Local $IsFloat = False

	If $Size = 3 Then
		$Size = 4
		$IsFloat = True
	EndIf

	Local $ThisData = StringLeft($RawData, 2 * $Size)
	Local $RawDataRemainder = StringTrimLeft($RawData, 2 * $Size)
	Local $Value = GetUnsignedVariant($Size, $ThisData)

	If $IsFloat Then
		$TempStruct = DllStructCreate("ulong")
		DllStructSetData($TempStruct, 1, $Value)
		$FloatStruct = DllStructCreate("float", DllStructGetPtr($TempStruct))
		$Value = DllStructGetData($FloatStruct, 1)
	Else
		If $DumpFieldsInHex Then $Value = Hex($Value, $Size * 2)
	EndIf

	Local $ret[2] = [$Value, $RawDataRemainder]

	Return $ret
EndFunc   ;==>GetMemberValue

Func GetStructMember($StructMember)
	Local $IsArray = StringInStr($StructMember, "[")

	If $IsArray Then
		Local $Split = StringSplit($StructMember, "[")
		Local $Type = $Split[1]
		$Split = StringSplit($Split[2], "]")
		Local $ArraySize = $Split[1]
		If $ArraySize = "" Then
			$ArraySize = $CurrentArraySize
			$CurrentArraySize = 0
		EndIf
		Local $Name = $Split[2]
	Else
		Local $Split = StringSplit($StructMember, " ")
		Local $Type = $Split[1]
		Local $ArraySize = 0
		Local $Name = $Split[2]
	EndIf

	Local $Size

	Switch $Type
		Case "byte"
			$Size = 1
		Case "UInt16"
			$Size = 2
		Case "Single", "float"
			$Size = 3
		Case "UInt32"
			$Size = 4
		Case "string"
			$Size = 5
	EndSwitch

	Local $ret[3] = [$Size, $ArraySize, $Name]

	Return $ret
EndFunc   ;==>GetStructMember

Func GetRawData($Block)
	Local $Split = StringSplit($Block, "}")
	Local $RawData = StringStripWS($Split[1], 8)
	$RawData = StringReplace($RawData, "0x", "")
	Return StringReplace($RawData, ",", "")
EndFunc   ;==>GetRawData